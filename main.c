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

void iniciarDescritor(descritorRodovias **descritor) {
    (*descritor) = (descritorRodovias *)malloc(sizeof(descritorRodovias));
    if(*descritor == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    (*descritor)->inicio = ?;
    (*descritor)->final = ?;
    (*descritor)->quantRodovias = 1;
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

