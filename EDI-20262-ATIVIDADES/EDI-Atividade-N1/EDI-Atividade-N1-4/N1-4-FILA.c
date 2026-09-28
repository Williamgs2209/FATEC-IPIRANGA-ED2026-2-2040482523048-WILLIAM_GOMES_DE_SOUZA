#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    int id;
    char nome[50];
    struct no *prox;
} No;

typedef No *NoPtr;

typedef struct {
    NoPtr ini;
    NoPtr fim;
} Cabecalho;

typedef Cabecalho *Fila;


/* =========================================================
   FUNCAO: Criar
   OBJETIVO: Criar uma nova fila vazia
   ========================================================= */
Fila Criar() {
    Fila f = (Fila) malloc(sizeof(Cabecalho));

    if (f != NULL) {
        f->ini = NULL;
        f->fim = NULL;
    }

    return f;
}


/* =========================================================
   FUNCAO: vazia
   OBJETIVO: Verificar se a fila possui elementos
   RETORNO:
       1 = fila vazia
       0 = fila possui elementos
   ========================================================= */
int vazia(Fila f) {

    if (f == NULL || f->ini == NULL) {
        return 1;
    }

    return 0;
}


/* =========================================================
   FUNCAO: inserir
   OBJETIVO: Inserir um novo paciente no final da fila
   RETORNO:
       1 = insercao realizada
       0 = erro de memoria
   ========================================================= */
int inserir(Fila f, int id, const char *nome) {

    NoPtr novo = (NoPtr) malloc(sizeof(No));

    if (novo == NULL) {
        return 0;
    }


    novo->id = id;

    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';

    novo->prox = NULL;

    if (f->ini == NULL) {
        f->ini = novo;
    } else {
        f->fim->prox = novo;
    }

    f->fim = novo;

    return 1;
}


/* =========================================================
   FUNCAO: listarFila
   OBJETIVO: Exibir todos os pacientes da fila
   ========================================================= */
void listarFila(Fila f) {

    if (f == NULL || f->ini == NULL) {
        printf("\n[AVISO] A fila esta vazia.\n");
        return;
    }

    NoPtr atual = f->ini;

    printf("\n=== FILA DE PACIENTES ===\n");

    while (atual != NULL) {

        printf("ID: %d - Nome: %s\n",
               atual->id,
               atual->nome);

        atual = atual->prox;
    }

    printf("=========================\n");
}


/* =========================================================
   FUNCAO: atender
   OBJETIVO:
       Remover o primeiro paciente da fila,
       respeitando a regra FIFO.
   ========================================================= */
void atender(Fila f) {

    if (vazia(f)) {
        printf("\n[AVISO] A fila esta vazia. Nao ha pacientes para atender.\n");
        return;
    }

    NoPtr paciente = f->ini;

    printf("\n=== ATENDIMENTO ===\n");
    printf("Paciente atendido:\n");
    printf("ID: %d\n", paciente->id);
    printf("Nome: %s\n", paciente->nome);

    /*
       O inicio da fila passa a ser
       o proximo paciente.
    */
    f->ini = paciente->prox;

    /*
       Se a fila ficou vazia depois da remocao,
       o fim tambem deve receber NULL.
    */
    if (f->ini == NULL) {
        f->fim = NULL;
    }

    /*
       Libera a memoria do paciente removido.
    */
    free(paciente);

    printf("Paciente removido da fila com sucesso.\n");
}


/* =========================================================
   FUNCAO: primeiro
   OBJETIVO:
       Consultar o primeiro paciente da fila
       sem remove-lo.
   ========================================================= */
void primeiro(Fila f) {

    if (vazia(f)) {
        printf("\n[AVISO] A fila esta vazia. Nao ha proximo paciente.\n");
        return;
    }

    printf("\n=== PROXIMO PACIENTE ===\n");
    printf("ID: %d\n", f->ini->id);
    printf("Nome: %s\n", f->ini->nome);
}


/* =========================================================
   FUNCAO: tamanho
   OBJETIVO:
       Contar a quantidade de pacientes presentes
       na fila.
   RETORNO:
       Quantidade de pacientes
   ========================================================= */
int tamanho(Fila f) {

    int quantidade = 0;
    NoPtr atual;

    if (f == NULL) {
        return 0;
    }

    atual = f->ini;

    while (atual != NULL) {

        quantidade++;

        atual = atual->prox;
    }

    return quantidade;
}


