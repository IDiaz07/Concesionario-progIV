#include <stdio.h>
#include "sqlite3.h"
#include <string.h>
#include "database.h"

int abrirDB(sqlite3 **db) {
    int rc = sqlite3_open("usuarios.db", db);
    if (rc) {
        printf("No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    return SQLITE_OK;
}

int crearTablaUsuarios(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS usuarios ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "nombre_usuario TEXT NOT NULL, "
                      "contrasena TEXT NOT NULL, "
                      "email TEXT);";

    char *errMsg;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al crear la tabla: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    return SQLITE_OK;
}

int registrarUsuario(sqlite3 *db, const char *nombre_usuario, const char *contrasena, const char *email) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO usuarios (nombre_usuario, contrasena, email) VALUES (?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error en la preparación de la sentencia: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_text(stmt, 1, nombre_usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Error al registrar usuario: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int verificarUsuario(sqlite3 *db, const char *nombre_usuario, const char *contrasena) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM usuarios WHERE nombre_usuario = ? AND contrasena = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error en la preparación de la sentencia: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_text(stmt, 1, nombre_usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return SQLITE_OK; // Usuario y contraseña correctos
    }

    sqlite3_finalize(stmt);
    return SQLITE_ERROR; // Usuario o contraseña incorrectos
}

