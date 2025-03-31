#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "database.h"

// Función para mostrar el menú de notificaciones
void menuNoti(sqlite3 *db, int id_usuario) {
    int opcion;
    do {
        printf("\nMenu Notificaciones:\n");
        printf("1. Eliminar todas las Notificaciones\n");
        printf("2. Eliminar una notificacion\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                // Llamamos a la función para eliminar todas las notificaciones de un usuario
                eliminarTodasLasNotificaciones(db, id_usuario);
                break;
            case 2:
                printf("Ingrese el ID de la notificacion a eliminar: ");
                int id;
                scanf("%d", &id);
                // Llamamos a la función para eliminar una notificación específica de un usuario
                eliminarNotificacionPorID(db, id_usuario, id);
                break;
            case 3:
                printf("Saliendo del menu de notificaciones...\n");
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 3);
}
