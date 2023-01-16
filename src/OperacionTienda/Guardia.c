//
// Created by maskla on 9/01/23.
//

#include "Guardia.h"
#include "Utils/Utils.h"
#include "Utils/ImpresoraMensajes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool iniciarTurnoGuardia(DatosSupervisor *datos) {
    /**
     * Inicializa los semaforos de sincronizacion entre procesos
     * Recupera los datos de los usuarios
     * Espera conexion de un cliente para procesar su informacion
     */
    limpiarArchivo();
    mensajeGuardia("Guardia reportandose a su puesto.");
    LISTA_USUARIOS *lista_usuarios = recuperarLista();
    mensajeGuardia("Lista de usuarios recueperada.");
    iniciarOperaciones(lista_usuarios, datos);
    return true;
}

void iniciarOperaciones(LISTA_USUARIOS *lista_usuarios, DatosSupervisor *datos) {
    PeticionGuardia *peticion;
    while (true) {
        imprimirMensaje(GUARDIA_TAG, "Esperando a que un cliente se forme y sea validado...\n");
        readMsg(datos->canal_clientes->msg_id, &datos->canal_clientes->mbuf, 1);
        peticion = descomponerPeticion(datos->canal_clientes->mbuf.mText);
        procesarPeticion(peticion, lista_usuarios, datos->canal_tienda, datos->canal_clientes);
    }
}

LISTA_USUARIOS *recuperarLista() {
    /**
     * Abre el archivo para leer la lista de archivos
     * En caso de que no se pueda abrir retorna una lista nula y termina este proceso
     * Obtiene la informacion del archivo en una sola cadena de texto
     * Lee linea por linea y
     * Regresa la lista de usuarios
     */
    mensajeGuardia("Recuperando lista de clientes.");
    LISTA_USUARIOS *lista;
    NODO_DATOS *usuario_actual;
    char nombre_archivo[123];
    sprintf(nombre_archivo, "files/%s", ARCHIVO_USUARIOS_NOM);
    crearArchivo("files", nombre_archivo);
    FILE *archivo = fopen(nombre_archivo, "r");
    char datos_usuario[2][60];
    bool es_primer_usuario = true;
    size_t pos = 0;
    char ch;
    size_t tipo_dato = 0;
    memset(datos_usuario[0], '\0', 60);
    memset(datos_usuario[1], '\0', 60);
    if (NULL == archivo) {
        errorGuardia("No se pudieron recuperar los usuarios.");
        return NULL;
    }
    do {
        ch = (char) fgetc(archivo);
        switch (ch) {
            case ':':
                tipo_dato = 1;
                pos = 0;
                break;
            case '\n':
                if (es_primer_usuario) {
                    usuario_actual = (NODO_DATOS *) malloc(sizeof(NODO_DATOS));
                    lista = usuario_actual;
                    es_primer_usuario = false;
                } else {
                    usuario_actual->siguiente = (NODO_DATOS *) malloc(sizeof(NODO_DATOS));
                    usuario_actual = usuario_actual->siguiente;
                }
                usuario_actual->usuario = (USUARIO *) malloc(sizeof(USUARIO));
                strcpy(usuario_actual->usuario->usuario, datos_usuario[0]);
                strcpy(usuario_actual->usuario->password, datos_usuario[1]);
                memset(datos_usuario[0], '\0', 60);
                memset(datos_usuario[1], '\0', 60);
                tipo_dato = 0;
                pos = 0;
                break;
            default:
                datos_usuario[tipo_dato][pos++] = ch;
                break;
        }
    } while (ch != EOF);
    if (usuario_actual != NULL)
        usuario_actual->siguiente = NULL;
    fclose(archivo);
    return lista;
}

void guardarLista(LISTA_USUARIOS *lista) {
    mensajeGuardia("Actualizando lista de clientes.");
    char nombre_archivo[123];
    char contenido[60];
    sprintf(nombre_archivo, "files/%s", ARCHIVO_USUARIOS_NOM);
    crearArchivo("files", nombre_archivo);
    FILE *archivo = fopen(nombre_archivo, "w");
    fprintf(archivo, "");
    fclose(archivo);
    archivo = fopen(nombre_archivo, "a");
    while (lista != NULL) {
        sprintf(contenido, "%s:%s\n", lista->usuario->usuario, lista->usuario->password);
        fprintf(archivo, "%s", contenido);
        memset(contenido, '\0', 60);
        lista = lista->siguiente;
    }
    fclose(archivo);
    mensajeGuardia("Lista actualizada.");
}

void extraerDatosUsuario(char *fuente, USUARIO *objetivo) {
    /**
     * Ubica la posicion del comodin (':' ASCII) que separa el usuario del password
     * Calcula el tamano que tiene el nombre del usuario des de la cadena
     * Crea el espacio donde se almacenan los datos del usuario
     * Copia parte de la cadena donde se ubica el nombre y el password hacia la estructura de datos del usuario
     */
    mensajeGuardia("Obteniendo identificaciones del cliente.");
    char *posicion = memchr(fuente, ':', strlen(fuente));
    size_t tamano_nombre = posicion - fuente;
    memset(objetivo->usuario, '\0', 25);
    memset(objetivo->password, '\0', 25);
    strncpy(objetivo->usuario, fuente, tamano_nombre);
    strcpy(objetivo->password, fuente + tamano_nombre + 1);
}

