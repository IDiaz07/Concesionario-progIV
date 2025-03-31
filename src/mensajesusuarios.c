#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

void enviarMensajeAUsuarios(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, nombre_usuario FROM usuarios;";
    char mensajeBase[500], mensajePersonalizado[600];

    //Usa {nombre} para personalizar con el nombre del usuario. Es decir se envian a todos el mensaje con su nombre personalizado.
    // Hola {nombre}, buenas. Apareceria como --> Hola DeustoMotors, buenas.
    printf("Ingrese el mensaje a enviar:\n");
    
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

        char *pos = strstr(mensajeBase, "{nombre}");
        if (pos) {
            strncpy(mensajePersonalizado, mensajeBase, pos - mensajeBase);
            mensajePersonalizado[pos - mensajeBase] = '\0';
            strcat(mensajePersonalizado, nombre_usuario);
            strcat(mensajePersonalizado, pos + 8);
        } else {
            strcpy(mensajePersonalizado, mensajeBase);
        }

        sqlite3_stmt *stmtInsert;
        const char *sqlInsert = "INSERT INTO notificaciones (id_usuario, mensaje) VALUES (?, ?);";

        if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmtInsert, NULL) == SQLITE_OK) {
            sqlite3_bind_int(stmtInsert, 1, id_usuario);
            sqlite3_bind_text(stmtInsert, 2, mensajePersonalizado, -1, SQLITE_STATIC);

            if (sqlite3_step(stmtInsert) == SQLITE_DONE) {
                printf("Mensaje enviado a %s: %s\n", nombre_usuario, mensajePersonalizado);
            } else {
                printf("Error al insertar mensaje para el usuario %s: %s\n", nombre_usuario, sqlite3_errmsg(db));
            }
        } else {
            printf("Error preparando la inserción: %s\n", sqlite3_errmsg(db));
        }
        
        sqlite3_finalize(stmtInsert);
    }

    sqlite3_finalize(stmt);
}