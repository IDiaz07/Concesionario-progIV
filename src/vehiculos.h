#ifndef VEHICULOS_H
#define VEHICULOS_H

#include <stdio.h>

typedef struct {
    char marca[50];
    char modelo[50];
    int anio;
    float precio;
} Vehiculo;

void anadirVehiculo(FILE* archivo);
void filtrarVehiculos(FILE* archivo);
#endif
