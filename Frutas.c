#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Fruta {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
    struct Fruta *prox;
} Fruta;

Fruta *lista_frutas = NULL;

Fruta *criar_fruta(int codigo, char *nome, int quantidade, float preco) {
    Fruta *nova_fruta = (Fruta *)malloc(sizeof(Fruta));
    if (nova_fruta) {
        nova_fruta->codigo = codigo;
        strcpy(nova_fruta->nome, nome);
        nova_fruta->quantidade = quantidade;
        nova_fruta->preco = preco;
        nova_fruta->prox = NULL;
    }
    return nova_fruta;
}

int codigo_existe(int codigo) {
    Fruta *atual = lista_frutas;
    while (atual != NULL) {
        if (atual->codigo == codigo) {
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
}

void cadastrar_fruta() {
    int codigo, quantidade;
    char nome[50];
    float preco;

    printf("Digite o codigo da fruta: ");
    scanf("%d", &codigo);

    if (codigo_existe(codigo)) {
        printf("Codigo ja existente. Tente outro.\n");
        return;
    }

    printf("Digite o nome da fruta: ");
    scanf("%s", nome);
    printf("Digite a quantidade em estoque: ");
    scanf("%d", &quantidade);
    printf("Digite o preco da fruta: ");
    scanf("%f", &preco);

    Fruta *nova_fruta = criar_fruta(codigo, nome, quantidade, preco);
    if (nova_fruta) {
        nova_fruta->prox = lista_frutas;
        lista_frutas = nova_fruta;
        printf("Fruta cadastrada com sucesso!\n");
    }
}

void listar_frutas() {
    Fruta *atual = lista_frutas;
    if (atual == NULL) {
        printf("Nenhuma fruta cadastrada.\n");
        return;
    }
    while (atual != NULL) {
        printf("Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n",
               atual->codigo, atual->nome, atual->quantidade, atual->preco);
        atual = atual->prox;
    }
}

Fruta *buscar_fruta(int codigo) {
    Fruta *atual = lista_frutas;
    while (atual != NULL) {
        if (atual->codigo == codigo) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void alterar_fruta() {
    int codigo;
    printf("Digite o codigo da fruta a ser alterada: ");
    scanf("%d", &codigo);

    Fruta *fruta = buscar_fruta(codigo);
    if (!fruta) {
        printf("Fruta nao encontrada.\n");
        return;
    }

    printf("Digite o novo nome da fruta: ");
    scanf("%s", fruta->nome);
    printf("Digite a nova quantidade em estoque: ");
    scanf("%d", &fruta->quantidade);
    printf("Digite o novo preco da fruta: ");
    scanf("%f", &fruta->preco);

    printf("Fruta alterada com sucesso!\n");
}

void excluir_fruta() {
    int codigo;
    printf("Digite o codigo da fruta a ser excluida: ");
    scanf("%d", &codigo);

    Fruta *atual = lista_frutas, *anterior = NULL;
    while (atual != NULL && atual->codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Fruta nao encontrada.\n");
        return;
    }

    if (atual->quantidade > 0) {
        printf("Nao e possivel excluir uma fruta com estoque.\n");
        return;
    }

    if (anterior == NULL) {
        lista_frutas = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }
    free(atual);
    printf("Fruta excluida com sucesso!\n");
}

void registrar_venda(int codigo, int quantidade, float total) {
    FILE *arquivo = fopen("vendas.txt", "a");
    if (arquivo) {
        fprintf(arquivo, "Codigo: %d, Quantidade: %d, Total: R$ %.2f\n", codigo, quantidade, total);
        fclose(arquivo);
    }
}

void vender_fruta() {
    int codigo, quantidade;
    printf("Digite o codigo da fruta: ");
    scanf("%d", &codigo);

    Fruta *fruta = buscar_fruta(codigo);
    if (!fruta) {
        printf("Fruta nao encontrada.\n");
        return;
    }

    printf("Digite a quantidade a ser vendida: ");
    scanf("%d", &quantidade);

    if (quantidade > fruta->quantidade) {
        printf("Quantidade insuficiente em estoque.\n");
        return;
    }

    fruta->quantidade -= quantidade;
    float total = quantidade * fruta->preco;
    printf("Venda realizada com sucesso! Total: R$ %.2f\n", total);
    registrar_venda(codigo, quantidade, total);
}

void liberar_memoria() {
    Fruta *atual = lista_frutas;
    while (atual != NULL) {
        Fruta *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}

int main() {
    int opcao;
    do {
        printf("\n--- Mercado de Frutas ---\n");
        printf("1. Cadastrar fruta\n");
        printf("2. Listar frutas\n");
        printf("3. Alterar fruta\n");
        printf("4. Excluir fruta\n");
        printf("5. Vender fruta\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrar_fruta();
                break;
            case 2:
                listar_frutas();
                break;
            case 3:
                alterar_fruta();
                break;
            case 4:
                excluir_fruta();
                break;
            case 5:
                vender_fruta();
                break;
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 6);

    liberar_memoria();
    return 0;
}
