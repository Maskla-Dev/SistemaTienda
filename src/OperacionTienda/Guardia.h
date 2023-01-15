//
// Created by maskla on 9/01/23.
//

#ifndef SISTEMATIENDA_GUARDIA_H
#define SISTEMATIENDA_GUARDIA_H

#include <stdbool.h>
#include <stddef.h>
#include "Utils/SemLogic.h"
#include "Utils/Utils.h"

/**
 * El guardia se encarga de validar las credenciales del cliente
 * Verifica si el cliente se encuentra en la base de datos
 * Registra si es un cliente nuevo
 * Comunica al supervisor la llegada de un cliente
 *
 */

#define GUARDIA_TAG "Guardia\0"
#define ARCHIVO_USUARIOS_NOM "ListaUsuarios\0"

#define MSG_AUTENTIFICACION_CORRECTA "Autentificacion correcta\0"
#define MSG_REGISTRO_CORRECTO "Registro correcto\0"
#define MSG_DATOS_INCORRECTOS "Datos incorrectos\0"

typedef struct Usuario {
    char usuario[25];
    char password[25];
} USUARIO;

typedef struct nodo_datos {
    USUARIO *usuario;
    struct nodo_datos *siguiente;
} NODO_DATOS, LISTA_USUARIOS;

typedef struct CadenaUsuario {
    char *datos_usuario;
    struct CadenaUsuario *siguiente;
} NODO_CADENA_DATOS;

typedef struct PeticionGuardia {
    size_t numero_usuario;
    char operacion;
    USUARIO datos;
} PeticionGuardia;

//Inicia las operaciones correspondientes a la entidad guardia
bool iniciarTurnoGuardia();

//Inicia las funciones principales del guardia: recibir identificaciones y evaluarlas
void iniciarOperaciones(LISTA_USUARIOS *);

//Obtiene la directiva desde un mensaje


//Recupera los usuarios desde el archivo de usuarios
NODO_DATOS *recuperarLista();

//De la estructura de usuarios, la almacena en un archivo
void guardarLista(NODO_DATOS *);

//Agrega un usuario a la lista de usuarios
void agregarUsuarioLista(USUARIO *, LISTA_USUARIOS **);

//Obtiene un usuario de la lista de usuarios
char *buscarUsuario(char *, NODO_DATOS *);

//Divide una cadena de texto en la informacion de usuarios sin procesar
NODO_CADENA_DATOS *separarUsuarios(char *);

//Construye la lista de usuarios desde cadenas de caracteres sin procesar
NODO_DATOS *obtenerListaUsuarios(NODO_CADENA_DATOS *);

//Desde una cadena de caracteres recupera la informacion del nombre de usuario y su password
void extraerDatosUsuario(char *, USUARIO *);

void procesarPeticion(PeticionGuardia *peticion, LISTA_USUARIOS *, PIPE *);

PeticionGuardia *descomponerPeticion(char *);

#endif //SISTEMATIENDA_GUARDIA_H
