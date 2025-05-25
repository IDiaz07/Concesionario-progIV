#ifndef VEHICULOS_H
#define VEHICULOS_H

#include <stdio.h>
#include "database.h"
#include "sockets.h"

typedef struct {
    char marca[50];
    char modelo[50];
    int anio;
    int precio;
} Vehiculo;


void anadirVehiculo(sqlite3 *db, FILE* archivo, SOCKET cliente_fd);


void filtrarVehiculos(sqlite3 *db, SOCKET cliente_fd);



void ComprarVehiculo(sqlite3 *db, SOCKET cliente_fd);

void mostrarVehiculosVendidos(sqlite3 *db, SOCKET cliente_fd);


#endif
