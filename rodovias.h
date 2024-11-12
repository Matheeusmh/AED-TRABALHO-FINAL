#ifndef RODOVIAS_H
#define RODOVIAS_H

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

void to_lowercase(char *str);
descritorCidades *iniciarCidades();
descritorRodovias *iniciarRodovias();
int verificadorCidade(descritorCidades *cidades, char *cidade);
void adicionarCidade(descritorCidades *cidades, char *cidade, char *estado, float km);
descritorCidades *verificadorRodovia(descritorRodovias *descritor, char *rodovia);
void adicionarRodovia(descritorRodovias *descritor, char *rodovia, char *cidade, char *estado, float km);
void carregarDados(descritorRodovias *descritor);
int removerRodovia(descritorRodovias *descritor, char *rodovia);
int removerCidade(descritorRodovias *descritor, char *cidade);
void rota(char *cidade1, char *cidade2, descritorRodovias *city);
int cruzamento(char *rodovia1, char *rodovia2, char *cidade, descritorRodovias *city);
void imprimirRodoviasPorCidade(descritorRodovias *descritor);
void menu(descritorRodovias *rodovias);

#endif