#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definição da estrutura que representa uma cidade
typedef struct noCidades {
    char cidade[22]; // Nome da cidade
    char estado[3]; // Sigla do estado
    float km; // Distância em km
    struct noCidades *prox; // Ponteiro para a próxima cidade
    struct noCidades *ant; // Ponteiro para a cidade anterior
} NoCidades;

// Estrutura que descreve uma lista de cidades
typedef struct {
    NoCidades *inicio; // Ponteiro para o início da lista
    NoCidades *final; // Ponteiro para o final da lista
    int quantCidades; // Quantidade de cidades
} descritorCidades;

// Definição da estrutura que representa uma rodovia
typedef struct noRodovias {
    char rodovia[8]; // Nome da rodovia
    descritorCidades cidades; // Lista de cidades associadas à rodovia
    struct noRodovias *prox; // Ponteiro para a próxima rodovia
} NoRodovias;

// Estrutura que descreve uma lista de rodovias
typedef struct {
    NoRodovias *inicio; // Ponteiro para o início da lista
    NoRodovias *final; // Ponteiro para o final da lista
    int quantRodovias; // Quantidade de rodovias
} descritorRodovias;

// Tipos para facilitar o uso dos ponteiros de cidades e rodovias
typedef NoCidades *novaCidade;
typedef NoRodovias *novaRodovia;

// Função que converte uma string para letras minuscula
void to_lowercase(char *str) {
    if (str == NULL) return;
    int i = 0;
    while (str[i]) {

        if (isupper(str[i])) {
            str[i] = str[i] + 32;
        }
        i++;
    }
}
// Função que inicializa o descritor de cidades
descritorCidades *iniciarCidades() {
    descritorCidades *novo;
    novo = (descritorCidades *)malloc(sizeof(descritorCidades));
    if(novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return NULL;
    }

    novo->inicio = NULL;
    novo->final = NULL;
    novo->quantCidades = 0;

    return novo;
}

// Função que inicializa o descritor de rodovias
descritorRodovias *iniciarRodovias() {
    descritorRodovias *novo;
    novo = (descritorRodovias *)malloc(sizeof(descritorRodovias));
    if(novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return NULL;
    }

    novo->inicio = NULL;
    novo->final = NULL;
    novo->quantRodovias = 0;

    return novo;
}

// Função que verifica se uma cidade já existe na lista de cidades
int verificadorCidade(descritorCidades *cidades, char *cidade) {
    novaCidade aux = cidades->inicio;

    while(aux != NULL) {
        if(strcmp(aux->cidade, cidade) == 0) return 1;
        aux = aux->prox;
    }

    return 0;
}

// Função para adicionar uma cidade na lista, ordenando pela distância em km
void adicionarCidade(descritorCidades *cidades, char *cidade, char *estado, float km) {
    if (verificadorCidade(cidades, cidade)) {
        return; // Se a cidade já existir, não faz nada.
    }

    NoCidades *addCidade = (NoCidades *)malloc(sizeof(NoCidades));
    if (addCidade == NULL) {
        printf("Erro ao alocar memória!\n");
        return;
    }

    // Inicializa os valores do novo nó
    strcpy(addCidade->cidade, cidade);
    strcpy(addCidade->estado, estado);
    addCidade->km = km;
    addCidade->prox = NULL;
    addCidade->ant = NULL;

    if (cidades->inicio == NULL) {
        // Lista vazia: o novo nó se torna o primeiro e último
        cidades->inicio = addCidade;
        cidades->final = addCidade;
    } else {
        NoCidades *aux = cidades->inicio;

        // Encontrar a posição correta com base na quilometragem (ordem crescente)
        while (aux != NULL && km >= aux->km) {
            aux = aux->prox;
        }

        if (aux == NULL) {
            // Inserir no final
            addCidade->ant = cidades->final;
            cidades->final->prox = addCidade;
            cidades->final = addCidade;
        } else if (aux->ant == NULL) {
            // Inserir no início
            addCidade->prox = cidades->inicio;
            cidades->inicio->ant = addCidade;
            cidades->inicio = addCidade;
        } else {
            // Inserir no meio
            addCidade->prox = aux;
            addCidade->ant = aux->ant;
            aux->ant->prox = addCidade;
            aux->ant = addCidade;
        }
    }

    cidades->quantCidades++;
}

// Função que verifica se uma rodovia existe, retornando a lista de cidades associada
descritorCidades *verificadorRodovia(descritorRodovias *descritor, char *rodovia) {
    novaRodovia aux = descritor->inicio;
    while(aux != NULL) {
        if(strcmp(aux->rodovia, rodovia) == 0) {
            return &(aux->cidades);
        }
        aux = aux->prox;
    }

    return NULL;
}

