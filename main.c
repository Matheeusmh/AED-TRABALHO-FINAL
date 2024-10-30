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

void adicionarCidade(NoRodovias **ptrRodovia, char *rodovia, char *cidade, char *estado, float km) {
    novaCidade addCidade;
    addCidade = (NoCidades *)malloc(sizeof(NoCidades));
    if(addCidade == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    strcpy(addCidade->cidade, cidade);
    strcpy(addCidade->estado, estado);
    addCidade->km = km;

    novaCidade auxCidade = (*ptrRodovia)->cidades;

    while(auxCidade->km <= addCidade->km) {
        auxCidade = auxCidade->prox;
    }

    auxCidade->ant->prox = addCidade;
    addCidade->ant = auxCidade->ant;

    addCidade->prox = auxCidade;
    auxCidade->ant = addCidade;

}
void adicionarRodovia(descritorRodovias **descritor, char *rodovia, char *cidade, char *estado, float km) {
    if(*descritor == NULL) {
        iniciarDescritor(descritor, rodovia, cidade, estado, km);
    }

    novaRodovia auxRodovia = (*descritor)->inicio;

    while(auxRodovia != NULL) {
        if(strcmp(auxRodovia->rodovia, rodovia) == 0) {
            adicionarCidade(&auxRodovia, rodovia, cidade, estado, km);
            return;
        }
    }

    novaRodovia addRodovia;
    addRodovia = (NoRodovias *)malloc(sizeof(NoRodovias));
    if(addRodovia == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    (*descritor)->final->prox = addRodovia;
    (*descritor)->final = addRodovia;

    iniciarRodovia(descritor, rodovia, cidade, estado, km);

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

