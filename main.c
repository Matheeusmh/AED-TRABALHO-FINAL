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
    NoCidades *cidades;
    struct noRodovias *prox;
} NoRodovias;

typedef struct {
    NoRodovias *inicio;
    NoRodovias *final;
    int quantRodovias;
} descritorRodovias;

typedef NoCidades *novaCidade;
typedef NoRodovias *novaRodovia;

void iniciarRodovia(descritorRodovias **descritor, char *rodovia, char *cidade, char *estado, float km) {
    novaRodovia auxRodovia = (*descritor)->inicio;
    auxRodovia = (NoRodovias *)malloc(sizeof(NoRodovias));
    if(auxRodovia == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    strcpy(auxRodovia->rodovia, rodovia);
    
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

    auxRodovia->cidades = cidade;
    auxRodovia->prox = NULL;
}

void iniciarDescritor(descritorRodovias **descritor, char *rodovia, char *cidade, char *estado, float km) {
    (*descritor) = (descritorRodovias *)malloc(sizeof(descritorRodovias));
    if(*descritor == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    novaRodovia addRodovia = (NoRodovias *)malloc(sizeof(NoRodovias));
    if(addRodovia == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    iniciarRodovia(descritor, rodovia, cidade, estado, km);

    (*descritor)->inicio = addRodovia;
    (*descritor)->final = addRodovia;
    (*descritor)->quantRodovias = 1;
}

void adicionarRodovia(descritorRodovias **descritor, char *rodovia, char *cidade, char *estado, float km) {
    if(*descritor == NULL) {
        iniciarDescritor(descritor, rodovia, cidade, estado, km);
    }
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

            inserirRodovia(descritor, rodovia, cidade, estado, km);
        }

        fclose(ptrArquivo);
    }
}

