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

// Estrutura da tabela hash
// Tabela hash para armazenar pistas e suspeitos
#define TAMANHO_HASH 10

// Estrutura do nó da tabela hash
typedef struct Suspeitos {
    char pista[100];
    char suspeito[50];
    struct Suspeitos* proximo;
} Suspeitos;

// Estrutura da tabela hash
typedef struct {
    Suspeitos* tabela[TAMANHO_HASH];
} TabelaHash;

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

// --- FUNÇÕES DA TABELA HASH ---

// Função hash simples baseada na primeira letra da pista
int funcaoHash(char* pista) {
    if (strlen(pista) == 0) return 0;
    return (pista[0] - 'A') % TAMANHO_HASH;
}

// Função para inicializar tabela hash
void inicializarHash(TabelaHash* hash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        hash->tabela[i] = NULL;
    }
}

// Função para inserir associação pista->suspeito na tabela hash
void inserirNaHash(TabelaHash* hash, char* pista, char* suspeito) {
    int indice = funcaoHash(pista);
    
    // Criar novo nó
    Suspeitos* novo = (Suspeitos*)malloc(sizeof(Suspeitos));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = NULL;
    
    // Inserir no início da lista encadeada
    if (hash->tabela[indice] == NULL) {
        hash->tabela[indice] = novo;
    } else {
        // Verificar se a pista já existe (não duplicar)
        Suspeitos* atual = hash->tabela[indice];
        while (atual != NULL) {
            if (strcmp(atual->pista, pista) == 0) {
                // Pista já existe, atualizar suspeito
                strcpy(atual->suspeito, suspeito);
                free(novo);
                return;
            }
            if (atual->proximo == NULL) break;
            atual = atual->proximo;
        }
        // Inserir no final
        atual->proximo = novo;
    }
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

// Função para buscar suspeito associado a uma pista
char* encontrarSuspeito(TabelaHash* hash, char* pista) {
    int indice = funcaoHash(pista);
    Suspeitos* atual = hash->tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL; // Pista não encontrada
}

// Função para contar quantas pistas um suspeito tem
int contarPistasDoSuspeito(TabelaHash* hash, Pistas* arvore, char* suspeito) {
    if (arvore == NULL) return 0;
    
    int count = 0;
    // Percorre a árvore de pistas em ordem
    count += contarPistasDoSuspeito(hash, arvore->esquerda, suspeito);
    
    // Verifica se esta pista aponta para o suspeito
    char* suspeito_da_pista = encontrarSuspeito(hash, arvore->pista);
    if (suspeito_da_pista != NULL && strcmp(suspeito_da_pista, suspeito) == 0) {
        count++;
    }
    
    count += contarPistasDoSuspeito(hash, arvore->direita, suspeito);
    return count;
}

// Função para exibir a árvore de pistas 
void exibirPistas(Pistas* raiz, TabelaHash* hash) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda, hash);

        // Buscar suspeito associado a esta pista
        char* suspeito = encontrarSuspeito(hash, raiz->pista); 

        // Verifica se o suspeito foi encontrado
        if (suspeito != NULL) {
            printf("  • %s - Local: %s - Suspeito: %s\n", raiz->pista, raiz->localizacao, suspeito);
        } else {
            printf("  • %s - Local: %s - Suspeito: (Não associado)\n", raiz->pista, raiz->localizacao);
        }
        exibirPistas(raiz->direita, hash);
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

// --- FUNÇÃO DO JULGAMENTO FINAL ---

// Verificar se o suspeito acusado tem pelo menos 2 pistas
void verificarSuspeitoFinal(TabelaHash* hash, Pistas* arvore_pistas) {
    char suspeito[50];
    int pistas_do_suspeito = 0;
    int resultado;
    
    printf("\n=== FASE DE JULGAMENTO FINAL ===\n");
    printf("\nCom base nas pistas coletadas, quem é o culpado?\n");
    printf("  • Gabriel - O mordomo da mansão\n");
    printf("  • Amanda - A herdeira da família\n"); 
    printf("  • Dr. Lopes - Médico da família\n");
    printf("  • Mario - Jardineiro da mansão\n");
    printf("Digite o nome do suspeito que você acusa: ");
    do {
    resultado = scanf(" %[^\n]", suspeito);
    if (resultado != 1) {
        printf("Entrada inválida. Digite o nome do suspeito: ");
        while (getchar() != '\n'); // Limpar buffer
    }
    } while (resultado != 1);
    
    // Contar quantas pistas apontam para este suspeito
    pistas_do_suspeito = contarPistasDoSuspeito(hash, arvore_pistas, suspeito);
    
    printf("\n=== VERDADE REVELADA ===\n");
    if (pistas_do_suspeito >= 2) {
        printf("ACUSAÇÃO CORRETA! %s é o culpado!\n", suspeito);
        printf("%d pistas incriminadoras encontradas.\n", pistas_do_suspeito);
    } else {
        printf("ACUSAÇÃO INCORRETA! %s não é o culpado.\n", suspeito);
        printf("Apenas %d pista(s) incriminadora(s).\n", pistas_do_suspeito);
        printf("É necessário pelo menos 2 pistas para condenar.\n");
    }
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

// Função para liberar a memória da tabela hash
void liberarHash(TabelaHash* hash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        Suspeitos* atual = hash->tabela[i];
        while (atual != NULL) {
            Suspeitos* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        hash->tabela[i] = NULL;
    }
}

// --- FUNÇÃO DO MENU DE EXPLORACAO ---

// função para explorar a arvore da mansão interativamente
void explorarSalasComPistas(Sala* raiz, TabelaHash* hash) {
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
        printf("  (s) - Sair da exploração e entrar no julgamento final\n");
        
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
                    exibirPistas(arvore_pistas, hash);
                }
                break;
                
            case 's':
                printf("\n=== RELATÓRIO FINAL DO DETETIVE ===\n");
                printf("Total de pistas coletadas: %d\n", total_pistas);
                if (arvore_pistas != NULL) {
                    printf("\n=== PISTAS COLETADAS ===\n");
                    exibirPistas(arvore_pistas, hash);
                }
                verificarSuspeitoFinal(hash, arvore_pistas);
                liberarPistas(arvore_pistas);
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

    // Inicializar tabela hash com associações pista->suspeito
    TabelaHash tabelaSuspeitos;
    inicializarHash(&tabelaSuspeitos);

    inserirNaHash(&tabelaSuspeitos, "Porta principal arrombada", "Gabriel");
    inserirNaHash(&tabelaSuspeitos, "Copo de vinho pela metade na mesa", "Amanda");
    inserirNaHash(&tabelaSuspeitos, "Livro sobre venenos aberto na estante", "Dr. Lopes");
    inserirNaHash(&tabelaSuspeitos, "Faca desaparecida do bloco de facas", "Mario");
    inserirNaHash(&tabelaSuspeitos, "Mancha escura no tapete", "Gabriel");
    inserirNaHash(&tabelaSuspeitos, "Pegadas de lama levando para a floresta", "Amanda");
    inserirNaHash(&tabelaSuspeitos, "Baú com documentos secretos", "Mario");
    inserirNaHash(&tabelaSuspeitos, "Computador com e-mail não enviado", "Dr. Lopes");
    inserirNaHash(&tabelaSuspeitos, "Camisa com mancha de sangue", "Gabriel");

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
    explorarSalasComPistas(hall, &tabelaSuspeitos);
    // Libera toda a memória alocada para as salas
    liberarHash(&tabelaSuspeitos);
    liberarSalas(hall);
    printf("\n=== INVESTIGAÇÃO CONCLUÍDA ===\n");
    return 0;
}