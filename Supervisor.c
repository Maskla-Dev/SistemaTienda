#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Supervisor.h"
#include "Utils/ImpresoraMensajes.h"
#include "Utils/SemLogic.h"
#include "Utils/Utils.h"
#include "Guardia.h"
#include "Vendedor.h"
#include "Asistente de Compra.h"

#define NUMERO_VENDEDORES 4

//Hilo principal, funciones del rol de supervisor
int main() {
    bool continuar_funciones = true;
    imprimirMensaje(SUPERVISOR_TAG, "Inicializando recursos...");
    //Crear los elementos de sincronizacion entre procesos y mutex para regiones criticas
    SEMAPHORE *sems = createSemset(RECURSOS_COMPARTIDOS_ID, NUM_RECURSOS_COMPARTIDOS, 'A');
    imprimirMensaje(SUPERVISOR_TAG, "\tRecursos de sincronizacion cargados.");
    //Crear recurso compartido para cuenta de vendedores
    SHR_NUM_NATURAL *conteo_clientes = crearRegionNumeroNatural(REGION_CONTEO_CLIENTES_ID, false);
    imprimirMensaje(SUPERVISOR_TAG, "\tRecursos de conteo cargados.");
    //Canal de comunicacion para clientes y agentes del sistema
    PIPE *canal_clientes = getPipe(CANAL_MENSAJES_CLIENTE_ID);
    PIPE *canal_tienda = getPipe(CANAL_MENSAJES_TIENDA_ID);
    imprimirMensaje(SUPERVISOR_TAG, "\tCanales de comunicacion cargados.");
    //Canal de comunicacion interno del sistema
    if (registrarGuardia()) {
        if (registrarVendedores(NUMERO_VENDEDORES) && registrarAsistenteCompras()) {
            while (true) {

            }
        }
    }
    imprimirError(SUPERVISOR_TAG, "No se puede iniciar operaciones en tienda sin el personal necesario");
    return EXIT_FAILURE;
}

bool registrarGuardia() {
    pid_t pid;
    imprimirMensaje(SUPERVISOR_TAG, "Esperando a que el guardia inicie su turno...");
    pid = fork();
    switch (pid) {
        case -1:
            imprimirError(SUPERVISOR_TAG, "Guardia no se ha reportado a su puesto.");
            return false;
        case 0:
            return iniciarTurnoGuardia();
        default:
            imprimirMensaje(SUPERVISOR_TAG, "Guardia se ha reportado a su puesto.");
            return true;
    }
}

bool registrarVendedores(size_t numero_vendedores) {
    pid_t pid;
    size_t cuenta_vendedores = 0;
    imprimirMensaje(SUPERVISOR_TAG, "Esperando a vendedores...");
    if (numero_vendedores > 0) {
        while (cuenta_vendedores < numero_vendedores) {
            pid = fork();
            switch (pid) {
                case -1:
                    imprimirError(SUPERVISOR_TAG, "No se han reportado los suficientes vendedores.");
                    return false;
                case 0:
                    iniciarTurnoVendedor(cuenta_vendedores);
                    break;
                default:
                    ++cuenta_vendedores;
                    imprimirMensaje(SUPERVISOR_TAG, "Se ha reportado un vendedor al trabajo...");
                    break;
            }
        }
        return true;
    }
    return false;
}

bool registrarAsistenteCompras() {
    pid_t pid;
    imprimirMensaje(SUPERVISOR_TAG, "Esperando a que el asistente de compras inicie su turno...");
    pid = fork();
    switch (pid) {
        case -1:
            imprimirError(SUPERVISOR_TAG, "Asistente de compras no se ha reportado a su puesto.");
            return false;
        case 0:
            return iniciarTurnoAsistente();
        default:
            imprimirMensaje(SUPERVISOR_TAG, "Asistente de compras se ha reportado a su puesto.");
            return true;
    }
}