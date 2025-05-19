#ifndef CONCESIONARIO_H
#define CONCESIONARIO_H

#include "sqlite3.h"
#include "sockets.h"

void mostrarMenuDeustoMotors();
int seleccionarOpcionDeustoMotors();
void anadirVehiculo(sqlite3 *db, FILE *archivo, SOCKET cliente_fd);
void cargarPlantillaDesdeArchivo(sqlite3 *db, const char *filename);
char nombreUsuarioAutenticado[50];

#endif
