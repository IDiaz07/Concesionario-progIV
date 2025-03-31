#ifndef FILE_H
#define FILE_H

#include "vehiculos.h"
#include <stdio.h>

void inicializarArchivo(FILE** archivo);
void guardarVehiculo(FILE* archivo, Vehiculo vehiculo);
void mostrarVehiculos(FILE* archivo);
void exportarAFichero(FILE* archivo, const char* nombreFichero);
void FiltrarMarca(FILE* archivo);
<<<<<<< HEAD
void cargarVehiculosDesdeArchivo(sqlite3 *db, FILE *archivo);
=======
void cargarPlantillaDesdeArchivo(sqlite3 *db, const char *nombreArchivo);
>>>>>>> 70fbbd7ed7f20d14142cbc64dde1b8c7d209e362

#endif
