#include <stdio.h>
#include <string.h>
#include "mantenimiento.h"

void mantenimientoYReparacion() {
    Cita cita;

    printf("Accion seleccionada: Mantenimiento y Reparacion\n");

    printf("Selecciona el tipo de servicio:\n");
    printf("1. Cambio de aceite\n");
    printf("2. Reparacion de frenos\n");
    printf("3. Reemplazo de neumaticos\n");
    printf("4. Revision general\n");
    printf("5. Salir\n");

    int opcionServicio;
    printf("Seleccione el servicio deseado: ");
    scanf("%d", &opcionServicio);

    switch (opcionServicio) {
        case 1:
            strcpy(cita.tipoServicio, "Cambio de aceite");
            break;
        case 2:
            strcpy(cita.tipoServicio, "Reparación de frenos");
            break;
        case 3:
            strcpy(cita.tipoServicio, "Reemplazo de neumaticos");
            break;
        case 4:
            strcpy(cita.tipoServicio, "Revision general");
            break;
        case 5:
            printf("Saliendo del servicio de mantenimiento...\n");
            return;
        default:
            printf("Opcion no valida.\n");
            return;
    }

    printf("Introduce la fecha de la cita (formato: DD/MM/YYYY): ");
    scanf("%s", cita.fecha);

    printf("Introduce la hora de la cita (formato: HH:MM): ");
    scanf("%s", cita.hora);

    // Confirmar la reserva
    printf("\nCita reservada con exito:\n");
    printf("Servicio: %s\n", cita.tipoServicio);
    printf("Fecha: %s\n", cita.fecha);
    printf("Hora: %s\n", cita.hora);
}
