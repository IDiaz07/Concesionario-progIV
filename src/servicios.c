#include <stdio.h>
#include "servicios.h"
#include "mantenimiento.h"  // Asegúrate de incluir este archivo

// Mostrar el menú de servicios de DeustoMotors
void mostrarMenuServicios() {
    printf("\nMenu Servicios:\n");
    printf("1. Mantenimiento y Reparacion\n");
    printf("2. Garantias\n");
    printf("3. Salir\n");
}

// Seleccionar la opción en el menú
void MenuServicios() {
    int opcion;
    do {
        mostrarMenuServicios();
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion); // Leer la opción seleccionada

        switch (opcion) {
        case 1:
            mantenimientoYReparacion();  // Llamada a la función de mantenimiento
            break;
        case 2:
            garantias();
            break;
        case 3:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opcion no valida.\n");
        }
    } while (opcion != 3);
}

// Método para la opción de Garantías
void garantias() {
    printf("Accion seleccionada: Garantias\n");
    printf("Mostrando informacion sobre garantias...\n");
}
