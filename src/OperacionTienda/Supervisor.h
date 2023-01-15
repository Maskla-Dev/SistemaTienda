//
// Created by maskla on 9/01/23.
//

#ifndef SISTEMATIENDA_SUPERVISOR_H
#define SISTEMATIENDA_SUPERVISOR_H

#include <stdbool.h>
#include "Utils/SemLogic.h"
#include "Utils/SharedMemory.h"
#include "Utils/Messages.h"
#include "Utils/Utils.h"

#define SUPERVISOR_TAG "Supervisor\0"
#define NUMERO_VENDEDORES 4

typedef struct DatosSupervisor {
    SEMAPHORE *semaforos_recursos;
    SHR_NUM_NATURAL *conteo_clientes;
    PIPE *canal_tienda;
    PIPE *canal_clientes;
    size_t clientes_entrantes;
} DatosSupervisor;

//Inicializa los recursos necesarios para el funcionamiento de la tienda
void iniciarRecursos(DatosSupervisor *);

//Imprime un mensaje a nombre del supervisor
void mensajeSupervisor(char *);

//Imprime un error a nombre del supervisor
void errorSupervisor(char *);

//Punto de entrada para ejecutar las funciones del supervisor
void iniciarSupervisor();

//Crea un proceso hijo cuyo rol es el guardia
bool registrarGuardia();

//Crea procesos hijos cuyos roles son de vendedor
bool registrarVendedores(size_t);

//Crea proceso hijo que asume el rol del asistente de compras
bool registrarAsistenteCompras();

//Obtiene el numero de cliente y lo direcciona a uno de los vendedores
void direccionarCliente(DatosSupervisor *datos);

#endif //SISTEMATIENDA_SUPERVISOR_H
