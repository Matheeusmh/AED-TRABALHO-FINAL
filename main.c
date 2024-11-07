#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct noCidades {
    char cidade[20];
    char estado[2];
    float km;
    struct noCidades *prox;
    struct noCidades *ant;
} NoCidades;

typedef struct noRodovias{
    char rodovia[8];
    descritorCidades *cidades;
    struct noRodovias *prox;
} NoRodovias;

typedef struct {
    NoRodovias *inicio;
    NoRodovias *final;
    int quantRodovias;
} descritorRodovias;

typedef struct {
    NoCidades *inicio;
    NoCidades *final;
    int quantCidades;
} descritorCidades;

typedef NoCidades *novaCidade;
typedef NoRodovias *novaRodovia;

char *stringToUpr(char *stringOrig) {
    char *stringCpy;
    stringCpy = (char *)malloc((strlen(stringOrig) + 1) * sizeof(char));
    if(stringCpy == NULL) {
        printf("Erro ao alocar memoria!\n");
        return NULL;
    }

    strcpy(stringCpy, stringOrig);

    for(int i = 0; stringCpy[i] != '\0'; i++) {
        stringCpy[i] = toupper(stringCpy[i]);
    }

    return stringCpy;
}

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

int verificadorCidade(descritorCidades *cidades, char *cidade) {
    novaCidade aux = cidades->inicio;

    while(aux != NULL) {
        if(strcmp(stringToUpr(aux->cidade), stringToUpr(cidade)) == 0) return 1;
    }

    return 0;
}

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

        while(km >= aux->km && aux != NULL) {
            aux = aux->prox;
        }

        addCidade->prox = aux;
        aux->ant->prox = addCidade;
        addCidade->ant = aux->ant;
        aux->ant = addCidade;

        if(aux == NULL) {
            cidades->final = addCidade;
        }
        else if(aux->ant = NULL) {
            cidades->inicio = addCidade;
        }
    }

    cidades->quantCidades++;
}

descritorCidades *verificadorRodovia(descritorRodovias *descritor, char *rodovia) {
    novaRodovia aux = descritor->inicio;
    while(aux != NULL) {
        if(strcmp(stringToUpr(aux->rodovia), stringToUpr(rodovia)) == 0) aux->cidades;
    }

    return NULL;
}

void adicionarRodovia(descritorRodovias *descritor, char *rodovia, char *cidade, char *estado, float km) {
    if(verificadorRodovia(descritor, rodovia) != NULL) {
        adicionarCidade(verificadorRodovia(descritor, rodovia), cidade, estado, km);
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

        while(fscanf(ptrArquivo, "%8s", rodovia) != EOF) {
            fscanf(ptrArquivo, "%20s", cidade);
            fscanf(ptrArquivo, "%2s", estado);
            fscanf(ptrArquivo, "%f", km);

            adicionarRodovia(descritor, rodovia, cidade, estado, km);
        }

        fclose(ptrArquivo);
    }
}

