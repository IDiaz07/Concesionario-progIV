#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sqlite3.h"
#include "sockets.h"

void enviarMensajeAUsuarios(sqlite3 *db, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, nombre_usuario FROM usuarios;";
    char mensajeBase[500], mensajePersonalizado[600];
    char sqlInsert[700];
    time_t t;
    struct tm *tm_info;
    char fechaHora[20];
    char buffer[2048] = {0};  // Acumula todos los mensajes aquí
    char linea[256];

    // Obtener fecha y hora actual
    time(&t);
    tm_info = localtime(&t);
    strftime(fechaHora, sizeof(fechaHora), "%Y-%m-%d %H:%M:%S", tm_info);

    // Pedir mensaje al cliente
    send(cliente_fd, "Ingrese el mensaje a enviar (use {nombre} para personalizar):\n", 65, 0);
    memset(mensajeBase, 0, sizeof(mensajeBase));
    recv(cliente_fd, mensajeBase, sizeof(mensajeBase), 0);

    // Preparar la consulta
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        snprintf(linea, sizeof(linea), "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        send(cliente_fd, linea, strlen(linea), 0);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id_usuario = sqlite3_column_int(stmt, 0);
        const char *nombre_usuario = (const char *)sqlite3_column_text(stmt, 1);

        // Reemplazar {nombre}
        char *pos = strstr(mensajeBase, "{nombre}");
        if (pos != NULL) {
            size_t inicio = pos - mensajeBase;
            snprintf(mensajePersonalizado, sizeof(mensajePersonalizado), "%.*s%s%s",
                     (int)inicio, mensajeBase, nombre_usuario, pos + strlen("{nombre}"));
        } else {
            snprintf(mensajePersonalizado, sizeof(mensajePersonalizado), "%s", mensajeBase);
        }

        // Insertar en la base de datos
        snprintf(sqlInsert, sizeof(sqlInsert),
                 "INSERT INTO notificaciones (id_usuario, mensaje, fecha_hora) VALUES (%d, '%s', '%s');",
                 id_usuario, mensajePersonalizado, fechaHora);

        char *errMsg = NULL;
        int rc = sqlite3_exec(db, sqlInsert, 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            snprintf(linea, sizeof(linea), "Error al insertar para %s: %s\n", nombre_usuario, errMsg);
            sqlite3_free(errMsg);
        } else {
            snprintf(linea, sizeof(linea), "Mensaje enviado a %s: %s\n", nombre_usuario, mensajePersonalizado);
        }

        // Acumular en buffer
        strncat(buffer, linea, sizeof(buffer) - strlen(buffer) - 1);
    }

    sqlite3_finalize(stmt);

    // Agregar línea final y enviar todo
    strncat(buffer, "Mensajes enviados.\nFIN_ENVIO\n", sizeof(buffer) - strlen(buffer) - 1);
    send(cliente_fd, buffer, strlen(buffer), 0);
}
