//
// Created by maskla on 9/01/23.
//

#include "Guardia.h"
#include "Utils/SemLogic.h"
#include "Utils/Utils.h"
#include "Utils/ImpresoraMensajes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool iniciarTurnoGuardia() {
    /**
     * Inicializa los semaforos de sincronizacion entre procesos
     * Recupera los datos de los usuarios
     * Espera conexion de un cliente para procesar su informacion
     */
    imprimirMensaje(GUARDIA_TAG, "Guardia reportandose a su puesto...\n");
    imprimirMensaje(GUARDIA_TAG, "");
    NODO_DATOS *usuarios_registrados;
    crearArchivo("files", ARCHIVO_USUARIOS_NOM);
    usuarios_registrados = recuperarLista();
    iniciarOperaciones(usuarios_registrados);
    return true;
}

void iniciarOperaciones(LISTA_USUARIOS *lista_usuarios) {
    PIPE *canal_clientes = getPipe(CANAL_MENSAJES_CLIENTE_ID);
    PIPE *canal_tienda = getPipe(CANAL_MENSAJES_TIENDA_ID);
    PeticionGuardia *peticion;
    while (true) {
        readMsg(canal_clientes->msg_id, &canal_clientes->mbuf, 1);
        peticion = descomponerPeticion(canal_clientes->mbuf.mText);
        procesarPeticion(peticion, lista_usuarios, canal_clientes);
    }
}

NODO_DATOS *recuperarLista() {
    /**
     * Abre el archivo para leer la lista de archivos
     * En caso de que no se pueda abrir retorna una lista nula y termina este proceso
     * Obtiene la informacion del archivo en una sola cadena de texto
     * Procesa la informacion de los usuarios y la almacena en una estructura
     * Regresa la lista de usuarios
     */
    NODO_DATOS *lista;
    NODO_CADENA_DATOS *primer_usuario;
    char *contenido_archivo;
    long tam_contenido;
    FILE *archivo = fopen(ARCHIVO_USUARIOS_NOM, "r");
    if (NULL == archivo) {
        imprimirError(GUARDIA_TAG, "No pudieron recuperarse los usuarios.");
        return NULL;
    }
    //Obtencion de datos del archivo en una sola cadena
    tam_contenido = obtenerTamArchivo(archivo) + 1;
    memset(contenido_archivo, '\0', tam_contenido);
    contenido_archivo = malloc(sizeof(char) * tam_contenido);
    fgets(contenido_archivo, (int) tam_contenido, archivo);
    //Contruccion de la estructura de datos de usuarios
    primer_usuario = separarUsuarios(contenido_archivo);
    lista = obtenerListaUsuarios(primer_usuario);
    return lista;
}

NODO_CADENA_DATOS *separarUsuarios(char *contenido) {
    /**
     * Recorre cada posicion de la cadena
     * Si encuentra un salto de linea separa la cadena
     *  Crea espacio para almacenar la cadena separada
     *  Separa la cadena al copiar la seccion de datos
     *  Crea espacio para otro nodo y cambia de nodo
     */
    NODO_CADENA_DATOS *primer_usuario = malloc(sizeof(NODO_CADENA_DATOS));
    NODO_CADENA_DATOS *usuario_actual = primer_usuario;
    for (size_t i = 0, pos = 0, tam; i < strlen(contenido); ++i) {
        if (contenido[i] == '\n') {
            tam = i - pos;
            usuario_actual->datos_usuario = malloc(sizeof(char) * tam);
            memset(usuario_actual->datos_usuario, '\0', tam);
            strncpy(usuario_actual->datos_usuario, (contenido + pos), tam - 1);
            pos = i + 1;
            usuario_actual->siguiente = malloc(sizeof(NODO_CADENA_DATOS));
            usuario_actual = usuario_actual->siguiente;
        }
    }
    usuario_actual->siguiente = NULL;
    free(contenido);
    return primer_usuario;
}

