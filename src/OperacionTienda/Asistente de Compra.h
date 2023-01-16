//
// Created by maskla on 9/01/23.
//

#ifndef SISTEMATIENDA_ASISTENTE_DE_COMPRA_H
#define SISTEMATIENDA_ASISTENTE_DE_COMPRA_H

#include <stdbool.h>
#include "Vendedor.h"
#include "Utils/Utils.h"

#define ASISTENTE_COMPRA_TAG "AsistenteCompra\0"

/**
 * Se encarga de gestionar los carritos para cada cliente
 * Crea un nuevo carrito
 * Agrega articulos al carrito
 * Quita articulos al carrito
 * Obtiene el total de la cuenta
 * Elimina carritos
 *
 */

typedef struct Carrito {
    size_t cliente;
    ListaArticulos *lista_articulos;
    struct Carrito *siguiente;
} Carrito, ListaCarritos;

typedef enum AccionAsistenteCompras {
    crear_carrito, agregar_articulo, obtener_total_carrito, quitar_articulo, eliminar_carrito
} AccionAsistenteCompras;

typedef struct ActividadAsistenteCompras {
    AccionAsistenteCompras actividad;
    Articulo articulo;
    size_t numero_cliente;
    ListaCarritos *lista_carritos;
    size_t numero_vendor;
} ActividadAsistenteCompras;

void actualizarCarritos(ListaCarritos *carritos);

void *interpretarPeticion(ActividadAsistenteCompras *actividad, PIPE *canal_tienda);

void realizarActividadAsistenteCompras(ActividadAsistenteCompras *actividad, PIPE *canal_tienda);

void nuevoCarrito(ListaCarritos *lista, size_t cliente);

void agregarAlCarrito(ListaCarritos *lista, size_t cliente, Articulo *articulo);

void quitarDelCarrito(ListaCarritos *lista, size_t cliente, char *nombre_articulo, size_t cantidad);

Articulo *buscarArticuloEnCarrito(Carrito *carrito, char *nombre_articulo);

Articulo *ultimoArticuloDeLista(ListaArticulos *lista);

void eliminarCarrito(ListaCarritos *lista, size_t cliente);

Carrito *obtenerCarrito(ListaCarritos *lista, size_t cliente);

long obtenerTotalCarrito(Carrito *carrito);

void iniciarTurnoAsistente();

#endif //SISTEMATIENDA_ASISTENTE_DE_COMPRA_H
