//
// Created by maskla on 15/01/23.
//

#include "OperacionTienda/Supervisor.h"
#include "stdlib.h"
#include "string.h"

int testIniciarRecursos();

int testDireccionarCliente();



int main(int argc, char **argv) {
    if (argc < 2 || (strcmp("1", argv[1]) == 0))
        return testIniciarRecursos();
    else if (strcmp("2", argv[1]) == 0)
        return testDireccionarCliente();
}

int testIniciarRecursos() {
    DatosSupervisor datos;
    iniciarRecursos(&datos);
    if (datos.canal_clientes == NULL || datos.canal_tienda == NULL || datos.clientes_entrantes != 0 ||
        datos.conteo_clientes == NULL || datos.semaforos_recursos->sem_id == -1)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int testDireccionarCliente() {
    DatosSupervisor datos;
    size_t conteo_inicial = 0;
    iniciarRecursos(&datos);
    sendMsg(datos.canal_tienda->msg_id, "2", (long) obtenerCanal(supervisor));
    direccionarCliente(&datos);
    readMsg(datos.canal_clientes->msg_id, &datos.canal_clientes->mbuf, 2);
    if (conteo_inicial == datos.clientes_entrantes || datos.canal_clientes->mbuf.mType != 2 ||
        strtol(datos.canal_clientes->mbuf.mText, NULL, 10) !=
        obtenerCanal(vendedor) + ((datos.clientes_entrantes - 1) % NUMERO_VENDEDORES))
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

