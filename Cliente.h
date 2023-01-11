//
// Created by maskla on 10/01/23.
//

#ifndef SISTEMATIENDA_CLIENTE_H
#define SISTEMATIENDA_CLIENTE_H

#include <stdlib.h>
#include <stdbool.h>
#include "Utils/Utils.h"
#include "Utils/Messages.h"

typedef struct Cliente {
    size_t numero_cliente;
    PIPE *canal_clientes;
} CLIENTE;

//Imprime un caracter tantas veces se especifique el variable tam en pantalla
void imprimirCaracter(char, size_t);

//Proceso de autentificacion
void iniciarEtapaAutenticacion(size_t, PIPE *);

//Verifica la opcion elegida en proceo de autentificacion
bool validarOpcionAutentificacion(size_t, PIPE *, long);

//Inicia el proceso de compra y sus variantes
void iniciarProcesoCompra();

//Autentificacion
bool autentificarse(size_t, PIPE *);

//Registrarse
bool registrarse(size_t, PIPE *);

//Solicita datos de identificacion, envia los datos con el siguiente codigo para la operacion
// A - Autentificarse, el guardia tendra que buscar en la lista de clientes al cliente
// R - Registro, el guardia tendra que registrar al cliente
// Adicionalemente el formato de datos a enviar mediante mensajeria seria el siguiente
// <numero cliente>|<CODIGO OPERACION>-<USUARIO>:<PASSWORD>
void introducirDatos(size_t, PIPE *, char);

//Obtiene su numero de cliente
size_t obtenerNumeroCliente();

bool validarMensajeGuardia(size_t, PIPE *, char *);

void salirPrograma();

#endif //SISTEMATIENDA_CLIENTE_H
