#ifndef CONCESIONARIO_H
#define CONCESIONARIO_H

#include "sqlite3.h"

void mostrarMenuDeustoMotors();
int seleccionarOpcionDeustoMotors();
void anadirVehiculo(sqlite3 *db, FILE *archivo);
void cargarPlantillaDesdeArchivo(sqlite3 *db, const char *filename);

#endif
