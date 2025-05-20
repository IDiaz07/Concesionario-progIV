#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "database.h"

// Función para mostrar el menú de notificaciones
void menuNoti(sqlite3 *db, int id_usuario, SOCKET cliente_fd) {
    char buffer[256];
    send(cliente_fd, "MenuNotificaciones|1. Eliminar todas|2. Eliminar una|3. Salir|", 60, 0);

    int opcion = 0;
    recv(cliente_fd, buffer, sizeof(buffer), 0);
    opcion = atoi(buffer);
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
                eliminarTodasLasNotificaciones(db, id_usuario, cliente_fd);
                break;
            case 2:
                send(cliente_fd, "Ingrese el ID de la notificación a eliminar:", 45, 0);
                recv(cliente_fd, buffer, sizeof(buffer), 0);
                eliminarNotificacionPorID(db, id_usuario, atoi(buffer), cliente_fd);
                break;
            case 3:
                send(cliente_fd, "Saliendo del menú de notificaciones", 35, 0);
                break;
            default:
                send(cliente_fd, "Opción no válida", 17, 0);
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

