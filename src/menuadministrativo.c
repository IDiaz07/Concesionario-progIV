#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "concesionario.h"
#include "servicios.h"
#include "database.h"
#include "mensajesusuarios.h"

void mostrarMenuDeustoMotors() {
    printf("\nMenu Administrativo:\n");
    printf("1. Enviar mensaje a los usuarios\n");
    printf("2. Ver todos los vehiculos vendidos\n");
    printf("3. Aniadir vehiculo\n");
    printf("4. Mirar Plantilla\n");
    printf("5. Salir\n");
}


int seleccionarOpcionDeustoMotors() {
    int opcion;
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

void menuAdministrativo(sqlite3 *db, SOCKET cliente_fd) {
    // Menú especial para Administradores
    FILE* archivo;
    inicializarArchivo(&archivo, cliente_fd);
    int opcionDeustoMotors;
    do {
        mostrarMenuDeustoMotors();
        opcionDeustoMotors = seleccionarOpcionDeustoMotors();


        switch (opcionDeustoMotors) {
        case 1:
            enviarMensajeAUsuarios(db);
            break;
        case 2:
            // Implementar verVehiculosVendidos()
            break;
        case 3:
            anadirVehiculo(db,archivo, cliente_fd);
            break;
        case 4:
            cargarPlantillaDesdeArchivo(db, "plantilla.txt");
            mostrarPlantilla(db);
            break;
        case 5:

            break;
        case 6:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opcion no valida.\n");
        }
    } while (opcionDeustoMotors != 5);
}