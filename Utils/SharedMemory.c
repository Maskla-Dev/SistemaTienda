//
// Created by maskla on 26/11/22.
//

#include "SharedMemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"

MEMID getSharedChunk(char *name, size_t data_size, int id) {
    if (strlen(name) == 0)
        return UNDEFINED_MEM;
    key_t mem_key = getKey(name, id);
    return shmget(mem_key, data_size, IPC_CREAT | S_RWUSR);
}

MEMADRESS attachData(MEMID mem_id, bool is_read_only) {
    return shmat(mem_id, NULL, is_read_only ? SHM_RDONLY : 0);
}

MEMADRESS getData(char *name, bool is_read_only, size_t data_size) {
    MEMID mem_id = getSharedChunk(name, data_size, 'A');
    if (mem_id != UNDEFINED_MEM) {
        MEMADRESS address = attachData(mem_id, is_read_only);
        return (address == (MEMADRESS) UNDEFINED_MEM) ? NULL : address;
    }
    return NULL;
}

SHR_NUM_NATURAL *crearRegionNumeroNatural(char *id, bool is_read_only) {
    size_t chunk_size = sizeof(SHR_NUM_NATURAL);
    SHR_NUM_NATURAL *data = (SHR_NUM_NATURAL *) getData(id, is_read_only, chunk_size);
    if (data != NULL && !is_read_only)
        data->mem_id = getSharedChunk(id, chunk_size, 'A');
    return data;
}

void checkSharedMemory(void *data) {
    if (data == NULL) {
        fprintf(stderr,
                "Error getting data,remove sems and shared memory manually running "
                "[ipcs] and then [ipcrm -s]\nExit from program...\n");
        exit(EXIT_FAILURE);
    }
}

int destroySharedMemory(MEMID mem_id) {
    return shmctl(mem_id, IPC_RMID, NULL);
}