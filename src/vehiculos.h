#ifndef VEHICULOS_H
#define VEHICULOS_H

#include <stdio.h>
#include "database.h"

typedef struct {
    char marca[50];
    char modelo[50];
    int anio;
    int precio;
} Vehiculo;


void anadirVehiculo(sqlite3 *db, FILE* archivo);


void filtrarVehiculos(FILE* archivo);

void ComprarVehiculo(sqlite3 *db);
#endif
