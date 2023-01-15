//
// Created by maskla on 9/01/23.
//

#include "Asistente de Compra.h"
#include "Utils/ImpresoraMensajes.h"

#include "Utils/Messages.h"
#include <stdlib.h>
#include <string.h>

void iniciarTurnoAsistente() {
    imprimirMensaje(ASISTENTE_COMPRA_TAG, "Asistente de compras reportandose a su puesto.\n");
    PIPE *canal_tienda = getPipe(CANAL_MENSAJES_TIENDA_ID);
    ActividadAsistenteCompras *actividad = (ActividadAsistenteCompras *) malloc(sizeof(ActividadAsistenteCompras));
    while (true) {
        imprimirMensaje(ASISTENTE_COMPRA_TAG, "Esperando a que entre una solicitud.\n");
        actividad = interpretarPeticion(actividad, canal_tienda);
        realizarActividadAsistenteCompras(actividad, canal_tienda);
    }
}

//Formato <Numero Peticion>:<cliente>|<numero_vendedor>:<Cantidad>-<Producto>
void *interpretarPeticion(ActividadAsistenteCompras *actividad, PIPE *canal_tienda) {
    //Extraer numero peticion
    readMsg(canal_tienda->msg_id, &canal_tienda->mbuf, asistente);
    char *wildcard = NULL;
    memset(actividad->articulo.nombre, '\0', 255);
    actividad->actividad = strtoul(canal_tienda->mbuf.mText, &wildcard, 10);
    actividad->numero_cliente = strtoul(wildcard + 1, &wildcard, 10);
    ++wildcard;
    actividad->numero_vendor = strtoul(wildcard, &wildcard, 10);
    ++wildcard;
    actividad->articulo.cantidad = strtoul(wildcard, &wildcard, 10);
    ++wildcard;
    strcpy(actividad->articulo.nombre, wildcard);
}


void realizarActividadAsistenteCompras(ActividadAsistenteCompras *actividad, PIPE *canal_tienda) {
    Carrito *carrito;
    switch (actividad->actividad) {
        case crear_carrito:
            nuevoCarrito(actividad->lista_carritos, actividad->numero_cliente);
            break;
        case obtener_total_carrito:
            carrito = obtenerCarrito(actividad->lista_carritos, actividad->numero_cliente);
            if (carrito != NULL) {
                sprintf(canal_tienda->mbuf.mText, "%ld", obtenerTotalCarrito(carrito));
                canal_tienda->mbuf.mType = vendedor + actividad->numero_vendor;
                sendMsg(canal_tienda->msg_id, canal_tienda->mbuf.mText, canal_tienda->mbuf.mType);
            }
            break;
        case agregar_articulo:
            agregarAlCarrito(actividad->lista_carritos, actividad->numero_cliente,
                             buscarArticulo(actividad->articulo.nombre));
            break;
        case quitar_articulo:
            quitarDelCarrito(actividad->lista_carritos, actividad->numero_cliente, actividad->articulo.nombre,
                             actividad->articulo.cantidad);
            break;
        case eliminar_carrito:
            eliminarCarrito(actividad->lista_carritos, actividad->numero_cliente);
            break;
        default:
            imprimirError(ASISTENTE_COMPRA_TAG, "No existe la actividad solicitada.\n");
            break;
    }
}

void nuevoCarrito(ListaCarritos *lista, size_t cliente) {
    Carrito *carrito_actual = lista;
    while (carrito_actual->siguiente != NULL) {
        carrito_actual = carrito_actual->siguiente;
    }
    carrito_actual->siguiente = (Carrito *) malloc(sizeof(Carrito));
    carrito_actual->siguiente->cliente = cliente;
    carrito_actual->siguiente->siguiente = NULL;
    carrito_actual->siguiente->lista_articulos = NULL;
}

void agregarAlCarrito(ListaCarritos *lista, size_t cliente, Articulo *articulo) {
    Carrito *carrito_cliente = lista;
    Articulo *articulo_lista;
    while (carrito_cliente != NULL) {
        if (carrito_cliente->cliente == cliente) {
            articulo_lista = buscarArticuloEnCarrito(carrito_cliente, articulo->nombre);
            if (articulo_lista != NULL) {
                ++articulo_lista->cantidad;
            } else {
                articulo_lista = ultimoArticuloDeLista(carrito_cliente->lista_articulos);
                if (articulo_lista != NULL) {
                    articulo_lista->siguiente = (Articulo *) malloc(sizeof(Articulo));
                    articulo_lista = articulo_lista->siguiente;
                } else {
                    articulo_lista = (Articulo *) malloc(sizeof(Articulo));
                }
                strcpy(articulo_lista->nombre, articulo->nombre);
                articulo_lista->cantidad = 1;
                articulo_lista->siguiente = NULL;
                articulo_lista->precio = articulo->precio;
            }
            break;
        }
        carrito_cliente = carrito_cliente->siguiente;
    }
}

void quitarDelCarrito(ListaCarritos *lista, size_t cliente, char *nombre_articulo, size_t cantidad) {
    Carrito *carrito = obtenerCarrito(lista, cliente);
    Articulo *articulo;
    long resultado;
    if (carrito != NULL) {
        articulo = buscarArticuloEnCarrito(carrito, nombre_articulo);
        if (articulo != NULL) {
            resultado = articulo->cantidad - cantidad;
            articulo->cantidad = resultado >= 0 ? resultado : 0;
        }
    }
}

void eliminarCarrito(ListaCarritos *lista, size_t cliente) {
    Carrito *carrito_previo = lista;
    Carrito *carrito_actual;
    if (carrito_previo != NULL) {
        carrito_actual = carrito_previo->siguiente;
        while (carrito_actual != NULL) {
            if (cliente == carrito_actual->cliente) {
                carrito_previo->siguiente = carrito_actual->siguiente;
                free(carrito_actual->lista_articulos);
                free(carrito_actual);
                break;
            }
        }
    }
}

Carrito *obtenerCarrito(ListaCarritos *lista, size_t cliente) {
    Carrito *carrito_actual = lista;
    while (carrito_actual != NULL) {
        if (carrito_actual->cliente == cliente)
            return carrito_actual;
    }
    return NULL;
}

Articulo *ultimoArticuloDeLista(ListaArticulos *lista) {
    Articulo *articulo_actual = lista;
    while (articulo_actual != NULL) {
        if (articulo_actual->siguiente == NULL)
            return articulo_actual;
        articulo_actual = articulo_actual->siguiente;
    }
    return NULL;
}

Articulo *buscarArticuloEnCarrito(Carrito *carrito, char *nombre_articulo) {
    Articulo *articulo_actual = carrito->lista_articulos;
    while (articulo_actual != NULL) {
        if (strcmp(nombre_articulo, articulo_actual->nombre) == 0) {
            return articulo_actual;
        }
        articulo_actual = articulo_actual->siguiente;
    }
    return NULL;
}

long obtenerTotalCarrito(Carrito *carrito) {
    long total = 0;
    Articulo *articulo_actual = carrito->lista_articulos;
    while (articulo_actual != NULL) {
        articulo_actual += articulo_actual->precio;
        articulo_actual = articulo_actual->siguiente;
    }
    return total;
}