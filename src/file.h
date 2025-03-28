#ifndef FILE_H
#define FILE_H

#include "vehiculos.h"
#include <stdio.h>

void inicializarArchivo(FILE** archivo);
void guardarVehiculo(FILE* archivo, Vehiculo* vehiculo);
void mostrarVehiculos(FILE* archivo);
void exportarAFichero(FILE* archivo, const char* nombreFichero);

#endif
