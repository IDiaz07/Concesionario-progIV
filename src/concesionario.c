#include <stdio.h>
#include <stdlib.h>
#include "concesionario.h"
#include "file.h"
#include "vehiculos.h"

void mostrarMenu() {
    printf("\nMenu:\n");
    printf("1. Aniadir vehiculo\n");
    printf("2. Mostrar vehiculos\n");
    printf("3. Exportar a archivo\n");
    printf("4. Salir\n");
}

int seleccionarOpcion() {
    int opcion;
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

int main() {
    FILE* archivo;

    inicializarArchivo(&archivo);

    int opcion;
    do {
        mostrarMenu();

        opcion = seleccionarOpcion();

        switch (opcion) {
            case 1:
                anadirVehiculo(archivo);
                break;
            case 2:
                mostrarVehiculos(archivo);
                break;
            case 3:
                exportarAFichero(archivo, "vehiculos.csv");
                break;
            case 4:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 4);

    fclose(archivo);
    return 0;
}
