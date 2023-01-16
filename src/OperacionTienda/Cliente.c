//
// Created by maskla on 9/01/23.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Utils/SemLogic.h"
#include "Guardia.h"
#include "Cliente.h"

//TODO: Canal 1 de clientes es para el guardia, requiere offset de 4 + n vendedores

void iniciarCliente() {
    char *banner = "| Bienvenido a Tiendas Tito |";
    size_t numero_cliente;
    imprimirCaracter('=', strlen(banner));
    fprintf(stdout, "%s\n", banner);
    imprimirCaracter('=', strlen(banner));
    numero_cliente = obtenerNumeroCliente();
    fprintf(stdout, "El numero de cliente es: %lu\n", numero_cliente);
    PIPE *canal_clientes = getPipe(CANAL_MENSAJES_CLIENTE_ID);
    fprintf(stdout, "Pasando a ser atendido por el guardia\n");
    iniciarEtapaAutenticacion(numero_cliente, canal_clientes);
    //Proceso de conexion con el vendedor
    fprintf(stdout, "Formandose con el vendedor para poder comprar...");
}

void iniciarEtapaAutenticacion(size_t numero_cliente, PIPE *canal_mensajeria) {
    /**
     * Durante la etapa de autentificacion los clientes se forman para ser atendidos por el guardia
     * El Guardia los atiende segun su numeracion
     * Cada Cliente tiene asignado un numero
     * El canal de comunicacion entre los distintos agentes del sistema es mediante colas de mensajes
     * Hay un canal general para los clientes y un canal para los agentes del sistema
     */
    long opcion = 0;
    char input[25];
    do {
        fprintf(stdout, "Por favor introduzca el numero de alguna de estas opciones para continuar\n");
        fprintf(stdout, "1 Identificarse\n");
        fprintf(stdout, "2 Registrarse\n");
        fprintf(stdout, "-1 Para salir\n");
        scanf("%s", input);
        opcion = strtol(input, NULL, 10);
        opcion = validarOpcionAutentificacion(numero_cliente, canal_mensajeria, opcion) ? -2 : opcion;
    } while (opcion >= 0);
}

void imprimirCaracter(char caracter, size_t tam) {
    char buffer[tam + 1];
    memset(buffer, '\0', tam + 1);
    for (size_t i = 0; i < tam; ++i)
        buffer[i] = caracter;
    fprintf(stdout, "%s\n", buffer);
}

bool validarOpcionAutentificacion(size_t numero_usuario, PIPE *canal_mensajeria, long opcion) {
    SEMAPHORE *guardia_sem = createSemset("Guardia\0", 1, 'A');
    bool estatus = false;
    switch (opcion) {
        case 1:
            estatus = autentificarse(numero_usuario, canal_mensajeria);
            break;
        case 2:
            estatus = registrarse(numero_usuario, canal_mensajeria);
            break;
        case -1:
            salirPrograma();
        default:
            fprintf(stdout, "La opcion %ld no es una entrada valida\n", opcion);
            break;
    }
    free(guardia_sem);
    return estatus;
}

size_t obtenerNumeroCliente() {
    size_t numero_cliente;
    fprintf(stdout, "Obteniendo numero de cliente...");
    SHR_NUM_NATURAL *region_numero_cliente = crearRegionNumeroNatural(REGION_CONTEO_CLIENTES_ID, false);
    SEMAPHORE *mutex_numero_cliente = createSemset(RECURSOS_COMPARTIDOS_ID, NUM_RECURSOS_COMPARTIDOS, 'A');
    downSem(mutex_numero_cliente, conteo_clientes);
    numero_cliente = region_numero_cliente->numero;
    ++region_numero_cliente->numero;
    upSem(mutex_numero_cliente, conteo_clientes);
    return numero_cliente;
}

bool autentificarse(size_t numero_cliente, PIPE *canal_mensajeria) {
    size_t cuenta_intentos = 0;
    bool bandera;
    do {
        introducirDatos(numero_cliente, canal_mensajeria, 'A');
        bandera = validarMensajeGuardia(numero_cliente, canal_mensajeria, MSG_AUTENTIFICACION_CORRECTA);
        if (bandera)
            fprintf(stdout, "Bienvenido!\n");
        else {
            fprintf(stdout, "Credenciales invalidas, vuelva a intentarlo.\n");
            ++cuenta_intentos;
            if (cuenta_intentos >= 3) {
                fprintf(stdout, "Demasiados intentos fallidos.\n");
                fprintf(stdout, "Intentelo de nuevo mas tarde.\n");
                return false;
            }
        }
    } while (bandera && cuenta_intentos < 3);
    return true;
}

bool registrarse(size_t numero_cliente, PIPE *canal_mensajeria) {
    bool bandera;
    introducirDatos(numero_cliente, canal_mensajeria, 'R');
    bandera = validarMensajeGuardia(numero_cliente, canal_mensajeria, MSG_REGISTRO_CORRECTO);
    if (bandera) {
        fprintf(stdout, "Bienvenido!\n");
        return true;
    }
    fprintf(stdout, "Usuario ya existe, intente con un usuario nuevo o identifiquese correctamente.\n");
    return false;
}

void introducirDatos(size_t numero_cliente, PIPE *canal_mensajeria, char codigo_operacion) {
    char password[25];
    char usuario[25];
    memset(password, '\n', 25);
    memset(usuario, '\n', 25);
    memset(canal_mensajeria->mbuf.mText, '\n', 51);
    fprintf(stdout, "Introduzca el usuario (maximo 25 caracteres):\n");
    scanf("%s", usuario);
    fprintf(stdout, "Introduzca password (maximo 25 caracteres)\n");
    scanf("%s", password);
    sprintf(canal_mensajeria->mbuf.mText, "%lu|%c-%s:%s\n", numero_cliente + 1, codigo_operacion, usuario, password);
    sendMsg(canal_mensajeria->msg_id, canal_mensajeria->mbuf.mText, 1);
}

bool validarMensajeGuardia(size_t numero_cliente, PIPE *canal_mensajeria, char *mensaje_esperado) {
    readMsg(canal_mensajeria->msg_id, &canal_mensajeria->mbuf, (long) numero_cliente + 1);
    return strcmp(mensaje_esperado, canal_mensajeria->mbuf.mText) == 0;
}

void salirPrograma() {
    fprintf(stdout, "Gracias por usar el sistema, vuelva pronto...\n");
    exit(EXIT_SUCCESS);
}