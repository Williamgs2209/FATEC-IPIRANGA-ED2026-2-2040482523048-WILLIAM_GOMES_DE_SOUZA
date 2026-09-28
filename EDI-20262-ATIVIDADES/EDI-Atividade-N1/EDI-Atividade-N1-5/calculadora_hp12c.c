#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define CAPACIDADE 4
#define TAM_EXPRESSAO 512
#define TAM_TOKEN 64

typedef struct {
    double registros[CAPACIDADE]; /* X, Y, Z, T */
    int quantidade;
} PilhaHP12c;

static void inicializar(PilhaHP12c *pilha) {
    pilha->quantidade = 0;
    for (int i = 0; i < CAPACIDADE; i++) pilha->registros[i] = 0.0;
}

static int pilha_vazia(const PilhaHP12c *pilha) { return pilha->quantidade == 0; }
static int pilha_cheia(const PilhaHP12c *pilha) { return pilha->quantidade == CAPACIDADE; }

/* O ?ndice 0 representa X. Ao atingir quatro registros, T ? descartado. */
static void empilhar(PilhaHP12c *pilha, double valor) {
    int limite = pilha_cheia(pilha) ? CAPACIDADE - 1 : pilha->quantidade;
    for (int i = limite; i > 0; i--) pilha->registros[i] = pilha->registros[i - 1];
    pilha->registros[0] = valor;
    if (!pilha_cheia(pilha)) pilha->quantidade++;
}

static int desempilhar(PilhaHP12c *pilha, double *valor) {
    if (pilha_vazia(pilha)) return 0;
    *valor = pilha->registros[0];
    for (int i = 0; i < pilha->quantidade - 1; i++)
        pilha->registros[i] = pilha->registros[i + 1];
    pilha->quantidade--;
    pilha->registros[pilha->quantidade] = 0.0;
    return 1;
}

static int converter_operando(const char *token, double *valor) {
    char *fim;
    errno = 0;
    long numero = strtol(token, &fim, 10);
    if (token[0] == '\0' || *fim != '\0' || errno == ERANGE) return 0;
    *valor = (double)numero;
    return 1;
}

static const char *nome_operacao(char operador) {
    switch (operador) {
        case '+': return "Soma";
        case '-': return "Sub";
        case '*': return "Mult";
        default: return "Div";
    }
}

static int processar_operador(PilhaHP12c *pilha, char operador, double *resultado) {
    double x, y;
    if (pilha->quantidade < 2) return 0;
    desempilhar(pilha, &x);
    desempilhar(pilha, &y);
    switch (operador) {
        case '+': *resultado = y + x; break;
        case '-': *resultado = y - x; break;
        case '*': *resultado = y * x; break;
        case '/':
            if (x == 0.0) {
                empilhar(pilha, y);
                empilhar(pilha, x);
                return -1;
            }
            *resultado = y / x;
            break;
        default:
            empilhar(pilha, y);
            empilhar(pilha, x);
            return -2;
    }
    empilhar(pilha, *resultado);
    printf("[LOG] Token %c -> Y=%.0f, X=%.0f -> %s = %.2f\n",
           operador, y, x, nome_operacao(operador), *resultado);
    return 1;
}

static int processar_expressao(char *expressao, double *resultado) {
    PilhaHP12c pilha;
    inicializar(&pilha);
    int quantidade_operadores = 0;
    int operandos_seguidos = 0;
    char *token = strtok(expressao, " \t\r\n");

    while (token != NULL) {
        double valor;
        if (converter_operando(token, &valor)) {
            if (++operandos_seguidos > CAPACIDADE) {
                puts("Erro: cada sequencia pode conter no maximo quatro operandos antes de um operador.");
                return 0;
            }
            empilhar(&pilha, valor);
            printf("[LOG] Token %s -> Empilhado. Visor (X) = %.2f\n", token, valor);
        } else if (strlen(token) == 1 && strchr("+-*/", token[0]) != NULL) {
            double parcial;
            int status = processar_operador(&pilha, token[0], &parcial);
            if (status == 0) {
                puts("Erro: operador recebido sem pelo menos dois operandos.");
                return 0;
            }
            if (status == -1) {
                puts("Erro: divisao por zero nao e permitida.");
                return 0;
            }
            quantidade_operadores++;
            operandos_seguidos = 0;
        } else {
            printf("Erro: token invalido: %s\n", token);
            return 0;
        }
        token = strtok(NULL, " \t\r\n");
    }

    if (quantidade_operadores == 0) {
        puts("Erro: a expressao deve conter pelo menos um operador valido.");
        return 0;
    }
    if (pilha.quantidade != 1) {
        puts("Erro: expressao RPN invalida; deve restar somente um valor na pilha.");
        return 0;
    }
    *resultado = pilha.registros[0];
    return 1;
}

int main(void) {
    char expressao[TAM_EXPRESSAO];
    char resposta[32];

    do {
        puts("====================================================");
        puts("       SIMULADOR HP12c - FATEC IPIRANGA");
        puts("====================================================");
        puts("");
        puts("Digite a expressao em RPN");
        puts("(ex: 5 1 2 + 4 * + 3 -):");
        printf("> ");
        if (fgets(expressao, sizeof(expressao), stdin) == NULL) break;
        puts("");

        double resultado;
        if (processar_expressao(expressao, &resultado)) {
            puts("");
            puts("----------------------------------------------------");
            printf("RESULTADO FINAL NO VISOR (X): %.2f\n", resultado);
            puts("----------------------------------------------------");
        }

        puts("");
        printf("Deseja realizar nova operacao? (S/N): ");
        if (fgets(resposta, sizeof(resposta), stdin) == NULL) break;
        for (size_t i = 0; resposta[i] != '\0'; i++)
            resposta[i] = (char)toupper((unsigned char)resposta[i]);
    } while (resposta[0] == 'S');

    puts("");
    puts("Obrigado por usar nossa Calculadora Fatec-HP12c");
    return 0;
}


