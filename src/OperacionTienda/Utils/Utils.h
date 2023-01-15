//
// Created by maskla on 26/11/22.
//

#ifndef P2_UTILS_H
#define P2_UTILS_H

#include <stdlib.h>
#include "Messages.h"
#include"SemLogic.h"
#include "SharedMemory.h"
#include <stdio.h>

#define BASE_CONNECTION_NAME "Costumer"

typedef struct Pipe {
    MSGID msg_id;
    MBUF mbuf;
} PIPE;

typedef struct CustomerDispatcher {
    SEMAPHORE *semset;
    PIPE *msgqueue;
} CompradorVendedor;

typedef struct stat DirStatus;

long obtenerCanal(size_t entidad);

//Crea un archivo, asegura que la ruta exista, de no ser asi la crear,
//de igual forma verifica si el archivo existe, de no ser asi lo crea.
void crearArchivo(char *, char *);

//Devuelve el tamano del archivo, es necesario que el archivo este abierto, regresa la posicion de lectura al inicio
long obtenerTamArchivo(FILE *);

//Return key for semget, shmget and msgget
key_t getKey(char *name, int id);


//Returns string reference for customer-dispatcher name
char *getConnectionName(size_t number);

//Returns dispatcher data for customer: semaphores and msg queue
CompradorVendedor *getDispatcherData(size_t customer_num);

//Returns dispatcher semaphores
SEMAPHORE *getDispatcherSems(char *name);

//Returns dispatcher pipe
PIPE *getPipe(char *name);

#endif //P2_UTILS_H
