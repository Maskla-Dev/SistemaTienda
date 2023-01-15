//
// Created by maskla on 9/01/23.
//

#include "ImpresoraMensajes.h"
#include <stdio.h>

void imprimirMensaje(char *agent, char *message) {
    fprintf(stdout, EMPLOYEE_MESSAGE_FORMAT, agent, getpid(), message);
}

void imprimirError(char *agent, char *error_message) {
    fprintf(stderr, EMPLOYEE_MESSAGE_FORMAT, agent, getpid(), error_message);
}