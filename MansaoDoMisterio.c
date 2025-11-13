/*  
    MANSÃO DO MISTÉRIO
    PROJETO DE JOGO DETETIVE: EXPLORAÇÃO DA MANSÃO

    OBJETIVO: Criar uma arvore bínaria para representar o mapa da mansão
    Permitir a exxploração interativa da mansão a partir do Hall de entrada, escolhendo ir para a esquerda (e) ou para a direita (d).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura das salas da mansão
typedef struct Sala{
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
}Sala;

// Cria a estrutura da sala
Sala* criarSala(char* nome) {
    Sala* novo = (Sala*) malloc(sizeof(Sala));
    strcpy(novo->nome, nome);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// função para explorar a arvore da mansão interativamente
void explorarSalas(Sala* raiz) {
    Sala* atual = raiz;
    char escolha;
    
    printf("\n=== EXPLORAÇÃO DA MANSÃO ===\n");
    
    while (atual != NULL) {
        printf("\nVocê está na: %s\n", atual->nome);
        
        // Verifica se é uma sala final (nó folha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Esta sala não tem saídas. Exploração concluída!\n");
            break;
        }
        
        // Mostra opções disponíveis
        printf("Opções disponíveis:\n");
        if (atual->esquerda != NULL) {
            printf("  (e) - Ir para a esquerda: %s\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf("  (d) - Ir para a direita: %s\n", atual->direita->nome);
        }
        printf("  (s) - Sair do jogo\n");
        
        // Obtém a escolha do jogador
        printf("Para onde deseja ir? e (esquerda), d (direita) ou s (sair): ");
        scanf(" %c", &escolha);
        
        // Processa a escolha
        switch (escolha) {
            case 'e':
                // Verifica se a saída pela esquerda existe
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda;
                } else {
                    printf("Não há saída pela esquerda!\n");
                }
                break;               
            case 'd':
                // Verifica se a saída pela direita existe
                if (atual->direita != NULL) {
                    atual = atual->direita;
                } else {
                    printf("Não há saída pela direita!\n");
                }
                break;
            case 's':
                printf("Saindo do jogo...\n");
                return;
            default:
                printf("Opção inválida! Use 'e', 'd' ou 's'.\n");
        }
    }
    printf("\n=== FIM DA EXPLORAÇÃO ===\n");
}

// Função para liberar toda a memória alocada para uma arvore binária
void liberarSalas(Sala* raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

// Função principal
int main() {
    // Cria as salas da mansão
    Sala* hall = criarSala("Hall de Entrada");
    Sala* sala_estar = criarSala("Sala de Estar");
    Sala* quarto = criarSala("Quarto");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* jantar = criarSala("Sala de Jantar");
    Sala* banheiro = criarSala("Banheiro");
    Sala* jogos = criarSala("Sala de Jogos");
    Sala* jardim = criarSala("Jardim");
    Sala* garagem = criarSala("Garagem");
    Sala* porao = criarSala("Porão Misterioso");
    Sala* escritorio = criarSala("Escritório");
    Sala* lavanderia = criarSala("Lavanderia");

    // ========== MONTAGEM DO MAPA DA MANSÃO ==========
    // Conecta as salas de forma lógica, criando a estrutura da árvore

     // Do Hall de Entrada: esquerda -> Sala de Estar, direita -> Biblioteca
    hall->esquerda = sala_estar;
    hall->direita = biblioteca;
    
    // Da Sala de Estar: esquerda -> Cozinha, direita -> Quarto
    sala_estar->esquerda = cozinha;
    sala_estar->direita = quarto;
    
    // Da Cozinha: direita -> Sala de Jantar (apenas direita)
    cozinha->direita = jantar;

    // Da Sala de Jantar: esquerda -> Lavanderia
    jantar->esquerda = lavanderia;

    // Do Quarto: esquerda -> Banheiro (apenas esquerda)
    quarto->esquerda = banheiro;
    
    // Da Biblioteca: esquerda -> Sala de Jogos, direita -> Escritório
    biblioteca->esquerda = jogos;
    biblioteca->direita = escritorio;

    // Do Escritório: esquerda -> Porão Misterioso, direita -> Jardim
    escritorio->direita = porao;
    escritorio->esquerda = jardim;

    // Do Jardim: direita -> Garagem (apenas direita)
    jardim->direita = garagem;

    printf("===  MANSÃO DO MISTÉRIO ===\n");
    // Inicia a exploração a partir do Hall de Entrada (raiz da árvore)
    explorarSalas(hall);
    // Libera toda a memória alocada para as salas
    liberarSalas(hall);

    return 0;
}