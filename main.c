#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura que representa uma cidade
typedef struct noCidades {
    char cidade[20]; // Nome da cidade
    char estado[2]; // Sigla do estado
    float km; // Distância em km
    struct noCidades *prox; // Ponteiro para a próxima cidade
    struct noCidades *ant; // Ponteiro para a cidade anterior
} NoCidades;

// Definição da estrutura que representa uma rodovia
typedef struct noRodovias {
    char rodovia[8]; // Nome da rodovia
    descritorCidades *cidades; // Lista de cidades associadas à rodovia
    struct noRodovias *prox; // Ponteiro para a próxima rodovia
} NoRodovias;

// Estrutura que descreve uma lista de rodovias
typedef struct {
    NoRodovias *inicio; // Ponteiro para o início da lista
    NoRodovias *final; // Ponteiro para o final da lista
    int quantRodovias; // Quantidade de rodovias
} descritorRodovias;

// Estrutura que descreve uma lista de cidades
typedef struct {
    NoCidades *inicio; // Ponteiro para o início da lista
    NoCidades *final; // Ponteiro para o final da lista
    int quantCidades; // Quantidade de cidades
} descritorCidades;

// Tipos para facilitar o uso dos ponteiros de cidades e rodovias
typedef NoCidades *novaCidade;
typedef NoRodovias *novaRodovia;

// Função que converte uma string para letras maiúsculas
char *stringToUpr(char *stringOrig) {
    char *stringCpy;
    stringCpy = (char *)malloc((strlen(stringOrig) + 1) * sizeof(char));
    if(stringCpy == NULL) {
        printf("Erro ao alocar memoria!\n");
        return NULL;
    }

    strcpy(stringCpy, stringOrig);

    // Converte cada caractere para maiúscula
    for(int i = 0; stringCpy[i] != '\0'; i++) {
        stringCpy[i] = toupper(stringCpy[i]);
    }

    return stringCpy;
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
        if(strcmp(stringToUpr(aux->cidade), stringToUpr(cidade)) == 0) return 1;
        aux = aux->prox;
    }

    return 0;
}

