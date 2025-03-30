#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vehiculos.h"
#include "file.h"

void anadirVehiculo(FILE* archivo) {
    Vehiculo nuevoVehiculo;

    printf("Introduce la marca del vehiculo: ");
    scanf("%s", nuevoVehiculo.marca);

    printf("Introduce el modelo del vehiculo: ");
    scanf("%s", nuevoVehiculo.modelo);

    printf("Introduce el anio del vehiculo: ");
    scanf("%d", &nuevoVehiculo.anio);

    printf("Introduce el precio del vehiculo: ");
    scanf("%f", &nuevoVehiculo.precio);

    guardarVehiculo(archivo, &nuevoVehiculo);
}
void filtrarVehiculos(FILE* archivo) {
    char criterio[50];
    char valor[50];
    char linea[256];
    Vehiculo vehiculo;

    printf("\nFiltrar vehculos por: marca, modelo, anio: ");
    scanf("%s", criterio);
    printf("Ingrese el valor a buscar: ");
    scanf("%s", valor);

    rewind(archivo);
    printf("\nVehiculos encontrados:\n");
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        sscanf(linea, "%[^;];%[^;];%d;%f", vehiculo.marca, vehiculo.modelo, &vehiculo.anio, &vehiculo.precio);

        if ((strcmp(criterio, "marca") == 0 && strcmp(vehiculo.marca, valor) == 0) ||
            (strcmp(criterio, "modelo") == 0 && strcmp(vehiculo.modelo, valor) == 0) ||
            (strcmp(criterio, "anio") == 0 && atoi(valor) == vehiculo.anio) ||
            (strcmp(criterio, "precio") == 0 && atof(valor) == vehiculo.precio)) {
            printf("%s %s %d %.2f\n", vehiculo.marca, vehiculo.modelo, vehiculo.anio, vehiculo.precio);
        }
    }
}
