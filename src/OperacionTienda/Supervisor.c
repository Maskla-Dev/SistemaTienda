#include <stdlib.h>
#include <unistd.h>
#include "Supervisor.h"
#include "Utils/ImpresoraMensajes.h"
#include "Utils/SemLogic.h"
#include "Utils/Utils.h"
#include "Guardia.h"
#include "Vendedor.h"
#include "Asistente de Compra.h"

/**
 * El supervisor se encarga de crear las instancias para el funcionamiento del guardia, vendedores y el asistente.
 * Crea los semaforos de sincronizacion para el acceso al archivo al inventario y el conteo de clientes
 * Las regiones criticas del sistema (conteo de clientes)
 * Determina el vendedor que atendera al clente
 */

//Hilo principal, funciones del rol de supervisor
void iniciarSupervisor() {
    //Inicia recursos para funcionamiento del sistema
    DatosSupervisor datos;
    iniciarRecursos(&datos);
    //Canal de comunicacion interno del sistema
    if (registrarGuardia()) {
        if (registrarVendedores(NUMERO_VENDEDORES) && registrarAsistenteCompras()) {
            while (true) {
                direccionarCliente(&datos);
            }
        }
    }
    imprimirError(SUPERVISOR_TAG, "No se puede iniciar operaciones en tienda sin el personal necesario\n");
}

void iniciarRecursos(DatosSupervisor *datos) {
    mensajeSupervisor("Inicializando recursos...");
    //Crear los elementos de sincronizacion entre procesos y mutex para regiones criticas
    datos->semaforos_recursos = createSemset(RECURSOS_COMPARTIDOS_ID, NUM_RECURSOS_COMPARTIDOS, 'A');
    setSemvalue(datos->semaforos_recursos, conteo_clientes, 1);
    setSemvalue(datos->semaforos_recursos, inventario, 1);
    mensajeSupervisor("\tRecursos de sincronizacion cargados.");
    //Crear recurso compartido para cuenta de vendedores
    datos->conteo_clientes = crearRegionNumeroNatural(REGION_CONTEO_CLIENTES_ID, false);
    mensajeSupervisor("\tRecursos de conteo cargados.");
    //Canal de comunicacion para clientes y agentes del sistema
    datos->canal_clientes = getPipe(CANAL_MENSAJES_CLIENTE_ID);
    datos->canal_tienda = getPipe(CANAL_MENSAJES_TIENDA_ID);
    mensajeSupervisor("\tCanales de comunicacion cargados.");
    datos->clientes_entrantes = 0;
}

void direccionarCliente(DatosSupervisor *datos) {
    size_t cliente;
    char vendedor_asignado[20];
    char mensajes_buf[124];
    mensajeSupervisor("Esperando mensaje del guardia...");
    //Obtiene numero de cliente para posteriormente enviar el vendedor_asignado que le corresponde
    readMsg(datos->canal_tienda->msg_id, &datos->canal_tienda->mbuf, (obtenerCanal(supervisor)));
    cliente = strtoul(datos->canal_tienda->mbuf.mText, NULL, 10);
    sprintf(mensajes_buf, "El cliente %lu quiere comprar", cliente);
    mensajeSupervisor(mensajes_buf);
    //Envia al cliente el numero de vendedor_asignado que le corresponde
    sprintf(vendedor_asignado, "%lu", obtenerCanal(vendedor) + (datos->clientes_entrantes % NUMERO_VENDEDORES));
    sprintf(mensajes_buf, "Se asigna el vendedor %s.", vendedor_asignado);
    mensajeSupervisor(mensajes_buf);
    ++datos->clientes_entrantes;
    sendMsg(datos->canal_clientes->msg_id, vendedor_asignado, (long) cliente);
}

void mensajeSupervisor(char *mensaje) {
    imprimirMensaje(SUPERVISOR_TAG, mensaje);
}

void errorSupervisor(char *error) {
    imprimirError(SUPERVISOR_TAG, error);
}

bool registrarGuardia() {
    pid_t pid;
    imprimirMensaje(SUPERVISOR_TAG, "Esperando a que el guardia inicie su turno...\n");
    pid = fork();
    switch (pid) {
        case -1:
            imprimirError(SUPERVISOR_TAG, "Guardia no se ha reportado a su puesto.\n");
            return false;
        case 0:
            return iniciarTurnoGuardia();
        default:
            imprimirMensaje(SUPERVISOR_TAG, "Guardia se ha reportado a su puesto.\n");
            return true;
    }
}

bool registrarVendedores(size_t numero_vendedores) {
    pid_t pid;
    size_t cuenta_vendedores = 0;
    imprimirMensaje(SUPERVISOR_TAG, "Esperando a vendedores...\n");
    if (numero_vendedores > 0) {
        while (cuenta_vendedores < numero_vendedores) {
            pid = fork();
            switch (pid) {
                case -1:
                    imprimirError(SUPERVISOR_TAG, "No se han reportado los suficientes vendedores.\n");
                    return false;
                case 0:
                    iniciarTurnoVendedor(cuenta_vendedores);
                    break;
                default:
                    ++cuenta_vendedores;
                    imprimirMensaje(SUPERVISOR_TAG, "Se ha reportado un vendedor al trabajo...\n");
                    break;
            }
        }
        return true;
    }
    return false;
}

bool registrarAsistenteCompras() {
    pid_t pid;
    imprimirMensaje(SUPERVISOR_TAG, "Esperando a que el asistente de compras inicie su turno...\n");
    pid = fork();
    switch (pid) {
        case -1:
            imprimirError(SUPERVISOR_TAG, "Asistente de compras no se ha reportado a su puesto.\n");
            return false;
        case 0:
            iniciarTurnoAsistente();
        default:
            imprimirMensaje(SUPERVISOR_TAG, "Asistente de compras se ha reportado a su puesto.\n");
            return true;
    }
}