NODO_DATOS *obtenerListaUsuarios(NODO_CADENA_DATOS *usuario) {
    /**
     * Itera sobre los nodos donde se ubican las cadenas de los datos de usuario y las convierte en una estructura
     * Para cuando no hay mas nodos
     * Primero extrae los datos del usuario
     * Segundo se recorre el siguiente nodo
     * Si no hay mas nodos no crea el espacio para el siguiente nodo, de lo contrario crea espacio para otro nodo
     */
    NODO_DATOS *inicio_lista = malloc(sizeof(NODO_DATOS));
    NODO_DATOS *usuario_actual = inicio_lista;
    while (usuario->siguiente != NULL) {
        usuario_actual->usuario = (USUARIO *) malloc(sizeof(USUARIO));
        extraerDatosUsuario(usuario->datos_usuario, usuario_actual->usuario);
        usuario = usuario->siguiente;
        usuario_actual->siguiente = usuario == NULL ? NULL : malloc(sizeof(NODO_DATOS));
        usuario_actual = usuario_actual->siguiente;
    }
    return inicio_lista;
}

void extraerDatosUsuario(char *fuente, USUARIO *objetivo) {
    /**
     * Ubica la posicion del comodin (':' ASCII) que separa el usuario del password
     * Calcula el tamano que tiene el nombre del usuario des de la cadena
     * Crea el espacio donde se almacenan los datos del usuario
     * Copia parte de la cadena donde se ubica el nombre y el password hacia la estructura de datos del usuario
     */
    char *posicion = memchr(fuente, ':', strlen(fuente));
    size_t tamano_nombre = posicion - fuente;
    memset(objetivo->usuario, '\0', 25);
    memset(objetivo->password, '\0', 25);
    strncpy(objetivo->usuario, fuente, tamano_nombre - 1);
    strcpy(objetivo->password, fuente + tamano_nombre + 1);
}

char *buscarUsuario(char *nombre_usuario, NODO_DATOS *lista) {
    while (lista->siguiente != NULL) {
        if (strcmp(lista->usuario->usuario, nombre_usuario) == 0)
            return lista->usuario->password;
        lista = lista->siguiente;
    }
    return NULL;
}

PeticionGuardia *descomponerPeticion(char *mensaje_sin_procesar) {
    PeticionGuardia *peticion = (PeticionGuardia *) malloc(sizeof(PeticionGuardia));
    peticion->numero_usuario = strtoul(mensaje_sin_procesar, &mensaje_sin_procesar, 10);
    mensaje_sin_procesar = mensaje_sin_procesar + 1;
    peticion->operacion = *(mensaje_sin_procesar);
    mensaje_sin_procesar = mensaje_sin_procesar + 1;
    extraerDatosUsuario(mensaje_sin_procesar, &peticion->datos);
    return peticion;
}

void procesarPeticion(PeticionGuardia *peticion, LISTA_USUARIOS *lista, PIPE *canal_comunicacion) {
    char *password;
    switch (peticion->operacion) {
        case 'A':
            password = buscarUsuario(peticion->datos.usuario, lista);
            if (password != NULL)
                if (strcmp(password, peticion->datos.password) == 0) {
                    sendMsg(canal_comunicacion->msg_id, MSG_AUTENTIFICACION_CORRECTA,
                            (long) peticion->numero_usuario + 1);
                } else {
                    sendMsg(canal_comunicacion->msg_id, MSG_DATOS_INCORRECTOS, (long) peticion->numero_usuario + 1);
                }
            break;
        case 'R':
            if (buscarUsuario(peticion->datos.usuario, lista) == NULL) {
                agregarUsuarioLista(&peticion->datos, &lista);
                sendMsg(canal_comunicacion->msg_id, MSG_AUTENTIFICACION_CORRECTA,
                        (long) peticion->numero_usuario + 1);
            } else {
                sendMsg(canal_comunicacion->msg_id, MSG_DATOS_INCORRECTOS, (long) peticion->numero_usuario + 1);
            }
            break;
    }
}

void agregarUsuarioLista(USUARIO *usuario, LISTA_USUARIOS **lista_usuarios) {
    NODO_DATOS *nodo = (NODO_DATOS *) malloc(sizeof(NODO_DATOS));
    nodo->usuario = (USUARIO *) malloc(sizeof(USUARIO));
    strcpy(nodo->usuario->usuario, usuario->usuario);
    strcpy(nodo->usuario->password, usuario->password);
    nodo->siguiente = *lista_usuarios;
    *lista_usuarios = nodo;
}