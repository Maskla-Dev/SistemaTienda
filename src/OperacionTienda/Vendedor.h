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
    struct Articulo *siguiente;
    size_t precio;
} Articulo, ListaArticulos;

void iniciarTurnoVendedor(size_t);

void obtenerNombreVendedor(size_t numero_vendedor, char *contenedor);

Articulo *buscarArticulo(char *nombre, ListaArticulos *lista);

void descontarArticulo(ListaArticulos *lista, char *nombre, size_t cantidad);

void agregarArticulo(ListaArticulos *lista, char *nombre, size_t cantidad);

ListaArticulos *obterListaArticulos();

void actualizarListaArticulos(ListaArticulos *lista);

void devolverListaArticulos(ListaArticulos *lista, PIPE *canal_clientes);

#endif //SISTEMATIENDA_VENDEDOR_H
