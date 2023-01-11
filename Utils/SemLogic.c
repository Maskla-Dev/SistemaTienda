//
// Created by maskla on 26/11/22.
//

#include "SemLogic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "Utils.h"

SEMID getSemset(char* name, size_t length, int id) {
    if (strlen(name) == 0 && length == 0)
        return UNDEFINED_SEM;
    key_t sem_key = getKey(name, id);
    return semget(sem_key, length, IPC_CREAT | SEM_PERMS);
}

SEMAPHORE* createSemset(char* name, size_t length, int id) {
    SEMID semset_id = getSemset(name, length, id);
    SEMAPHORE* semaphore;
    if (semset_id == UNDEFINED_SEM)
        return NULL;
    semaphore = (SEMAPHORE*)malloc(sizeof(SEMAPHORE));
    if (semaphore != NULL) {
        semaphore->sem_id = semset_id;
        semaphore->length = length;
    }
    return semaphore;
}

int setSemvalue(SEMAPHORE* semset, size_t sem_num, int value) {
    SEMUN arg;
    arg.val = value;
    if (sem_num >= semset->length) {
        return -1;
    }
    return semctl(semset->sem_id, sem_num, SETVAL, arg);
}

int doSemop(SEMAPHORE* semset, unsigned short sem_num, int value) {
    struct sembuf sem_op = {sem_num, value, 0};
    return semop(semset->sem_id, &sem_op, 1);
}

int destroySemset(SEMAPHORE* semset) {
    return semctl(semset->sem_id, 0, IPC_RMID);
}

int waitSem(SEMAPHORE* semset, unsigned short sem_num) {
    return doSemop(semset, sem_num, 0);
}

int upSem(SEMAPHORE* semset, unsigned short sem_num) {
    return doSemop(semset, sem_num, 1);
}

int downSem(SEMAPHORE* semset, unsigned short sem_num) {
    return doSemop(semset, sem_num, -1);
}

void semSentinel(int semop_return) {
    if (semop_return == -1) {
        fprintf(stderr,
                "[%d]: Sem malfunction, remove sems and shared memory manually running "
                "[ipcs] and then [ipcrm -s]\nExit from program...\n", getpid());
        exit(EXIT_FAILURE);
    }
}
