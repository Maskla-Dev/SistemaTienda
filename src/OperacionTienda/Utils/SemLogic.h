//
// Created by maskla on 26/11/22.
//

#ifndef P2_P2_SEMLOGIC_H
#define P2_P2_SEMLOGIC_H

#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef UNDEFINED_SEM
#define UNDEFINED_SEM (-1)
#endif

#ifndef TIENDA_SEMS
#define TIENDA_SEMS 1
#endif

#ifndef SEM_PERMS
#define SEM_PERMS 0644
#endif

#define NUM_RECURSOS_COMPARTIDOS 2
#define RECURSOS_COMPARTIDOS_ID "RecursosCompartidos\0"

typedef enum RecursosCompartidos {
    conteo_clientes, inventario
} RecursosCompartidos;

typedef int SEMID;

// Performs semaphore control
typedef struct Semaphore {
    SEMID sem_id;
    size_t length;
} SEMAPHORE;

// Semun
typedef union {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
#if defined(_linux_)
    struct seminfo* __buf;
#endif
} SEMUN;

// Returns semaphore set id
SEMID getSemset(char *name, size_t length, int id);

// Returns SEMAPHORE pointer structure on success initialization, otherwise NULL
SEMAPHORE *createSemset(char *name, size_t length, int id);

// Set value to semaphore from SEMAPHORE
int setSemvalue(SEMAPHORE *semset, size_t sem_num, int value);

// Change sem value with semop
int doSemop(SEMAPHORE *semset, unsigned short sem, int value);

// Destroy semset
int destroySemset(SEMAPHORE *semset);

// Add 1 sem value
int upSem(SEMAPHORE *semset, unsigned short sem_num);

// Go to sleep if sem is equal to 0
int waitSem(SEMAPHORE *semset, unsigned short sem_num);

// Substract 1 sem value
int downSem(SEMAPHORE *semset, unsigned short sem_num);

// Check if semop is correctly dispatched, if it was not, exit from program
void semSentinel(int semop_return);

#endif //P2_P2_SEMLOGIC_H
