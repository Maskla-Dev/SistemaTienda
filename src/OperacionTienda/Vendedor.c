//
// Created by maskla on 9/01/23.
//

#include "Vendedor.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Utils/ImpresoraMensajes.h"

void iniciarTurnoVendedor(size_t numero_vendedor) {
    limpiarInventario();
    char SELLER_NAME[SELLERNAME_LENGTH];
    obtenerNombreVendedor(numero_vendedor, SELLER_NAME);
    PIPE *canal_tienda = getPipe(CANAL_MENSAJES_TIENDA_ID);
    PIPE *canal_clientes = getPipe(CANAL_MENSAJES_CLIENTE_ID);
    while (true) {

    }
}

Articulo *buscarArticulo(char *nombre, ListaArticulos *lista) {
    while (lista != NULL) {
        if (strcmp(nombre, lista->nombre) == 0)
            return lista;
        lista = lista->siguiente;
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

//Formato de separacion de articulos item:cantidad:precio
// TODO:Tratar como region critica
ListaArticulos *obtenerInventario(size_t numero_vendedor) {
    ListaArticulos *inventario, *articulo_actual;
    char nombre_archivo[123];
    char contenido[256];
    sprintf(nombre_archivo, "files/%s", "Inventario");
    crearArchivo("files", nombre_archivo);
    FILE *archivo = fopen(nombre_archivo, "r");
    char datos_articulo[3][60];
    bool es_primer_articulo = true;
    size_t pos = 0;
    char ch;
    size_t tipo_dato = 0;
    memset(datos_articulo[0], '\0', 60);
    memset(datos_articulo[1], '\0', 60);
    memset(datos_articulo[2], '\0', 60);
    if (NULL == archivo) {
        vendedorError("No se pudo recuperar el inventario", numero_vendedor);
        exit(EXIT_FAILURE);
    }
    do {
        ch = (char) fgetc(archivo);
        switch (ch) {
            case ':':
                ++tipo_dato;
                pos = 0;
                break;
            case '\n':
                if (es_primer_articulo) {
                    articulo_actual = (Articulo *) malloc(sizeof(Articulo));
                    inventario = articulo_actual;
                    es_primer_articulo = false;
                } else {
                    articulo_actual->siguiente = (Articulo *) malloc(sizeof(Articulo));
                    articulo_actual = articulo_actual->siguiente;
                }
                strcpy(articulo_actual->nombre, datos_articulo[0]);
                articulo_actual->cantidad = strtoul(datos_articulo[1], NULL, 10);
                articulo_actual->precio = strtoul(datos_articulo[2], NULL, 10);
                memset(datos_articulo[0], '\0', 60);
                memset(datos_articulo[1], '\0', 60);
                memset(datos_articulo[2], '\0', 60);
                tipo_dato = 0;
                pos = 0;
                break;
            default:
                datos_articulo[tipo_dato][pos++] = ch;
                break;
        }
    } while (ch != EOF);
    if (articulo_actual != NULL)
        articulo_actual->siguiente = NULL;
    fclose(archivo);
    return inventario;
}

void obtenerNombreVendedor(size_t numero_vendedor, char *contenedor) {
    memset(contenedor, '\0', SELLERNAME_LENGTH);
    sprintf(contenedor, "%s %lu", SELLER_TAG, numero_vendedor);
}

void vendedorError(char *error, size_t numero_vendedor) {
    char nombre_completo[26];
    obtenerNombreVendedor(numero_vendedor, nombre_completo);
    imprimirError(nombre_completo, error);
}

void vendedorMensaje(char *mensaje, size_t numero_vendedor) {
    char nombre_completo[26];
    obtenerNombreVendedor(numero_vendedor, nombre_completo);
    imprimirMensaje(nombre_completo, mensaje);
}

void limpiarInventario() {
    crearArchivo("files", "files/Inventario");
    FILE *archivo = fopen("files/Inventario", "w");
    char *contenido = "Mazapanes:25:2\nManzanas:50:6\nFrijoles:20:23\nPaquetaxo:15:30\n";
    fprintf(archivo, "%s", contenido);
    fclose(archivo);
}