// Função para adicionar uma cidade na lista, ordenando pela distância em km
void adicionarCidade(descritorCidades *cidades, char *cidade, char *estado, float km) {
    if(verificadorCidade(cidades, cidade)) {
        return;
    }
    novaCidade addCidade;
    addCidade = (NoCidades *)malloc(sizeof(NoCidades));
    if(addCidade == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    strcpy(addCidade->cidade, cidade);
    strcpy(addCidade->estado, estado);
    addCidade->km = km;
    addCidade->prox = NULL;
    addCidade->ant = NULL;

    if(cidades->inicio == NULL) {
        cidades->inicio = addCidade;
        cidades->final = addCidade;
    }
    else {
        novaCidade aux = cidades->inicio;

        while(aux != NULL && km >= aux->km) {
            aux = aux->prox;
        }

        addCidade->prox = aux;
        if (aux->ant != NULL) {
            aux->ant->prox = addCidade;
        }

        addCidade->ant = aux->ant;
        aux->ant = addCidade;

        if(aux == NULL) {
            cidades->final = addCidade;
        }
        else if(aux->ant == NULL) {
            cidades->inicio = addCidade;
        }
    }

    cidades->quantCidades++;
}

// Função que verifica se uma rodovia existe, retornando a lista de cidades associada
descritorCidades *verificadorRodovia(descritorRodovias *descritor, char *rodovia) {
    novaRodovia aux = descritor->inicio;
    while(aux != NULL) {
        if(strcmp(stringToUpr(aux->rodovia), stringToUpr(rodovia)) == 0) {
            return aux->cidades;
        }
        aux = aux->prox;
    }

    return NULL;
}

// Função para adicionar uma rodovia e associar uma cidade a ela
void adicionarRodovia(descritorRodovias *descritor, char *rodovia, char *cidade, char *estado, float km) {
    descritorCidades *verificaRodovia = verificadorRodovia(descritor, rodovia);
    if(verificaRodovia != NULL) {
        adicionarCidade(verificaRodovia, cidade, estado, km);
        return;
    }

    novaRodovia aux = descritor->inicio;
    while(aux != NULL) {

    }
    novaRodovia nova;
    nova = (NoRodovias *)malloc(sizeof(NoRodovias));
    if(nova == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    strcpy(nova->rodovia, rodovia);
    nova->cidades = iniciarCidades();
    adicionarCidade(nova->cidades, cidade, estado, km);
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
void lerArquivo(descritorRodovias **descritor) {
    FILE *ptrArquivo;
    char rodovia[8], cidade[20], estado[2];
    float km;

    ptrArquivo = fopen("?", "r");
    if(ptrArquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    else {

        while(fscanf(ptrArquivo, "%20s", cidade) != EOF) {
            fscanf(ptrArquivo, "%2s", estado);
            fscanf(ptrArquivo, "%8s", rodovia);
            fscanf(ptrArquivo, "%f", &km);

            adicionarRodovia(descritor, rodovia, cidade, estado, km);
        }

        fclose(ptrArquivo);
    }
}

int removerRodovia(descritorRodovias *descritor, char *rodovia){
    if (descritor == NULL || descritor->inicio == NULL){ // Verificação para ver se é válido o descritor de rodovias ou se a lista está vazia ou não.
        printf("\nErro! Não há rodovias!\n");
        return 0;
    }

    novaRodovia *aux = descritor->inicio; // Ponteiro auxiliar que armazena a primeira rodovia e percorre a lista de rodovias.
    novaRodovia *ant = NULL; // Ponteiro para armazenar o dado anterior a fim de melhor entendimento.

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
        novaCidade *cityAtual = aux->cidades; // Variável auxiliar para percorrer a lista de cidade da rodovia em questão.

        // Percurso para liberar a memória de cada cidade da rodovia:
        while(cityAtual != NULL){
            novaCidade *cityRemove = cityAtual; // Variável para armazenar a cidade que será removida enquanto o loop for executado.
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
    novaRodovia *aux = descritor->inicio;

    // Percorrendo a lista de rodovias:
    while (aux != NULL) {
        novaCidade *cityAtual = aux->cidades->inicio; // Variável para armazenar a cidade atual durante o percurso.
        novaCidade *cityAnt = NULL; // Variável para armazenar a cidade anterior.

        // Percorrendo a lista de cidades da rodovia atual:
        while (cityAtual != NULL) {
            // Se a cidade for encontrada:
            if (strcmp(cityAtual->cidade, cidade) == 0){
                // Verifica os possíveis casos:

                // Se for a primeira:
                if(cityAnt == NULL)
                    aux->cidades->inicio = cityAtual->prox; // Atualiza o início.
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

void to_lowercase(char *str) {
    if (str == NULL) return;
    int i = 0;
    while (str[i]) {

        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
        i++;
    }
}


void rota(char *cidade1, char *cidade2, descritorRodovias *city){
    if(city == NULL){
        printf("Descritor das rodovias não iniciado\n");
        return;
    }

    int cont;
    NoRodovias *marcadorR = city->inicio;
    NoCidades *marcadorC;
    NoCidades *inicio;
    int verificador;
    to_lowercase(cidade1);
    to_lowercase(cidade2);

    for(int x = 0; x < city->quantRodovias; x++){
        marcadorC = marcadorR->cidades;
        cont = 0;
        verificador = 0;
        while( marcadorC != NULL ){
            if (strcmp(cidade1, marcadorC->cidade) == 0){
                if(cont == 0){
                    verificador = 1;
                }
                inicio = marcadorC;
                cont++;
            }

            if (strcmp(cidade2, marcadorC->cidade) == 0){
                cont++;
            }
            marcadorC = marcadorC->prox;
        }

        if( cont == 2){
            if (verificador == 1) {
                printf("O percurso começa em %s, pela rodovia %s no km %.2f, passando por:\n",
                       cidade1, marcadorR->rodovia, inicio->km);

                marcadorC = inicio;
                while (marcadorC != NULL) {
                    printf("Cidade: %s (%s), Km: %.2f\n", marcadorC->cidade, marcadorC->estado, marcadorC->km);
                    if (strcmp(cidade2, marcadorC->cidade) == 0) break;
                    marcadorC = marcadorC->prox;
                }
            }
            else {
                printf("O percurso começa em %s, pela rodovia %s no km %.2f, passando por:\n",
                       cidade1, marcadorR->rodovia, inicio->km);

                marcadorC = inicio;
                while (marcadorC != NULL) {
                    printf("Cidade: %s (%s), Km: %.2f\n", marcadorC->cidade, marcadorC->estado, marcadorC->km);
                    if (strcmp(cidade2, marcadorC->cidade) == 0){
                        break;
                    }
                    marcadorC = marcadorC->ant;
                }
            }
            return;
            }
        }

        marcadorR = marcadorR->prox;
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
            marcadorC1 = marcadorR->cidades;
        }
        if (strcmp(marcadorR->rodovia, rodovia2) == 0) {
            marcadorC2 = marcadorR->cidades;
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