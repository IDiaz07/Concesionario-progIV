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

//FUNCION PARA MEJORAR LA INTEGRACION DE NOTIFICACIONES
void mostrarNotificaciones(sqlite3 *db, int id_usuario) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, mensaje FROM notificaciones WHERE id_usuario = ? AND leido = 0;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("Error al preparar la consulta de notificaciones.\n");
        return;
    }

    sqlite3_bind_int(stmt, 1, id_usuario);

    printf("\n--- Notificaciones No Leídas ---\n");
    int hayNotificaciones = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *mensaje = sqlite3_column_text(stmt, 1);
        printf("ID %d: %s\n", id, mensaje);
        hayNotificaciones = 1;
    }

    if (!hayNotificaciones) {
        printf("No tienes notificaciones nuevas.\n");
    }

    sqlite3_finalize(stmt);

    // Marcar todas como leídas
    const char *updateSql = "UPDATE notificaciones SET leido = 1 WHERE id_usuario = ?;";
    sqlite3_stmt *updateStmt;
    if (sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(updateStmt, 1, id_usuario);
        sqlite3_step(updateStmt);
        sqlite3_finalize(updateStmt);
    } else {
        printf("Error al marcar notificaciones como leídas.\n");
    }
}

