#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_PRODUTOS 200
#define TAM_STRING 100
#define ARQUIVO_ESTOQUE "estoque.txt"

struct Estoque {
    char nome[TAM_STRING];
    float preco;
    int quantidade;
};

void LimparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void carregarDoArquivo(struct Estoque *estoque, int *totalProdutos) {
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "r");
    if (arquivo == NULL) {
        *totalProdutos = 0;
        return;
    }

    *totalProdutos = 0;
    while (fscanf(arquivo, "%99[^|]|%f|%d\n", estoque[*totalProdutos].nome, 
                  &estoque[*totalProdutos].preco, &estoque[*totalProdutos].quantidade) == 3) {
        (*totalProdutos)++;
        if (*totalProdutos >= MAX_PRODUTOS) break;
    }
    fclose(arquivo);
}

void salvarNoArquivo(struct Estoque *estoque, int totalProdutos) {
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar.\n");
        return;
    }

    for (int i = 0; i < totalProdutos; i++) {
        fprintf(arquivo, "%s|%.2f|%d\n", estoque[i].nome, 
                estoque[i].preco, estoque[i].quantidade);
    }
    fclose(arquivo);
}

int main() {
    struct Estoque *estoque;
    estoque = (struct Estoque *) calloc(MAX_PRODUTOS, sizeof(struct Estoque));

    if (estoque == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    int totalProdutos = 0;
    int opcao;

    
    carregarDoArquivo(estoque, &totalProdutos);

    do {
        printf("=========================\n");
        printf("        ESTOQUE         \n");
        printf("=========================\n");
        printf("1 - Cadastrar novo produto\n");
        printf("2 - Listar todos os produtos\n");
        printf("3 - Atualizar quantidade\n");
        printf("4 - Vender produto\n");
        printf("5 - Remover produto\n");
        printf("0 - Sair\n");
        printf("--------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        LimparBufferEntrada();

        switch (opcao) {
            case 1:
                printf("--- Cadastro de Novo Produto ---\n\n");
                if (totalProdutos < MAX_PRODUTOS) {
                    printf("Digite o nome do produto: ");
                    fgets(estoque[totalProdutos].nome, TAM_STRING, stdin);
                    estoque[totalProdutos].nome[strcspn(estoque[totalProdutos].nome, "\n")] = '\0';

                    printf("Digite o preco: ");
                    scanf("%f", &estoque[totalProdutos].preco);
                    LimparBufferEntrada();

                    printf("Digite a quantidade em estoque: ");
                    scanf("%d", &estoque[totalProdutos].quantidade);
                    LimparBufferEntrada();

                    totalProdutos++;
                    salvarNoArquivo(estoque, totalProdutos); 
                    printf("\nProduto cadastrado com sucesso!\n");
                } else {
                    printf("Estoque cheio!\n");
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;

            case 2:
                printf("--- Lista de Produtos ---\n\n");
                if (totalProdutos == 0) {
                    printf("Nenhum produto cadastrado ainda.\n");
                } else {
                    for (int i = 0; i < totalProdutos; i++) {
                        printf("---------------\n");
                        printf("PRODUTO: %d\n", i + 1);
                        printf("Nome: %s\n", estoque[i].nome);
                        printf("Preco: R$ %.2f\n", estoque[i].preco);
                        printf("Quantidade: %d\n", estoque[i].quantidade);
                    }
                    printf("---------------\n");
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;

            case 3:
                if (totalProdutos == 0) {
                    printf("Nenhum produto cadastrado para atualizar.\n");
                } else {
                    printf("Digite o número do produto que deseja atualizar a quantidade: ");
                    int numProduto;
                    scanf("%d", &numProduto);
                    LimparBufferEntrada();

                    int indice = numProduto - 1;
                    if (indice >= 0 && indice < totalProdutos) {
                        printf("Digite a nova quantidade: ");
                        scanf("%d", &estoque[indice].quantidade);
                        LimparBufferEntrada();
                        salvarNoArquivo(estoque, totalProdutos); 
                        printf("\nQuantidade atualizada com sucesso!\n");
                    } else {
                        printf("\nNúmero de produto inválido.\n");
                    }
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;

            case 4: 
                if (totalProdutos == 0) {
                    printf("Nenhum produto cadastrado para venda.\n");
                } else {
                    printf("Digite o numero do produto que deseja vender: ");
                    int numProduto;
                    scanf("%d", &numProduto);
                    LimparBufferEntrada();

                    int indice = numProduto - 1;
                    if (indice >= 0 && indice < totalProdutos) {
                        printf("Digite a quantidade a vender: ");
                        int quantidadeVenda;
                        scanf("%d", &quantidadeVenda);
                        LimparBufferEntrada();

                        if (quantidadeVenda > 0 && quantidadeVenda <= estoque[indice].quantidade) {
                            estoque[indice].quantidade -= quantidadeVenda;
                            salvarNoArquivo(estoque, totalProdutos); 
                            printf("\nVenda realizada com sucesso!\n");
                            printf("Produto: %s | Quantidade restante: %d\n", estoque[indice].nome, estoque[indice].quantidade);
                        } else {
                            printf("\nQuantidade inválida ou estoque insuficiente.\n");
                        }
                    } else {
                        printf("\nNúmero de produto inválido.\n");
                    }
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;

            case 5: 
                if (totalProdutos == 0) {
                    printf("Nenhum produto cadastrado para remover.\n");
                } else {
                    printf("Digite o número do produto que deseja remover: ");
                    int numProduto;
                    scanf("%d", &numProduto);
                    LimparBufferEntrada();

                    int indice = numProduto - 1;
                    if (indice >= 0 && indice < totalProdutos) {
                        for (int i = indice; i < totalProdutos - 1; i++) {
                            estoque[i] = estoque[i + 1]; 
                        }
                        totalProdutos--;
                        salvarNoArquivo(estoque, totalProdutos); 
                        printf("\nProduto removido com sucesso!\n");
                    } else {
                        printf("\nNúmero de produto inválido.\n");
                    }
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;

            case 0:
                printf("\nSaindo do sistema...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
        }

    } while (opcao != 0);

    free(estoque);
    printf("Memória liberada com sucesso.\n");

    return 0;
}