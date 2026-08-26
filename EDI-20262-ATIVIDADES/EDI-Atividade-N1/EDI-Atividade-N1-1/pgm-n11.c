
#include <stdio.h>

#define TAMANHO_TURMA 5

void exibirCabecalho(void);
float calcularMedia(float vetor[], int tamanho);
void simularAjuste(float notaOriginal, float bonus);
void aplicarBonus(float *nota, float bonus);

int main(void)
{
    float notas[TAMANHO_TURMA];
    float bonus;
    float mediaInicial;
    float mediaFinal;
    int i;

    exibirCabecalho();

    for (i = 0; i < TAMANHO_TURMA; i++)
    {
        printf("Nota do aluno %d: ", i + 1);
        scanf("%f", &notas[i]);
    }

    printf("Informe o valor do bonus a aplicar: ");
    scanf("%f", &bonus);

    printf("--- Media da turma antes do ajuste ---\n");

    mediaInicial = calcularMedia(notas, TAMANHO_TURMA);

    printf("Media inicial: %.2f\n", mediaInicial);

    printf("--- Simulacao do ajuste (passagem por valor) ---\n");

    simularAjuste(notas[0], bonus);

    printf("Nota do aluno 1 apos a simulacao (inalterada): %.2f\n", notas[0]);

    printf("--- Aplicacao real do bonus (passagem por referencia) ---\n");

    for (i = 0; i < TAMANHO_TURMA; i++)
    {
        aplicarBonus(&notas[i], bonus);
    }

    printf("Bonus de %.2f aplicado a todas as notas da turma.\n", bonus);

    printf("--- Notas finais da turma ---\n");

    for (i = 0; i < TAMANHO_TURMA; i++)
    {
        printf("Aluno %d: %.2f\n", i + 1, notas[i]);
    }

    printf("--- Media da turma apos o ajuste ---\n");

    mediaFinal = calcularMedia(notas, TAMANHO_TURMA);

    printf("Media final: %.2f\n", mediaFinal);

    return 0;
}

void exibirCabecalho(void)
{
    printf("=================================\n");
    printf("SISTEMA DE NOTAS- TURMA ADS\n");
    printf("=================================\n");
}

float calcularMedia(float vetor[], int tamanho)
{
    float soma = 0.0;
    int i;

    /*
     * Em C, vetores passados como parâmetros são tratados como
     * ponteiros para o primeiro elemento. Por isso, as alterações
     * feitas nos elementos dentro da função podem afetar o vetor
     * original.
     */
    for (i = 0; i < tamanho; i++)
    {
        soma += vetor[i];
    }

    return soma / tamanho;
}

void simularAjuste(float notaOriginal, float bonus)
{
    float resultado;

    /*
     * Os parâmetros são recebidos por valor, portanto a função
     * trabalha com cópias dos valores originais e não altera
     * a variável do chamador.
     */
    resultado = notaOriginal + bonus;

    printf(
        "Simulacao para o aluno 1: %.2f + %.2f = %.2f (nao aplicado ainda)\n",
        notaOriginal,
        bonus,
        resultado
    );
}

void aplicarBonus(float *nota, float bonus)
{
    /*
     * O endereço da nota é recebido por meio de um ponteiro.
     * Assim, *nota acessa diretamente a posição original do vetor
     * e permite alterar seu valor.
     */
    *nota += bonus;
}