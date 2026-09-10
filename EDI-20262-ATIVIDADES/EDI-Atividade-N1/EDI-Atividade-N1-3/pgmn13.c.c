#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VALOR_PREMIUM 100.0

typedef struct {
    char placa[8];
    float valor;
} Veiculo;

void exibirCabecalho(void);
void preencherFrota(Veiculo *frota, int quantidade);
void exibirFrotaRecursivo(Veiculo *frota, int indice, int quantidade);
float calcularValorTotalRecursivo(Veiculo *frota, int indice, int quantidade);
int buscarVeiculoRecursivo(Veiculo *frota, int indice, int quantidade, char placa[8]);
int contarPremiumRecursivo(Veiculo *frota, int indice, int quantidade);

void exibirCabecalho(void)
{
    printf("=================================\n");
    printf("LAVA - RAPIDO BRILHO TOTAL - FILA DE ATENDIMENTO\n");
    printf("=================================\n");
}

void preencherFrota(Veiculo *frota, int quantidade)
{
    int i;

    for (i = 0; i < quantidade; i++)
    {
        printf("Placa do veiculo %d: ", i + 1);
        scanf("%7s", (frota + i)->placa);

        printf("Valor do servico (R$): ");
        scanf("%f", &(frota + i)->valor);
    }
}

void exibirFrotaRecursivo(Veiculo *frota, int indice, int quantidade)
{
    if (indice == quantidade)
    {
        return;
    }

    printf("%d) Placa: %s | Valor: R$ %.2f\n",
           indice + 1,
           (frota + indice)->placa,
           (frota + indice)->valor);

    exibirFrotaRecursivo(frota, indice + 1, quantidade);
}

float calcularValorTotalRecursivo(Veiculo *frota, int indice, int quantidade)
{
    /*
     * Caso base: quando indice == quantidade, o fim do vetor foi alcancado
     * e nao ha mais valores para somar, retornando 0.0f.
     * Passo recursivo: soma o valor do veiculo atual ao resultado da
     * chamada recursiva para o proximo indice.
     */
    if (indice == quantidade)
    {
        return 0.0f;
    }

    return (frota + indice)->valor +
           calcularValorTotalRecursivo(frota, indice + 1, quantidade);
}

int buscarVeiculoRecursivo(Veiculo *frota, int indice, int quantidade, char placa[8])
{
    /*
     * Caso base 1: quando indice == quantidade, toda a frota foi
     * percorrida e a placa nao foi encontrada, retornando -1.
     * Caso base 2: quando a placa atual coincide com a placa pesquisada,
     * retorna o indice em que o veiculo foi encontrado.
     * Passo recursivo: avanca para o proximo veiculo.
     */
    if (indice == quantidade)
    {
        return -1;
    }

    if (strcmp((frota + indice)->placa, placa) == 0)
    {
        return indice;
    }

    return buscarVeiculoRecursivo(frota, indice + 1, quantidade, placa);
}

int contarPremiumRecursivo(Veiculo *frota, int indice, int quantidade)
{
    if (indice == quantidade)
    {
        return 0;
    }

    if ((frota + indice)->valor >= VALOR_PREMIUM)
    {
        return 1 + contarPremiumRecursivo(frota, indice + 1, quantidade);
    }

    return contarPremiumRecursivo(frota, indice + 1, quantidade);
}

int main(void)
{
    int quantidade;
    char placaBusca[8];
    int posicaoBusca;
    int totalPremium;
    float valorTotal;
    Veiculo *frota;

    exibirCabecalho();

    printf("Quantos veiculos serao atendidos hoje? ");
    scanf("%d", &quantidade);

    /*
     * O vetor e alocado dinamicamente de acordo com a quantidade
     * informada pelo usuario.
     */
    frota = (Veiculo *)malloc(quantidade * sizeof(Veiculo));

    /*
     * A verificacao contra NULL garante que o ponteiro somente seja
     * utilizado caso a alocacao de memoria tenha sido realizada.
     */
    if (frota == NULL)
    {
        printf("Erro ao alocar memoria para a frota.\n");
        return 1;
    }

    preencherFrota(frota, quantidade);

    printf("\n--- Veiculos atendidos hoje ---\n");
    exibirFrotaRecursivo(frota, 0, quantidade);

    printf("\n--- Busca recursiva por placa ---\n");
    printf("Placa a ser pesquisada: ");
    scanf("%7s", placaBusca);

    posicaoBusca = buscarVeiculoRecursivo(
        frota, 0, quantidade, placaBusca
    );

    if (posicaoBusca != -1)
    {
        printf("Veiculo encontrado na posicao %d! Placa: %s | Valor: R$ %.2f\n",
               posicaoBusca + 1,
               (frota + posicaoBusca)->placa,
               (frota + posicaoBusca)->valor);
    }
    else
    {
        printf("Veiculo nao encontrado na frota.\n");
    }

    valorTotal = calcularValorTotalRecursivo(frota, 0, quantidade);
    totalPremium = contarPremiumRecursivo(frota, 0, quantidade);

    printf("\n--- Resumo do dia ---\n");
    printf("Total de veiculos atendidos: %d\n", quantidade);
    printf("Servicos premium (>= R$ 100.00): %d\n", totalPremium);
    printf("Valor total arrecadado: R$ %.2f\n", valorTotal);

    /*
     * A memoria alocada e liberada uma unica vez ao final da execucao.
     * Em seguida, o ponteiro e definido como NULL para evitar seu uso
     * posterior como referencia para memoria ja liberada.
     */
    free(frota);
    frota = NULL;

    printf("\nMemoria da frota liberada com sucesso. Sistema encerrado.\n");

    return 0;
}