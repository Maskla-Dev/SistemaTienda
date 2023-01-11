//
// Created by maskla on 26/11/22.
//
#include <string.h>
#include <stdio.h>
#include "Utils.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <dirent.h>

void crearArchivo(char *ruta, char *nombre) {
    DIR *dir = opendir(ruta);
    DirStatus path_status = {0};
    int file;
    if (!dir)
        mkdir("files", 0700);
    else
        closedir(dir);
    if (access(nombre, F_OK) == -1) {
        file = open(nombre, O_CREAT);
        close(file);
    }
}

long obtenerTamArchivo(FILE *archivo) {
    long tam;
    fseek(archivo, 0L, SEEK_END);
    tam = ftell(archivo);
    rewind(archivo);
    return tam;
}

key_t getKey(char *name, int id) {
    char full_name[1024] = "";
    sprintf(full_name, "files/%s", name);
    crearArchivo("files", full_name);
    return ftok(full_name, id);
}

TIENDA *getServerData(bool is_read_only) {
    TIENDA *server_data = malloc(sizeof(TIENDA));
    server_data->semset = createSemset(TIENDA_SEMS_NOM, TIENDA_SEMS, 'A');
    server_data->seats = getSeatsInfo(is_read_only);
    server_data->customers = crearRegionConteoVendedores(is_read_only);
    return server_data;
}

size_t readCustomerCount(SEMAPHORE *semset, Customers *customers) {
    size_t value;
    semSentinel(downSem(semset, customer_num));
    value = customers->conteo;
    semSentinel(upSem(semset, customer_num));
    return value;
}

void incrementCustomerCount(SEMAPHORE *semset, Customers *customers, long value) {
    semSentinel(downSem(semset, customer_num));
    customers->conteo += value;
    semSentinel(upSem(semset, customer_num));
}

char *getConnectionName(size_t number) {
    char *name = malloc(sizeof(char) * strlen(BASE_CONNECTION_NAME) + 5);
    sprintf(name, "%s%lu", BASE_CONNECTION_NAME, number);
    return name;
}

CompradorVendedor *getDispatcherData(size_t customer_num) {
    CompradorVendedor *data = malloc(sizeof(CompradorVendedor));
    char *path_name = getConnectionName(customer_num);
    data->semset = getDispatcherSems(path_name);
    data->msgqueue = getPipe(path_name);
    fprintf(stdout, "[%d] semid=%d msgid=%d\n", getpid(), data->semset->sem_id, data->msgqueue->msg_id);
    free(path_name);
    return data;
}

SEMAPHORE *getDispatcherSems(char *path_name) {
    return createSemset(path_name, CUSTOMER_SEMS, 'A');
}

PIPE *getPipe(char *path_name) {
    PIPE *pipe;
    MSGID msg_id = getMsgid(path_name, 'A');
    if (msg_id == UNDEFINED_MSG)
        return NULL;
    pipe = malloc(sizeof(PIPE));
    pipe->msg_id = msg_id;
    return pipe;
}