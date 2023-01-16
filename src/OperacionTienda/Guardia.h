//
// Created by maskla on 9/01/23.
//

#ifndef SISTEMATIENDA_GUARDIA_H
#define SISTEMATIENDA_GUARDIA_H

#include <stdbool.h>
#include <stddef.h>
#include "Utils/SemLogic.h"
#include "Utils/Utils.h"
#include "Supervisor.h"

/**
 * 1 El guardia se encarga de validar las credenciales del cliente
 * 2 Verifica si el cliente se encuentra en la base de datos
 * 3 Registra si es un cliente nuevo
 * 4 Comunica al supervisor la llegada de un cliente
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

typedef struct PeticionGuardia {
    size_t numero_usuario;
    char operacion;
    USUARIO datos;
} PeticionGuardia;

//Desde una cadena de caracteres recupera la informacion del nombre de usuario y su password
void extraerDatosUsuario(char *, USUARIO *);

//De la cadena de texto obtienes los datos de una peticion
PeticionGuardia *descomponerPeticion(char *);

//Recupera los usuarios desde el archivo de usuarios
NODO_DATOS *recuperarLista();

//Obtiene un usuario de la lista de usuarios
char *buscarUsuario(char *, NODO_DATOS *);

//Agrega un usuario a la lista de usuarios
void agregarUsuarioLista(USUARIO *, LISTA_USUARIOS *);

//De la estructura de usuarios, la almacena en un archivo
void guardarLista(LISTA_USUARIOS *);

//Realiza actividades a partir de una peticion
void procesarPeticion(PeticionGuardia *peticion, LISTA_USUARIOS *, PIPE *, PIPE *);

//Inicia las operaciones correspondientes a la entidad guardia
bool iniciarTurnoGuardia(DatosSupervisor *datos);

//Inicia las funciones principales del guardia: recibir identificaciones y evaluarlas
void iniciarOperaciones(LISTA_USUARIOS *lista, DatosSupervisor *datos);

//Imprime un mensaje a nombre del guardia
void mensajeGuardia(char *);

//Imrpime un error a nombre del guardia
void errorGuardia(char *);

void limpiarArchivo();

#endif //SISTEMATIENDA_GUARDIA_H