char *buscarUsuario(char *nombre_usuario, NODO_DATOS *lista) {
    char msg[256];
    sprintf(msg, "Buscando usuario %s...", nombre_usuario);
    mensajeGuardia(msg);
    while (lista != NULL) {
        if (strcmp(lista->usuario->usuario, nombre_usuario) == 0) {
            mensajeGuardia("Usuario encontrado.");
            return lista->usuario->password;
        }
        lista = lista->siguiente;
    }
    mensajeGuardia("No se encuentra usuario.");
    return NULL;
}

PeticionGuardia *descomponerPeticion(char *mensaje_sin_procesar) {
    imprimirMensaje(GUARDIA_TAG, "Procesando peticion...\n");
    PeticionGuardia *peticion = (PeticionGuardia *) malloc(sizeof(PeticionGuardia));
    peticion->numero_usuario = strtoul(mensaje_sin_procesar, &mensaje_sin_procesar, 10);
    mensaje_sin_procesar = mensaje_sin_procesar + 1;
    peticion->operacion = *(mensaje_sin_procesar);
    mensaje_sin_procesar = mensaje_sin_procesar + 2;
    extraerDatosUsuario(mensaje_sin_procesar, &peticion->datos);
    return peticion;
}

void procesarPeticion(PeticionGuardia *peticion, LISTA_USUARIOS *lista, PIPE *canal_tienda, PIPE *canal_clientes) {
    char *password = buscarUsuario(peticion->datos.usuario, lista);
    switch (peticion->operacion) {
        case 'A':
            if (password != NULL) {
                if (strcmp(password, peticion->datos.password) == 0) {
                    mensajeGuardia("El usuario se ha verificado correctamente");
                    sprintf(canal_tienda->mbuf.mText, "%lu", peticion->numero_usuario);
                    sendMsg(canal_tienda->msg_id, canal_tienda->mbuf.mText, obtenerCanal(supervisor));
                } else {
                    errorGuardia("Usuario con identificaciones no validadas");
                    sendMsg(canal_clientes->msg_id, MSG_DATOS_INCORRECTOS, (long) peticion->numero_usuario);
                }
            } else {
                errorGuardia("Usuario con identificaciones no validadas");
                sendMsg(canal_clientes->msg_id, MSG_DATOS_INCORRECTOS, (long) peticion->numero_usuario);
            }
            break;
        case 'R':
            if (password == NULL) {
                mensajeGuardia("El usuario se ha registrado correctamente");
                agregarUsuarioLista(&peticion->datos, lista);
                sprintf(canal_tienda->mbuf.mText, "%lu", peticion->numero_usuario);
                sendMsg(canal_tienda->msg_id, canal_tienda->mbuf.mText, obtenerCanal(supervisor));
                agregarUsuarioLista(&peticion->datos, lista);
                guardarLista(lista);
            } else {
                errorGuardia("Usuario con identificaciones no validadas");
                sendMsg(canal_clientes->msg_id, MSG_DATOS_INCORRECTOS, (long) peticion->numero_usuario);
            }
            break;
    }
    imprimirMensaje(GUARDIA_TAG, "Peticion procesada correctamente.\n");
}

void agregarUsuarioLista(USUARIO *usuario, LISTA_USUARIOS *lista_usuarios) {
    mensajeGuardia("Agregando usuario a la lista...");
    char *usuario_existe = buscarUsuario(usuario->usuario, lista_usuarios);
    if (usuario_existe == NULL) {
        while (lista_usuarios->siguiente != NULL)
            lista_usuarios = lista_usuarios->siguiente;
        lista_usuarios->siguiente = (NODO_DATOS *) malloc(sizeof(NODO_DATOS));
        lista_usuarios = lista_usuarios->siguiente;
        lista_usuarios->usuario = (USUARIO *) malloc(sizeof(USUARIO));
        strcpy(lista_usuarios->usuario->usuario, usuario->usuario);
        strcpy(lista_usuarios->usuario->password, usuario->password);
        lista_usuarios->siguiente = NULL;
    }
}

void mensajeGuardia(char *mensaje) {
    imprimirMensaje(GUARDIA_TAG, mensaje);
}

void errorGuardia(char *error) {
    imprimirError(GUARDIA_TAG, error);
}

void limpiarArchivo() {
    FILE *archivo = fopen("files/ListaUsuarios", "w");
    char *contenido = "JuanAlvarez:1234\nMarcoLuan:%412\nLuz1234:Federico\n";
    fprintf(archivo, "%s", contenido);
    fclose(archivo);
}