#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sqlite3.h"

void enviarMensajeAUsuarios(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, nombre_usuario FROM usuarios;";
    char mensajeBase[500], mensajePersonalizado[600];
    char sqlInsert[700];
    time_t t;
    struct tm *tm_info;
    char fechaHora[20];
    time(&t);
    tm_info = localtime(&t);
    strftime(fechaHora, sizeof(fechaHora), "%Y-%m-%d %H:%M:%S", tm_info);

    printf("Ingrese el mensaje a enviar (use {nombre} para personalizar con el nombre del usuario):\n");
    while (getchar() != '\n');
    fgets(mensajeBase, sizeof(mensajeBase), stdin);
    mensajeBase[strcspn(mensajeBase, "\n")] = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id_usuario = sqlite3_column_int(stmt, 0);
        const char *nombre_usuario = (const char *)sqlite3_column_text(stmt, 1);

        // Personalizar mensaje con el nombre del usuario
        snprintf(mensajePersonalizado, sizeof(mensajePersonalizado), mensajeBase, nombre_usuario);

        // Insertar en la base de datos con fecha y hora
        snprintf(sqlInsert, sizeof(sqlInsert),
                 "INSERT INTO notificaciones (id_usuario, mensaje, fecha_hora) VALUES (%d, '%s', '%s');",
                 id_usuario, mensajePersonalizado, fechaHora);

        char *errMsg;
        int rc = sqlite3_exec(db, sqlInsert, 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            printf("Error al insertar mensaje para el usuario %s: %s\n", nombre_usuario, errMsg);
            sqlite3_free(errMsg);
        } else {
            printf("Mensaje enviado a %s a las %s: %s\n", nombre_usuario, fechaHora, mensajePersonalizado);
        }
    }

    sqlite3_finalize(stmt);
}
