#include <stdio.h>
#include <stdlib.h>
#include "rodovias.h"

int main(void) {
    descritorRodovias *rodovias;
    rodovias = iniciarRodovias();

    carregarDados(rodovias);

    menu(rodovias);

    return 0;
}
