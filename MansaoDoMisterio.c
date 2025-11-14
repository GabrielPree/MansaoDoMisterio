/*
    PROJETO DE JOGO DETETIVE: EXPLORAÇÃO DA MANSÃO

    OBJETIVO: Criar uma arvore bínaria para representar o mapa da mansão
    Permitir a exxploração interativa da mansão a partir do Hall de entrada, escolhendo ir para a esquerda (e) ou para a direita (d).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição das estruturas
// Estrutura das salas da mansão
typedef struct Sala{
    char nome[50];
    char pista[100];            // Pista da sala
    int pista_coletada;        // int para controlar se já foi coletada
    struct Sala* esquerda;
    struct Sala* direita;
}Sala;

// Estrutura para armazenar pistas na BST
typedef struct Pistas {
    char pista[100];
    char localizacao[50];       // Local em que a pista foi coletada
    struct Pistas* esquerda;
    struct Pistas* direita;
} Pistas;

// --- FUNÇÕES DA ARVORE DA MANSÃO ---

// Cria a estrutura da sala
Sala* criarSala(char* nome, char* pista) {
    Sala* novo = (Sala*)malloc(sizeof(Sala));
    strcpy(novo->nome, nome);
    if (pista != NULL) {
        strcpy(novo->pista, pista);
        novo->pista_coletada = 0;  // Pista não coletada inicialmente
    } else {
        strcpy(novo->pista, "");
        novo->pista_coletada = 1;  // Sem pista = considerada "coletada"
    }
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// --- FUNÇÕES DA ARVORE DE PISTAS ---

// Função para criar um nó para a árvore de pistas
Pistas* criarPistas(char* pista, char* localizacao) {
    Pistas* novo = (Pistas*)malloc(sizeof(Pistas));
    strcpy(novo->pista, pista);
    strcpy(novo->localizacao, localizacao);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Função para inserir uma nova pista na árvore de pistas (BST)
Pistas* inserirPista(Pistas* raiz, char* pista, char* localizacao) {
    if (raiz == NULL) {
        return criarPistas(pista, localizacao);
    }
    // Comparação da pista para inserir na árvore
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista, localizacao);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista, localizacao);
    }
    // não insere pista se já estiver na árvore
    return raiz;
}

// Função para exibir a árvore de pistas 
void exibirPistas(Pistas* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("  • %s - Local: %s\n", raiz->pista, raiz->localizacao);
        exibirPistas(raiz->direita);
    }
}

// Função para coletar a pista da sala atual e adiciona à árvore de pistas
Pistas* coletarPista(Sala* sala_atual, Pistas* arvore_pistas) {
    if (sala_atual == NULL || sala_atual->pista_coletada) {
        return arvore_pistas;
    }
    
    printf("\nPista Encontrada: %s\n", sala_atual->pista);
    sala_atual->pista_coletada = 1;  // Marca como coletada
    
    // Insere na árvore de pistas
    return inserirPista(arvore_pistas, sala_atual->pista, sala_atual->nome);
}

// --- FUNÇÕES AUXILIARES ---

// Função para liberar toda a memória alocada para uma arvore binária
void liberarSalas(Sala* raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

// Função para liberar a memória da árvore de pistas
void liberarPistas(Pistas* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// --- FUNÇÃO DO MENU DE EXPLORACAO ---

// função para explorar a arvore da mansão interativamente
void explorarSalasComPistas(Sala* raiz) {
    Sala* atual = raiz;
    Pistas* arvore_pistas = NULL; // Arvore BST para armazenar pistas
    char escolha;
    int total_pistas = 0;
    
    printf("\n=== EXPLORAÇÃO DA MANSÃO DO MISTÉRIO ===\n");
    printf("Colete pistas nos cômodos para resolver o caso!\n");
    printf("Você deve optar por sair (s) a qualquer momento para encerrar.\n");

     while (1) {  // Loop infinito - só para quando usuário escolher 's'
        printf("\nVocê está na: %s\n", atual->nome);
        
        // Coleta pista se disponível
        if (!atual->pista_coletada && strlen(atual->pista) > 0) {
            arvore_pistas = coletarPista(atual, arvore_pistas);
            total_pistas++;
        }
        
        // Mostra opções disponíveis
        printf("\nOpções disponíveis:\n");
        if (atual->esquerda != NULL) {
            printf("  (e) - Ir para a esquerda: %s\n", atual->esquerda->nome);
        } else {
            printf("  (e) - [Sem saída pela esquerda]\n");
        }
        if (atual->direita != NULL) {
            printf("  (d) - Ir para a direita: %s\n", atual->direita->nome);
        } else {
            printf("  (d) - [Sem saída pela direita]\n");
        }
        printf("  (p) - Ver pistas coletadas (%d pistas)\n", total_pistas);
        printf("  (s) - SAIR da exploração\n");
        
        // Obtém a escolha do jogador
        printf("\nPara onde deseja ir? ");
        scanf(" %c", &escolha);
        
        // Processa a escolha
        switch (escolha) {
            case 'e':
                // Verifica se a saída pela esquerda existe
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda;
                } else {
                    printf("Não há saída para esquerda!\n");
                }
                break;
                
            case 'd':
                // Verifica se a saída pela direita existe
                if (atual->direita != NULL) {
                    atual = atual->direita;
                } else {
                    printf("Não há saída para direita!\n");
                }
                break;
                
            case 'p':
                // Exibe as pistas coletadas
                printf("\n=== PISTAS COLETADAS ===\n");
                if (arvore_pistas == NULL) {
                    printf("  Nenhuma pista coletada ainda.\n");
                } else {
                    exibirPistas(arvore_pistas);
                }
                break;      
            case 's':
                printf("\n=== RELATÓRIO FINAL DO DETETIVE ===\n");
                printf("Total de pistas coletadas: %d\n", total_pistas);
                if (arvore_pistas != NULL) {
                    printf("\n=== PISTAS COLETADAS ===\n");
                    exibirPistas(arvore_pistas);
                }
                printf("\nSaindo da mansão...\n");
                return;               
            default:
                printf("Opção inválida! Use 'e', 'd', 'p' ou 's'.\n");
        }
    }
    printf("\n=== FIM DA EXPLORAÇÃO ===\n");
}

// --- FUNÇÃO PRINCIPAL ---

int main() {
    // Cria as salas da mansão
    Sala* hall = criarSala("Hall de Entrada", "Porta principal arrombada");
    Sala* sala_estar = criarSala("Sala de Estar", "Copo de vinho pela metade na mesa");
    Sala* quarto = criarSala("Quarto", NULL);
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto na estante");
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida do bloco de facas");
    Sala* jantar = criarSala("Sala de Jantar", NULL);
    Sala* banheiro = criarSala("Banheiro", "Mancha escura no tapete");
    Sala* jogos = criarSala("Sala de Jogos", NULL);
    Sala* jardim = criarSala("Jardim", "Pegadas de lama levando para a floresta");
    Sala* garagem = criarSala("Garagem", NULL);
    Sala* porao = criarSala("Porão Misterioso", "Baú com documentos secretos");
    Sala* escritorio = criarSala("Escritório", "Computador com e-mail não enviado");
    Sala* lavanderia = criarSala("Lavanderia", "Camisa com mancha de sangue");

    // Inicia a Arvore BST sem pistas
    Pistas* arvore_pistas = NULL; 

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
    escritorio->esquerda = porao;
    escritorio->direita = jardim;

    // Do Jardim: direita -> Garagem (apenas direita)
    jardim->direita = garagem;

    printf("=== MANSÃO DO MISTÉRIO ===\n");
    // Inicia a exploração a partir do Hall de Entrada (raiz da árvore)
    explorarSalasComPistas(hall);
    // Libera toda a memória alocada para as salas
    liberarSalas(hall);
    liberarPistas(arvore_pistas);

    printf("\n=== INVESTIGAÇÃO CONCLUÍDA ===\n");
    return 0;
}