#include <stdio.h>
#include <stdlib.h>
#include "concesionario.h"
#include "vehiculos.h"
#include "base_datos.h"

int main() {
    sqlite3* db;
    
    // Inicializar la base de datos
    inicializarBaseDeDatos(&db);

    int opcion;
    do {
        // Mostrar el menú principal
        mostrarMenu();

        // Obtener la opción seleccionada por el usuario
        opcion = seleccionarOpcion();

        // Ejecutar la opción seleccionada
        switch (opcion) {
            case 1:
                anadirVehiculo(db);
                break;
            case 2:
                mostrarVehiculos(db);
                break;
            case 3:
                exportarAFichero(db, "vehiculos.csv");
                break;
            case 4:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 4);

    // Cerrar la base de datos antes de finalizar el programa
    sqlite3_close(db);
    return 0;
}
