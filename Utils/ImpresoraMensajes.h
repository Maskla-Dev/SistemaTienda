//
// Created by maskla on 9/01/23.
//

#ifndef SISTEMATIENDA_IMPRESORAMENSAJES_H
#define SISTEMATIENDA_IMPRESORAMENSAJES_H

#include <unistd.h>
#include <stdbool.h>

#define EMPLOYEE_MESSAGE_FORMAT "[%s | ID %d] : %s"

//Imprime un mensaje en la salida estandar del sistema
void imprimirMensaje(char *, char *);

//
void imprimirError(char *, char *);

#endif //SISTEMATIENDA_IMPRESORAMENSAJES_H
