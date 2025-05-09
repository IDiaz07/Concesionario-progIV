#ifndef FILE_H
#define FILE_H


#include <stdio.h>
#include "vehiculos.h"

void inicializarArchivo(FILE** archivo);
void guardarVehiculo(FILE* archivo, Vehiculo vehiculo);
void mostrarVehiculos(FILE* archivo);
void exportarAFichero(FILE* archivo, const char* nombreFichero);
void FiltrarMarca(FILE* archivo);


void cargarVehiculosDesdeArchivo(sqlite3 *db, FILE *archivo);

void cargarPlantillaDesdeArchivo(sqlite3 *db, const char *nombreArchivo);


void cargarVehiculosDesdeArchivo(sqlite3 *db, FILE *archivo);
void cargarPlantillaDesdeArchivo(sqlite3 *db, const char *nombreArchivo);


#endif
