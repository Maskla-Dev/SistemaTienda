//
// Created by maskla on 16/01/23.
//
#include <string.h>
#include "OperacionTienda/Vendedor.h"

int testObtenerInventario();

int testBuscarArticulo();

int main(int argc, char **argv) {
    if (argc < 2 || strcmp("1", argv[1]) == 0)
        return testObtenerInventario();
    else if (strcmp("2", argv[1]) == 0)
        return testBuscarArticulo();
}

int testObtenerInventario() {
    limpiarInventario();
    ListaArticulos articulos[] = {
            [0] = {.nombre = "Mazapanes", .cantidad = 25, .precio = 2},
            [1] = {.nombre = "Manzanas", .cantidad = 50, .precio = 6},
            [2] = {.nombre = "Frijoles", .cantidad = 20, .precio = 23},
            [3] = {.nombre = "Paquetaxo", .cantidad = 15, .precio = 30}
    };
    ListaArticulos *inventario = obtenerInventario(0);
    size_t iterador = 0;
    if (inventario == NULL)
        return EXIT_FAILURE;
    while (inventario != NULL) {
        if (strcmp(articulos[iterador].nombre, inventario->nombre) != 0 &&
            inventario->cantidad != articulos[iterador].cantidad && inventario->precio != articulos->precio)
            return EXIT_FAILURE;
        inventario = inventario->siguiente;
        ++iterador;
    }
    return EXIT_SUCCESS;
}

int testBuscarArticulo() {
    limpiarInventario();
    char nombre_articulo1[25] = "Mazapanes";
    char nombre_articulo2[25] = "Mazapan";
    ListaArticulos *lista = obtenerInventario(0);
    Articulo *articulo = buscarArticulo(nombre_articulo1, lista);
    if (articulo == NULL)
        return EXIT_FAILURE;
    if (strcmp(nombre_articulo1, articulo->nombre) != 0)
        return EXIT_FAILURE;
    articulo = buscarArticulo(nombre_articulo2, lista);
    if (articulo != NULL)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}