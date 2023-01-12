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

char *getConnectionName(size_t number) {
    char *name = malloc(sizeof(char) * strlen(BASE_CONNECTION_NAME) + 5);
    sprintf(name, "%s%lu", BASE_CONNECTION_NAME, number);
    return name;
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