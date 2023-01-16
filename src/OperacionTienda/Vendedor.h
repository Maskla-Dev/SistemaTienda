//
// Created by maskla on 9/01/23.
//

#ifndef SISTEMATIENDA_VENDEDOR_H
#define SISTEMATIENDA_VENDEDOR_H

#include <stddef.h>
#include "Utils/Messages.h"
#include "Utils/Utils.h"

#define SELLERNAME_LENGTH 20
#define SELLER_TAG "Vendedor"

typedef struct Articulo {
    char nombre[255];
    size_t cantidad;
    size_t precio;
    struct Articulo *siguiente;
} Articulo, ListaArticulos;

/**
 * Se encarga de procesar las compras
 * Obtiene el catalogo y lo devuelve al cliente
 * Actualiza el estado del inventario (agregar o quitar)
 *
 */

//Obtiene la lista de articulos para vender
ListaArticulos *obtenerInventario(size_t numero_vendedor);

//Busca un articulo en la lista
Articulo *buscarArticulo(char *nombre, ListaArticulos *lista);


void iniciarTurnoVendedor(size_t);

void obtenerNombreVendedor(size_t numero_vendedor, char *contenedor);

void descontarArticulo(ListaArticulos *lista, char *nombre, size_t cantidad);

void agregarArticulo(ListaArticulos *lista, char *nombre, size_t cantidad);


void actualizarListaArticulos(ListaArticulos *lista);

void devolverListaArticulos(ListaArticulos *lista, PIPE *canal_clientes);

void limpiarInventario();

void vendedorMensaje(char *, size_t);

void vendedorError(char *, size_t);

#endif //SISTEMATIENDA_VENDEDOR_H
