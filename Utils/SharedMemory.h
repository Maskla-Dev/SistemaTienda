//
// Created by maskla on 26/11/22.
//

#ifndef P2_P2_SHAREDMEMORY_H
#define P2_P2_SHAREDMEMORY_H

#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define TOTAL_ROWS 7
#define TOTAL_COLUMNS 8
#define S_RWUSR 0600
#define AIRLINE_DATA_NAME "Airlineshared\0"
#define UNDEFINED_MEM (-1)

#define REGION_CONTEO_CLIENTES_ID "ConteoClientes\0"

typedef int MEMID;
typedef void *MEMADRESS;

typedef struct {
    MEMID mem_id;
    size_t numero;
} SHR_NUM_NATURAL;

MEMID getSharedChunk(char *name, size_t data_size, int id);

MEMADRESS attachData(MEMID mem_id, bool is_read_only);

MEMADRESS getData(char *name, bool is_read_only, size_t data_size);

SHR_NUM_NATURAL *crearRegionNumeroNatural(char *id, bool is_read_only);

void checkSharedMemory(void *data);

int destroySharedMemory(MEMID mem_id);

#endif //P2_P2_SHAREDMEMORY_H
