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
