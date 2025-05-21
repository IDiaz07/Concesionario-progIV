#ifndef FILE_H
#define FILE_H

#include <winsock2.h>
#include <stdio.h>
#include "vehiculos.h"
#include "sockets.h"

void inicializarArchivo(FILE** archivo, SOCKET cliente_fd);
void guardarVehiculo(FILE* archivo, Vehiculo vehiculo, SOCKET cliente_fd);
void mostrarVehiculos(FILE* archivo, SOCKET cliente_fd);
void exportarAFichero(FILE* archivo, const char* nombreFichero);
void FiltrarMarca(FILE* archivo, SOCKET cliente_fd);

void cargarVehiculosDesdeArchivo(sqlite3 *db, FILE *archivo, SOCKET cliente_fd);
void cargarPlantillaDesdeArchivo(sqlite3 *db, const char *nombreArchivo);


#endif
