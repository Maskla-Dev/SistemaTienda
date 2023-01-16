//
// Created by maskla on 15/01/23.
//

#include "OperacionTienda/Supervisor.h"
#include "OperacionTienda/Guardia.h"
#include "stdlib.h"
#include "string.h"

//Testing

int testExtraerDatosUsuario();

int testDescomponerPeticion();

int testRecuperarUsuarios();

int testBuscarUsuario();

int testAgregarUsuario();

int testGuardarLista();

int testProcesarPeticion();

int main(int argc, char **argv) {
    if (argc < 2 || strcmp("1", argv[1]) == 0)
        return testExtraerDatosUsuario();
    else if (strcmp("2", argv[1]) == 0)
        return testDescomponerPeticion();
    else if (strcmp("3", argv[1]) == 0)
        return testRecuperarUsuarios();
    else if (strcmp("4", argv[1]) == 0)
        return testBuscarUsuario();
    else if (strcmp("5", argv[1]) == 0)
        return testAgregarUsuario();
    else if (strcmp("6", argv[1]) == 0)
        return testGuardarLista();
    else if (strcmp("7", argv[1]) == 0)
        return testProcesarPeticion();
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

int testRecuperarUsuarios() {
    limpiarArchivo();
    USUARIO usuarios[] = {
            [0] = {.usuario = "JuanAlvarez", .password = "1234"},
            [1] = {.usuario = "MarcoLuan", .password = "%412"},
            [2] = {.usuario = "Luz1234", .password = "Federico"}
    };
    size_t iterador = 0;
    LISTA_USUARIOS *lista = recuperarLista();
    if (lista == NULL)
        return EXIT_FAILURE;
    while (lista != NULL) {
        if (strcmp(usuarios[iterador].usuario, lista->usuario->usuario) != 0 &&
            strcmp(usuarios[iterador].password, lista->usuario->password) != 0)
            return EXIT_FAILURE;
        lista = lista->siguiente;
        ++iterador;
    }
    return EXIT_SUCCESS;
}

int testBuscarUsuario() {
    limpiarArchivo();
    USUARIO usuario = {.usuario = "MarcoLuan", .password = "%412"};
    LISTA_USUARIOS *lista = recuperarLista();
    char *password = buscarUsuario(usuario.usuario, recuperarLista());
    if (strcmp(password, usuario.password) != 0)
        return EXIT_FAILURE;
    password = buscarUsuario(usuario.password, lista);
    if (password != NULL)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int testAgregarUsuario() {
    limpiarArchivo();
    USUARIO usuarios[] = {
            [0] = {.usuario = "JuanAlvarez", .password = "1234"},
            [1] = {.usuario = "MarcoLuan", .password = "%412"},
            [2] = {.usuario = "Luz1234", .password = "Federico"},
            [3] = {.usuario = "Marcela", .password = "NUNCA"}
    };
    LISTA_USUARIOS *lista = recuperarLista();
    size_t iterador = 0;
    agregarUsuarioLista(&usuarios[3], lista);
    agregarUsuarioLista(&usuarios[2], lista);
    while (lista != NULL) {
        if (strcmp(usuarios[iterador].usuario, lista->usuario->usuario) != 0 &&
            strcmp(usuarios[iterador].password, lista->usuario->password) != 0 &&
            iterador > 4)
            return EXIT_FAILURE;
        lista = lista->siguiente;
        ++iterador;
    }
    if (iterador == 3)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int testGuardarLista() {
    limpiarArchivo();
    USUARIO usuarios[] = {
            [0] = {.usuario = "JuanAlvarez", .password = "1234"},
            [1] = {.usuario = "MarcoLuan", .password = "%412"},
            [2] = {.usuario = "Luz1234", .password = "Federico"},
            [3] = {.usuario = "Marcela", .password = "NUNCA"}
    };
    LISTA_USUARIOS *lista = recuperarLista();
    size_t iterador = 0;
    agregarUsuarioLista(&usuarios[3], lista);
    guardarLista(lista);
    lista = recuperarLista();
    while (lista != NULL) {
        if (strcmp(usuarios[iterador].usuario, lista->usuario->usuario) != 0 &&
            strcmp(usuarios[iterador].password, lista->usuario->password) != 0)
            return EXIT_FAILURE;
        lista = lista->siguiente;
        ++iterador;
    }
    if (iterador != 4)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int testProcesarPeticion() {
    limpiarArchivo();
    USUARIO usuarios[] = {
            [0] = {.usuario = "JuanAlvarez", .password = "1234"},
            [1] = {.usuario = "MarcoLuan", .password = "%412"},
            [2] = {.usuario = "Luz1234", .password = "Federico"},
            [3] = {.usuario = "Marcela", .password = "NUNCA"}
    };
    PeticionGuardia peticion = {
            .numero_usuario = obtenerCanal(0),
            .operacion = 'A',
            .datos = {
                    .usuario = "JuanAlvarez",
                    .password = "1234"
            }
    };
    LISTA_USUARIOS *lista = recuperarLista();
    DatosSupervisor datos;
    iniciarRecursos(&datos);
    //Verifica caso correcto de peticion A
    procesarPeticion(&peticion, lista, datos.canal_tienda, datos.canal_clientes);
    readMsg(datos.canal_tienda->msg_id, &datos.canal_tienda->mbuf, obtenerCanal(supervisor));
    if (strtoul(datos.canal_tienda->mbuf.mText, NULL, 10) != peticion.numero_usuario)
        return EXIT_FAILURE;
    //Verifica caso incorrecto de peticion A
    //Password incorrecto
    strcpy(peticion.datos.password, "1235");
    procesarPeticion(&peticion, lista, datos.canal_tienda, datos.canal_clientes);
    readMsg(datos.canal_clientes->msg_id, &datos.canal_clientes->mbuf, (long) peticion.numero_usuario);
    if (strcmp(MSG_DATOS_INCORRECTOS, datos.canal_clientes->mbuf.mText) != 0)
        return EXIT_FAILURE;
    //Usuario incorrecto
    strcpy(peticion.datos.usuario, usuarios[3].usuario);
    procesarPeticion(&peticion, lista, datos.canal_tienda, datos.canal_clientes);
    readMsg(datos.canal_clientes->msg_id, &datos.canal_clientes->mbuf, (long) peticion.numero_usuario);
    if (strcmp(MSG_DATOS_INCORRECTOS, datos.canal_clientes->mbuf.mText) != 0)
        return EXIT_FAILURE;
    //Verifica caso correcto de peticion B
    peticion.operacion = 'R';
    procesarPeticion(&peticion, lista, datos.canal_tienda, datos.canal_clientes);
    readMsg(datos.canal_tienda->msg_id, &datos.canal_tienda->mbuf, obtenerCanal(supervisor));
    if (strtoul(datos.canal_tienda->mbuf.mText, NULL, 10) != peticion.numero_usuario)
        return EXIT_FAILURE;
    //Verifica caso incorrecto de peticion B
    procesarPeticion(&peticion, lista, datos.canal_tienda, datos.canal_clientes);
    readMsg(datos.canal_clientes->msg_id, &datos.canal_clientes->mbuf, (long) peticion.numero_usuario);
    if (strcmp(MSG_DATOS_INCORRECTOS, datos.canal_clientes->mbuf.mText) != 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}