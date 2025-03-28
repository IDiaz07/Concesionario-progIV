#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vehiculos.h"
#include "file.h"

void anadirVehiculo(FILE* archivo) {
    Vehiculo nuevoVehiculo;

    printf("Introduce la marca del vehículo: ");
    scanf("%s", nuevoVehiculo.marca);

    printf("Introduce el modelo del vehículo: ");
    scanf("%s", nuevoVehiculo.modelo);

    printf("Introduce el año del vehículo: ");
    scanf("%d", &nuevoVehiculo.anio);

    printf("Introduce el precio del vehículo: ");
    scanf("%f", &nuevoVehiculo.precio);

    guardarVehiculo(archivo, &nuevoVehiculo);
}
