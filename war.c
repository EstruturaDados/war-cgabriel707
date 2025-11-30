#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Definições e Constantes ---
#define QTD_TERRITORIOS 5
#define QTD_MISSOES 3

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- Protótipos das Funções (Modularização) ---
Territorio* inicializarMapa();
void atribuirMissao(char** missaoDestino);
void exibirMapa(const Territorio* mapa);
void realizarAtaque(Territorio* mapa);
int verificarVitoria(const Territorio* mapa, const char* missao);
void liberarMemoria(Territorio* mapa, char* missao);
void menuPrincipal();

// --- Função Principal ---
int main() {
    srand(time(NULL)); // Semente para aleatoriedade
    menuPrincipal();
    return 0;
}

// --- Implementação do Menu e Fluxo do Jogo ---
void menuPrincipal() {
    // 1. Inicialização (Automática conforme Nível Mestre)
    Territorio* mapa = inicializarMapa();
    char* missaoJogador = NULL;
    
    if (!mapa) {
        printf("Erro fatal de memoria.\n");
        return;
    }

    atribuirMissao(&missaoJogador);

    int opcao;
    int jogoAtivo = 1;

    printf("\n=== WAR ESTRUTURADO: NIVEL MESTRE ===\n");
    printf("Sua Missao: %s\n", missaoJogador);

    do {
        printf("\n--- MENU ---\n");
        printf("1. Atacar\n");
        printf("2. Verificar Missao / Status\n");
        printf("3. Exibir Mapa\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                realizarAtaque(mapa);
                // Verifica vitória automaticamente após ataque
                if (verificarVitoria(mapa, missaoJogador)) {
                    printf("\n\n🏆 PARABENS! Voce cumpriu sua missao: %s 🏆\n", missaoJogador);
                    jogoAtivo = 0; // Encerra o jogo
                }
                break;
            case 2:
                printf("Missao atual: %s\n", missaoJogador);
                if (verificarVitoria(mapa, missaoJogador)) {
                    printf("Status: CONCLUIDA! Voce venceu.\n");
                    jogoAtivo = 0;
                } else {
                    printf("Status: Em progresso...\n");
                }
                break;
            case 3:
                exibirMapa(mapa);
                break;
            case 0:
                printf("Saindo do jogo...\n");
                jogoAtivo = 0;
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (jogoAtivo);

    liberarMemoria(mapa, missaoJogador);
}

// --- Funções de Inicialização e Memória ---

Territorio* inicializarMapa() {
    // Uso de calloc para limpar a memória ao alocar
    Territorio* mapa = (Territorio*) calloc(QTD_TERRITORIOS, sizeof(Territorio));
    
    if (mapa != NULL) {
        // Dados iniciais fixos para facilitar testes (Nível Mestre)
        strcpy(mapa[0].nome, "Brasil");   strcpy(mapa[0].cor, "Verde");    mapa[0].tropas = 5;
        strcpy(mapa[1].nome, "Argentina");strcpy(mapa[1].cor, "Azul");     mapa[1].tropas = 3;
        strcpy(mapa[2].nome, "Mexico");   strcpy(mapa[2].cor, "Vermelho"); mapa[2].tropas = 2;
        strcpy(mapa[3].nome, "EUA");      strcpy(mapa[3].cor, "Azul");     mapa[3].tropas = 4;
        strcpy(mapa[4].nome, "China");    strcpy(mapa[4].cor, "Verde");    mapa[4].tropas = 6;
    }
    return mapa;
}

void atribuirMissao(char** missaoDestino) {
    const char* listaMissoes[QTD_MISSOES] = {
        "Destruir o exercito Azul",
        "Conquistar 3 territorios",
        "Eliminar o exercito Vermelho"
    };

    int sorteio = rand() % QTD_MISSOES;
    
    // Alocação dinâmica exata para a string
    *missaoDestino = (char*) malloc((strlen(listaMissoes[sorteio]) + 1) * sizeof(char));
    if (*missaoDestino) {
        strcpy(*missaoDestino, listaMissoes[sorteio]);
    }
}

void liberarMemoria(Territorio* mapa, char* missao) {
    if (mapa) free(mapa);
    if (missao) free(missao);
    printf("Memoria liberada. Jogo encerrado.\n");
}

// --- Lógica de Jogo (Batalha e Missões) ---

void exibirMapa(const Territorio* mapa) {
    printf("\n--- MAPA ATUAL ---\n");
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("[%d] %-10s | Cor: %-8s | Tropas: %d\n", 
               i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void realizarAtaque(Territorio* mapa) {
    int idAtk, idDef;
    exibirMapa(mapa);
    
    printf("\nDigite o ID do atacante (1-%d): ", QTD_TERRITORIOS);
    scanf("%d", &idAtk);
    printf("Digite o ID do defensor (1-%d): ", QTD_TERRITORIOS);
    scanf("%d", &idDef);

    // Ajuste de índice (usuário digita 1, array é 0)
    idAtk--; idDef--;

    // Validações básicas
    if (idAtk < 0 || idAtk >= QTD_TERRITORIOS || idDef < 0 || idDef >= QTD_TERRITORIOS) {
        printf("IDs invalidos!\n"); return;
    }
    if (mapa[idAtk].tropas <= 1) {
        printf("Atacante precisa de mais de 1 tropa!\n"); return;
    }
    if (strcmp(mapa[idAtk].cor, mapa[idDef].cor) == 0) {
        printf("Voce nao pode atacar seu proprio aliado!\n"); return;
    }

    printf("\n⚔️  Batalha: %s vs %s ⚔️\n", mapa[idAtk].nome, mapa[idDef].nome);
    
    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;
    printf("Dados -> Atacante: %d | Defensor: %d\n", dadoAtk, dadoDef);

    // Regra: Empates favorecem o atacante
    if (dadoAtk >= dadoDef) {
        printf("Resultado: Vitoria do ATACANTE!\n");
        mapa[idDef].tropas--;
        
        // Verifica conquista
        if (mapa[idDef].tropas == 0) {
            printf("🏳️  TERRITORIO CONQUISTADO! %s agora pertence ao exercito %s.\n", 
                   mapa[idDef].nome, mapa[idAtk].cor);
            
            strcpy(mapa[idDef].cor, mapa[idAtk].cor); // Troca a cor
            mapa[idDef].tropas = 1; // Ocupa com 1 tropa
            mapa[idAtk].tropas--;   // Remove 1 do atacante que moveu
        }
    } else {
        printf("Resultado: Vitoria da DEFESA!\n");
        mapa[idAtk].tropas--;
    }
}

int verificarVitoria(const Territorio* mapa, const char* missao) {
    // Vamos assumir que O JOGADOR É O EXÉRCITO "VERDE"
    char minhaCor[] = "Verde";

    if (strstr(missao, "Conquistar 3 territorios") != NULL) {
        int conta = 0;
        for (int i = 0; i < QTD_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, minhaCor) == 0) conta++;
        }
        return (conta >= 3);
    }
    
    if (strstr(missao, "Destruir o exercito Azul") != NULL) {
        for (int i = 0; i < QTD_TERRITORIOS; i++) {
            // Se ainda existir algum azul, não venceu
            if (strcmp(mapa[i].cor, "Azul") == 0) return 0;
        }
        return 1;
    }

    if (strstr(missao, "Eliminar o exercito Vermelho") != NULL) {
        for (int i = 0; i < QTD_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) return 0;
        }
        return 1;
    }

    return 0;
}
