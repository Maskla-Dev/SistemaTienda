//
// Created by maskla on 9/01/23.
//

#ifndef SISTEMATIENDA_SUPERVISOR_H
#define SISTEMATIENDA_SUPERVISOR_H

#include <stdbool.h>

#define SUPERVISOR_TAG "Supervisor"

//Crea un proceso hijo cuyo rol es el guardia
bool registrarGuardia();

//Crea procesos hijos cuyos roles son de vendedor
bool registrarVendedores(size_t);

//Crea proceso hijo que asume el rol del asistente de compras
bool registrarAsistenteCompras();

//Espera a que el cliente se conecte
void esperarConexionCliente();

#endif //SISTEMATIENDA_SUPERVISOR_H
