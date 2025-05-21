#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "database.h"

// Función para mostrar el menú de notificaciones
void menuNoti(sqlite3 *db, int id_usuario, SOCKET cliente_fd) {
    char buffer[256];
    int opcion = 0;

    do {
        const char *menu =  "\nMenu Notificaciones:\n"
                            "1. Eliminar todas las Notificaciones\n"
                            "2. Eliminar una notificacion\n"
                            "3. Salir\n"
                            "Seleccione una opcion: ";
        send(cliente_fd, menu, strlen(menu), 0);

        memset(buffer, 0, sizeof(buffer));
        recv(cliente_fd, buffer, sizeof(buffer), 0);
        opcion = atoi(buffer);

        switch (opcion) {
            case 1:
                eliminarTodasLasNotificaciones(db, id_usuario, cliente_fd);
                break;
            case 2:
                send(cliente_fd, "Ingrese el ID de la notificación a eliminar: ", 46, 0);
                memset(buffer, 0, sizeof(buffer));
                recv(cliente_fd, buffer, sizeof(buffer), 0);
                eliminarNotificacionPorID(db, id_usuario, atoi(buffer), cliente_fd);
                break;
            case 3:
                send(cliente_fd, "Saliendo del menu de notificaciones.\n", 38, 0);
                break;
            default:
                send(cliente_fd, "Opción no válida.\n", 19, 0);
                break;
        }

    } while (opcion != 3);
}


//FUNCION PARA MEJORAR LA INTEGRACION DE NOTIFICACIONES
void mostrarNotificaciones(sqlite3 *db, int id_usuario, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, mensaje FROM notificaciones WHERE id_usuario = ? AND leido = 0;";
    char buffer[512];

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        send(cliente_fd, "Error al preparar la consulta de notificaciones.\n", 50, 0);
        return;
    }

    sqlite3_bind_int(stmt, 1, id_usuario);
    int hayNotificaciones = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *mensaje = sqlite3_column_text(stmt, 1);

        snprintf(buffer, sizeof(buffer), "ID %d: %s\n", id, mensaje);
        send(cliente_fd, buffer, strlen(buffer), 0);
        hayNotificaciones = 1;
    }

    if (!hayNotificaciones) {
        send(cliente_fd, "No tienes notificaciones nuevas.\n", 34, 0);
    }

    sqlite3_finalize(stmt);

    // Marcar como leídas
    const char *updateSql = "UPDATE notificaciones SET leido = 1 WHERE id_usuario = ?;";
    sqlite3_stmt *updateStmt;
    if (sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(updateStmt, 1, id_usuario);
        sqlite3_step(updateStmt);
        sqlite3_finalize(updateStmt);
    } else {
        send(cliente_fd, "Error al marcar notificaciones como leidas.\n", 45, 0);
    }
}