/* =========================================================
   FUNCAO: destruir
   OBJETIVO:
       Liberar toda a memoria utilizada pela fila.
   ========================================================= */
void destruir(Fila f) {

    NoPtr atual;
    NoPtr proximo;

    if (f == NULL) {
        return;
    }

    atual = f->ini;

    /*
       Percorre todos os pacientes da fila
       liberando cada no.
    */
    while (atual != NULL) {

        proximo = atual->prox;

        free(atual);

        atual = proximo;
    }

    /*
       Depois de liberar todos os pacientes,
       libera o cabecalho da fila.
    */
    free(f);
}


/* =========================================================
   FUNCAO PRINCIPAL
   ========================================================= */
int main() {

    Fila filaAtendimento = Criar();

    int opcao;
    int id;
    char nome[50];

    if (filaAtendimento == NULL) {
        printf("Erro ao criar a fila.\n");
        return 1;
    }

    do {

        printf("\n");
        printf("============================================\n");
        printf("     SISTEMA HOSPITALAR - FATEC IPIRANGA\n");
        printf("============================================\n");
        printf("1. Chegada de Paciente (Inserir na Fila)\n");
        printf("2. Listar Fila de Pacientes\n");
        printf("3. Atender Paciente (Remover da Fila)\n");
        printf("4. Consultar Proximo Paciente\n");
        printf("5. Mostrar Tamanho da Fila\n");
        printf("6. Verificar se a Fila esta Vazia\n");
        printf("7. Limpar/Destruir Fila\n");
        printf("0. Sair\n");
        printf("============================================\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);

        switch (opcao) {

            /* =========================================
               OPCAO 1 - INSERIR
               ========================================= */
            case 1:

                printf("\nInforme o ID do paciente: ");
                scanf("%d", &id);

                getchar();

                printf("Informe o Nome do paciente: ");

                fgets(nome, sizeof(nome), stdin);

                nome[strcspn(nome, "\n")] = '\0';

                if (inserir(filaAtendimento, id, nome)) {

                    printf("\n>> Paciente inserido com sucesso!\n");

                } else {

                    printf("\n>> Erro ao inserir paciente.\n");
                }

                break;


            /* =========================================
               OPCAO 2 - LISTAR
               ========================================= */
            case 2:

                listarFila(filaAtendimento);

                break;


            /* =========================================
               OPCAO 3 - ATENDER
               ========================================= */
            case 3:

                atender(filaAtendimento);

                break;


            /* =========================================
               OPCAO 4 - CONSULTAR PRIMEIRO
               ========================================= */
            case 4:

                primeiro(filaAtendimento);

                break;


            /* =========================================
               OPCAO 5 - TAMANHO
               ========================================= */
            case 5:

                printf("\nQuantidade de pacientes na fila: %d\n",
                       tamanho(filaAtendimento));

                break;


            /* =========================================
               OPCAO 6 - VERIFICAR FILA VAZIA
               ========================================= */
            case 6:

                if (vazia(filaAtendimento)) {

                    printf("\n[AVISO] A fila esta vazia.\n");

                } else {

                    printf("\n[INFO] A fila possui pacientes aguardando atendimento.\n");
                }

                break;


            /* =========================================
               OPCAO 7 - DESTRUIR FILA
               ========================================= */
            case 7:

                /*
                   A fila sera destruida e toda a memoria
                   dos pacientes sera liberada.
                */
                destruir(filaAtendimento);

                filaAtendimento = NULL;

                printf("\n>> Fila destruida e memoria liberada.\n");

                /*
                   Cria uma nova fila vazia para que o
                   programa possa continuar funcionando.
                */
                filaAtendimento = Criar();

                if (filaAtendimento == NULL) {

                    printf("Erro ao recriar a fila.\n");
                    return 1;
                }

                break;


            /* =========================================
               OPCAO 0 - SAIR
               ========================================= */
            case 0:

                printf("\nEncerrando o sistema...\n");

                break;


            /* =========================================
               OPCAO INVALIDA
               ========================================= */
            default:

                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);


    /*
       Antes de encerrar o programa,
       libera toda a memoria restante.
    */
    destruir(filaAtendimento);

    return 0;
}