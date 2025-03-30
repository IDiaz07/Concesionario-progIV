#ifndef VEHICULOS_H
#define VEHICULOS_H

#include <stdio.h>
#include "database.h"

typedef struct {
    char marca[50];
    char modelo[50];
    int anio;
    float precio;
} Vehiculo;

void anadirVehiculo(sqlite3 *db, FILE* archivo);

#endif
