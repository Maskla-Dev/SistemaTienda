//
// Created by maskla on 9/01/23.
//

#include "Vendedor.h"
#include <stdio.h>
#include <string.h>

void iniciarTurnoVendedor(size_t numero_vendedor) {
    char SELLER_NAME[SELLERNAME_LENGTH];
    memset(SELLER_NAME, '\0', SELLERNAME_LENGTH);
    sprintf(SELLER_NAME, "%s %lu", SELLER_TAG, numero_vendedor);
}