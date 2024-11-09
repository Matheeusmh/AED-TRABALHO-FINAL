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