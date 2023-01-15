//
// Created by maskla on 15/01/23.
//

#include "../src/OperacionTienda/Supervisor.h"
#include "../src/OperacionTienda/Guardia.h"
#include "stdlib.h"
#include "string.h"

int testIniciarRecursos();

int testDireccionarCliente();

int testExtraerDatosUsuario();

int testDescomponerPeticion();

int main(int argc, char **argv) {
    if (argc < 2 || (strcmp("1", argv[1]) == 0))
        return testIniciarRecursos();
    else if (strcmp("2", argv[1]) == 0)
        return testDireccionarCliente();
    else if (strcmp("3", argv[1]) == 0)
        return testExtraerDatosUsuario();
    else if (strcmp("4", argv[1]) == 0)
        return testDescomponerPeticion();
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

int testExtraerDatosUsuario() {
    USUARIO usuario;
    char *password = "1234";
    char *nombre_usuario = "JuanAlvarez";
    char fuente[60];
    memset(fuente, '\0', 60);
    sprintf(fuente, "%s:%s", nombre_usuario, password);
    extraerDatosUsuario(fuente, &usuario);
    if (strcmp(nombre_usuario, usuario.usuario) != 0 || strcmp(password, usuario.password) != 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int testDescomponerPeticion() {
    PeticionGuardia *peticion;
    char sin_procesar[1024];
    size_t numero_usuario = 10;
    char operacion = 'A';
    char *nombre_usuario = "JuanAlvarez";
    char *password = "1234";
    sprintf(sin_procesar, "%lu-%c-%s:%s", numero_usuario, operacion, nombre_usuario, password);
    peticion = descomponerPeticion(sin_procesar);
    if (peticion->operacion != operacion || peticion->numero_usuario != numero_usuario ||
        strcmp(nombre_usuario, peticion->datos.usuario) != 0 ||
        strcmp(password, peticion->datos.password) != 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}