// Função para adicionar uma rodovia e associar uma cidade a ela
void adicionarRodovia(descritorRodovias *descritor, char *rodovia, char *cidade, char *estado, float km) {
    to_lowercase(rodovia);
    to_lowercase(cidade);
    to_lowercase(estado);
    
    descritorCidades *verificaRodovia = verificadorRodovia(descritor, rodovia);
    if(verificaRodovia != NULL) {
        adicionarCidade(verificaRodovia, cidade, estado, km);
        return;
    }

    novaRodovia nova;
    nova = (NoRodovias *)malloc(sizeof(NoRodovias));
    if(nova == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    strcpy(nova->rodovia, rodovia);
    descritorCidades *tempCidades = iniciarCidades();
    if (tempCidades == NULL) {
        free(nova);
        return;
    }
    nova->cidades = *tempCidades;
    free(tempCidades);
    adicionarCidade(&(nova->cidades), cidade, estado, km);
    nova->prox = NULL;

    if(descritor->inicio == NULL) {
        descritor->inicio =  nova;
        descritor->final = nova;
    }
    else {
        descritor->final->prox = nova;
        descritor->final = nova;
    }

    descritor->quantRodovias++;
}

// Função para ler dados de um arquivo e preencher a lista de rodovias e cidades
void carregarDados(descritorRodovias *descritor) {
    FILE *ptrArquivo;
    char rodovia[9], cidade[22], estado[3], linha[100];
    float km;

    ptrArquivo = fopen("rodovias.txt", "r");
    if(ptrArquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    while (fgets(linha, sizeof(linha), ptrArquivo) != NULL) {
        sscanf(linha, "%21[^\n]", cidade); // Read city
        fgets(linha, sizeof(linha), ptrArquivo);
        sscanf(linha, "%2[^\n]", estado); // Read state
        fgets(linha, sizeof(linha), ptrArquivo);
        sscanf(linha, "%8[^\n]", rodovia); // Read highway
        fgets(linha, sizeof(linha), ptrArquivo);
        sscanf(linha, "%f", &km); // Read distance

        adicionarRodovia(descritor, rodovia, cidade, estado, km);
    }

    fclose(ptrArquivo);
}


int removerRodovia(descritorRodovias *descritor, char *rodovia){
    if (descritor == NULL || descritor->inicio == NULL){ // Verificação para ver se é válido o descritor de rodovias ou se a lista está vazia ou não.
        printf("\nErro! Não há rodovias!\n");
        return 0;
    }

    novaRodovia aux = descritor->inicio; // Ponteiro auxiliar que armazena a primeira rodovia e percorre a lista de rodovias.
    novaRodovia ant = NULL; // Ponteiro para armazenar o dado anterior a fim de melhor entendimento.

    // Percurso pela lista de rodovias para ver se a rodovia solicitada está ou não presente:
    while(aux != NULL && strcmp(aux->rodovia, rodovia) != 0){
        ant = aux; // Atualização de anterior.
        aux = aux->prox; // Avança para o próximo.
    }

    // Se a rodovia não for encontrada retorna 0 (erro).
    if(aux == NULL){
        printf("\nA rodovia não foi encontrada!\n");
        return 0;
    }
    // Se a rodovia for encontrada precisamos remover todas as cidades presentes nela!
    else{
        novaCidade cityAtual = aux->cidades.inicio; // Variável auxiliar para percorrer a lista de cidade da rodovia em questão.

        // Percurso para liberar a memória de cada cidade da rodovia:
        while(cityAtual != NULL){
            novaCidade cityRemove = cityAtual; // Variável para armazenar a cidade que será removida enquanto o loop for executado.
            cityAtual = cityAtual->prox; // Avança para a próxima cidade da lista.
            free(cityRemove); // Liberando a memória da cidade...
        }
    }

    // Atualização dos ponteiros para remover a rodovia da lista:

    // Se for a primeira:
    if(ant == NULL)
        descritor->inicio = aux->prox; // Atualiza o início...
    else
        ant->prox = aux->prox; // Ajusta o ponteiro anterior para apontar para o próximo.

    // Se for a última:
    if(descritor->final == aux)
        descritor->final = ant; // Atualiza o fim...

    free(aux); // Libera a memória da rodovia removida.
    descritor->quantRodovias--; // Decrementa a quantidade de rodovias.

    return 1;
}

int removerCidade(descritorRodovias *descritor, char *cidade){
    if (descritor == NULL || descritor->inicio == NULL){ // Verificação para ver se é válido o descritor de rodovias ou se a lista está vazia ou não.
        printf("\nErro! Não há rodovias!\n");
        return 0;
    }

    // Ponteiro auxiliar que armazena a primeira rodovia e percorre toda a lista de rodovias a fim de verificar se a cidade a ser removida está presente nela:
    novaRodovia aux = descritor->inicio;

    // Percorrendo a lista de rodovias:
    while (aux != NULL) {
        novaCidade cityAtual = aux->cidades.inicio; // Variável para armazenar a cidade atual durante o percurso.
        novaCidade cityAnt = NULL; // Variável para armazenar a cidade anterior.

        // Percorrendo a lista de cidades da rodovia atual:
        while (cityAtual != NULL) {
            // Se a cidade for encontrada:
            if (strcmp(cityAtual->cidade, cidade) == 0){
                // Verifica os possíveis casos:

                // Se for a primeira:
                if(cityAnt == NULL)
                    aux->cidades.inicio = cityAtual->prox; // Atualiza o início.
                else
                    cityAnt->prox = cityAtual->prox; // Se não, ajusta anterior para remoção da cidade.

                free(cityAtual); // Liberando a memória da cidade a ser removida...
                cityAtual = cityAtual->prox;
            }
            // Se a cidade não for encontrada: continua o percurso avançando para a próxima cidade até ser igual a NULL.
            else{
                cityAnt = cityAtual;
                cityAtual = cityAtual->prox;
            }
        }
        // Avança para a próxima rodovia:
        aux = aux->prox;
    }

    return 1;
}

void rota(char *cidade1, char *cidade2, descritorRodovias *city) {
    if (city == NULL) {
        printf("Descritor das rodovias não iniciado\n");
        return;
    }

    NoRodovias *marcadorR = city->inicio;
    NoCidades *marcadorC;
    NoCidades *inicio;
    to_lowercase(cidade1);
    to_lowercase(cidade2);

    for (int x = 0; x < city->quantRodovias; x++) {
        marcadorC = marcadorR->cidades.inicio;
        int cont = 0, verificador = 0;
        inicio = NULL;

        while (marcadorC != NULL) {
            if (strcmp(cidade1, marcadorC->cidade) == 0) {
                if (cont == 0) {
                    verificador = 1;
                }
                inicio = marcadorC;
                cont++;
            }

            if (strcmp(cidade2, marcadorC->cidade) == 0) {
                cont++;
            }
            marcadorC = marcadorC->prox;
        }

        if (cont == 2 && inicio != NULL) {
            printf("O percurso começa em %s, pela rodovia %s no km %.2f, passando por:\n",
                   cidade1, marcadorR->rodovia, inicio->km);

            marcadorC = inicio;
            while (marcadorC != NULL) {
                printf("Cidade: %s (%s), Km: %.2f\n", marcadorC->cidade, marcadorC->estado, marcadorC->km);
                if (strcmp(cidade2, marcadorC->cidade) == 0) {
                    break;
                }
                marcadorC = (verificador == 1) ? marcadorC->prox : marcadorC->ant;
            }
            return; // Caminho encontrado, saída da função
        }

        marcadorR = marcadorR->prox; // Move para a próxima rodovia
    }

    printf("Percurso não encontrado entre %s e %s.\n", cidade1, cidade2);
}


int cruzamento(char *rodovia1, char *rodovia2, char *cidade, descritorRodovias *city) {
    if (city == NULL) {
        printf("Descritor das rodovias não iniciado\n");
        return 0;
    }

    NoRodovias *marcadorR = city->inicio;
    NoCidades *marcadorC1 = NULL;
    NoCidades *marcadorC2 = NULL;

    while (marcadorR != NULL) {
        if (strcmp(marcadorR->rodovia, rodovia1) == 0) {
            marcadorC1 = marcadorR->cidades.inicio;
        }
        if (strcmp(marcadorR->rodovia, rodovia2) == 0) {
            marcadorC2 = marcadorR->cidades.inicio;
        }
        marcadorR = marcadorR->prox;
    }

    if (marcadorC1 == NULL || marcadorC2 == NULL) {
        printf("Uma ou ambas as rodovias não foram encontradas\n");
        return 0;
    }

    int verificador1 = 0;
    while (marcadorC1 != NULL) {
        if (strcmp(marcadorC1->cidade, cidade) == 0) {
            verificador1 = 1;
            break;
        }
        marcadorC1 = marcadorC1->prox;
    }

    int verificador2 = 0;
    while (marcadorC2 != NULL) {
        if (strcmp(marcadorC2->cidade, cidade) == 0) {
            verificador2 = 1;
            break;
        }
        marcadorC2 = marcadorC2->prox;
    }

    if (verificador1 && verificador2) {
        return 1;
    }
    return 0;
}

void imprimirRodoviasPorCidade(descritorRodovias *descritor) {
    NoRodovias *aux = descritor->inicio;

    while(aux != NULL) {
        NoCidades *aux2 = aux->cidades.inicio;
        printf("Rodovia: %s\n", aux->rodovia);
        while(aux2 != NULL) {
            printf("Cidade: %s\n", aux2->cidade);
            printf("Estado: %s\n", aux2->estado);
            printf("KM: %.2f\n\n", aux2->km);
            aux2 = aux2->prox;
        }
        aux = aux->prox;
    }
}

void menu(descritorRodovias *rodovias) {
    while (1) {
        int escolha;
        char nomeRodovia[10], nomeCidade[20];

        printf("\nEscolha a opção desejada:\n");
        printf("1 - Adicionar Rodovia\n");
        printf("2 - Remover Cidade\n");
        printf("3 - Remover Rodovia\n");
        printf("4 - Traçar Percurso\n");
        printf("5 - Cruzamento entre rodovias\n");
        printf("6 - Imprimir cidades\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &escolha);
        getchar(); // Limpa o buffer de nova linha deixado por scanf()

        switch (escolha) {
            case 1: {
                char estado[3]; // Ajuste para comportar string de 2 caracteres + '\0'
                float km;

                printf("\nInsira:\n");
                printf("Nome da Cidade: ");
                fgets(nomeCidade, sizeof(nomeCidade), stdin);
                nomeCidade[strcspn(nomeCidade, "\n")] = '\0'; // Remove nova linha
                printf("Estado (Sigla): ");
                scanf("%2s", estado); // Limita leitura a 2 caracteres
                getchar(); // Limpa o buffer de nova linha
                printf("Nome da Rodovia: ");
                scanf("%s", nomeRodovia);
                getchar(); // Limpa o buffer de nova linha
                printf("Quilometragem: ");
                scanf("%f", &km);
                getchar(); // Limpa o buffer de nova linha

                adicionarRodovia(rodovias, nomeRodovia, nomeCidade, estado, km);
                break;
            }
            case 2:
                printf("\nQual cidade você deseja remover?\n");
                fgets(nomeCidade, sizeof(nomeCidade), stdin);
                nomeCidade[strcspn(nomeCidade, "\n")] = '\0'; // Remove nova linha

                removerCidade(rodovias, nomeCidade);
                break;

            case 3:
                printf("\nQual rodovia você deseja remover?\n");
                scanf("%s", nomeRodovia);
                getchar(); // Limpa o buffer de nova linha

                removerRodovia(rodovias, nomeRodovia);
                break;

            case 4: {
                char cidade1[30], cidade2[30];
                printf("\nQual rota você deseja fazer?\n");
                printf("Nome da cidade em que você começará a rota: ");
                fgets(cidade1, sizeof(cidade1), stdin);
                cidade1[strcspn(cidade1, "\n")] = '\0'; // Remove nova linha
                printf("Nome da cidade em que você quer chegar: ");
                fgets(cidade2, sizeof(cidade2), stdin);
                cidade2[strcspn(cidade2, "\n")] = '\0'; // Remove nova linha

                rota(cidade1, cidade2, rodovias);
                break;
            }
            case 5: {
                char rodovia1[30], rodovia2[30], cidade[20];

                printf("\nDigite o nome da primeira rodovia: ");
                scanf("%s", rodovia1);
                getchar(); // Limpa o buffer de nova linha
                printf("Digite o nome da segunda rodovia: ");
                scanf("%s", rodovia2);
                getchar(); // Limpa o buffer de nova linha

                if (cruzamento(rodovia1, rodovia2, cidade, rodovias) == 1) {
                    printf("\nAs rodovias se cruzam na cidade %s\n", cidade);
                } else {
                    printf("\nAs rodovias não se cruzam.\n");
                }
                break;
            }
            case 6:
                imprimirRodoviasPorCidade(rodovias);
                break;
            case 0:
                printf("\nFinalizando...\n");
                return;

            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    }
}


int main(void) {
    descritorRodovias *rodovias;
    rodovias = iniciarRodovias();

    carregarDados(rodovias);

    imprimirRodoviasPorCidade(rodovias);

    menu(rodovias);

    return 0;
}
