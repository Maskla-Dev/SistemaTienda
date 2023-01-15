//
// Created by maskla on 9/01/23.
//

#include "Vendedor.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void iniciarTurnoVendedor(size_t numero_vendedor) {
    char SELLER_NAME[SELLERNAME_LENGTH];
    obtenerNombreVendedor(numero_vendedor, SELLER_NAME);
    PIPE *canal_tienda = getPipe(CANAL_MENSAJES_TIENDA_ID);
    PIPE *canal_clientes = getPipe(CANAL_MENSAJES_CLIENTE_ID);
    while (true) {

    }
}

Articulo *buscarArticulo(char *nombre, ListaArticulos *lista) {
    Articulo *articulo_actual = lista;
    while (articulo_actual != NULL) {
        if (strcmp(lista->nombre, articulo_actual->nombre) == 0) {
            return articulo_actual;
        }
        articulo_actual = articulo_actual->siguiente;
    }
    return NULL;
}

void descontarArticulo(ListaArticulos *lista, char *nombre, size_t cantidad) {
    Articulo *articulo = buscarArticulo(nombre, lista);
    long resultado;
    if (articulo != NULL) {
        resultado = articulo->cantidad - cantidad;
        articulo->cantidad = resultado >= 0 ? resultado : 0;
    }
}

void agregarArticulo(ListaArticulos *lista, char *nombre, size_t cantidad) {
    Articulo *articulo = buscarArticulo(nombre, lista);
    if (articulo != NULL) {
        articulo->cantidad += cantidad;
    }
}

//Formato de separacion de articulos <item>:<cantidad>-Precio
ListaArticulos *obtenerListaArticulos() {
    char *contenido_archivo;
    long tam_contenido;
    ListaArticulos *lista = (ListaArticulos *) malloc(sizeof(ListaArticulos));
    Articulo *articulo_actual = lista;
    FILE *archivo_vendedor = fopen("Inventario", "r");
}

void obtenerNombreVendedor(size_t numero_vendedor, char *contenedor) {
    memset(contenedor, '\0', SELLERNAME_LENGTH);
    sprintf(contenedor, "%s %lu", SELLER_TAG, numero_vendedor);
}