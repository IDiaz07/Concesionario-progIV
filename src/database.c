#include <stdio.h>
#include "sqlite3.h"
#include <string.h>
#include "database.h"
#include <locale.h>
#include <winsock2.h>

// -------------------- FUNCIONES BÁSICAS DE BD --------------------
int abrirDB(sqlite3 **db) {
    int rc = sqlite3_open("db.db", db);
    return rc;
}

// -------------------- MANEJO DE USUARIOS --------------------
int registrarUsuario(sqlite3 *db, const char *nombre_usuario, const char *contrasena, const char *email, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO usuarios (nombre_usuario, contrasena, email) VALUES (?, ?, ?);";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        send(cliente_fd, "Error preparando la consulta|", 27, 0);
        return rc;
    }

    sqlite3_bind_text(stmt, 1, nombre_usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        send(cliente_fd, "Error|Usuario ya existe o datos inválidos|", 41, 0);
        return rc;
    }
    
    send(cliente_fd, "Exito|Usuario registrado|", 24, 0);
    return SQLITE_OK;
}

int verificarUsuario(sqlite3 *db, const char *nombre_usuario, const char *contrasena, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM usuarios WHERE nombre_usuario = ? AND contrasena = ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        send(cliente_fd, "Error|Problema con la base de datos|", 35, 0);
        return rc;
    }

    sqlite3_bind_text(stmt, 1, nombre_usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_ROW) {
        send(cliente_fd, "Exito|Login correcto|", 20, 0);
        return SQLITE_OK;
    }
    
    send(cliente_fd, "Error|Credenciales incorrectas|", 30, 0);
    return SQLITE_ERROR;
}

// -------------------- MANEJO DE VEHÍCULOS --------------------
int registrarVehiculo(sqlite3 *db, char *marca, char *modelo, int anio, int precio, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO vehiculos (marca, modelo, anio, precio) VALUES (?, ?, ?, ?)";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        send(cliente_fd, "Error|Fallo en registro vehiculo|", 33, 0);
        return rc;
    }

    sqlite3_bind_text(stmt, 1, marca, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, modelo, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, anio);
    sqlite3_bind_int(stmt, 4, precio);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        send(cliente_fd, "Error|Vehiculo ya existe|", 24, 0);
        return rc;
    }
    
    send(cliente_fd, "Exito|Vehiculo registrado|", 25, 0);
    return SQLITE_OK;
}

void listarVehiculos(sqlite3 *db, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM vehiculos;";
    char buffer[512];
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        send(cliente_fd, "Error|Problema con la consulta|", 30, 0);
        return;
    }

    send(cliente_fd, "Listado|", 8, 0);
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        snprintf(buffer, sizeof(buffer), 
            "%s|%s|%d|%d\n",
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_int(stmt, 3),
            sqlite3_column_int(stmt, 4));
        
        send(cliente_fd, buffer, strlen(buffer), 0);
    }
    
    send(cliente_fd, "FinListado|", 11, 0);
    sqlite3_finalize(stmt);
}

// -------------------- MANEJO DE NOTIFICACIONES --------------------
void mostrarNotificaciones(sqlite3 *db, int idUsuario, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT mensaje FROM notificaciones WHERE id_usuario = ? AND leido = 0;";
    char buffer[256];
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        send(cliente_fd, "Error|Error en notificaciones|", 29, 0);
        return;
    }

    sqlite3_bind_int(stmt, 1, idUsuario);
    send(cliente_fd, "Notificaciones|", 15, 0);
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        snprintf(buffer, sizeof(buffer), "%s", sqlite3_column_text(stmt, 0));
        send(cliente_fd, buffer, strlen(buffer), 0);
        send(cliente_fd, "|", 1, 0);
    }
    
    send(cliente_fd, "FinNotificaciones|", 18, 0);
    sqlite3_finalize(stmt);
}

// -------------------- FUNCIONES AUXILIARES --------------------
int buscarIDUsuario(sqlite3 *db, const char *nombre_usuario, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM usuarios WHERE nombre_usuario=?";
    int id = -1;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        send(cliente_fd, "Error|Error en busqueda|", 24, 0);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, nombre_usuario, -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
        char respuesta[32];
        snprintf(respuesta, sizeof(respuesta), "Exito|ID|%d|", id);
        send(cliente_fd, respuesta, strlen(respuesta), 0);
    } else {
        send(cliente_fd, "Error|Usuario no encontrado|", 27, 0);
    }
    
    sqlite3_finalize(stmt);
    return id;
}

// -------------------- CREACIÓN DE TABLAS --------------------
int crearTablas(sqlite3 *db) {
    const char *sqls[] = {
        "CREATE TABLE IF NOT EXISTS usuarios (id INTEGER PRIMARY KEY AUTOINCREMENT, nombre_usuario TEXT UNIQUE, contrasena TEXT, email TEXT);",
        "CREATE TABLE IF NOT EXISTS vehiculos (id INTEGER PRIMARY KEY AUTOINCREMENT, marca TEXT, modelo TEXT, anio INTEGER, precio REAL);",
        "CREATE TABLE IF NOT EXISTS notificaciones (id INTEGER PRIMARY KEY AUTOINCREMENT, id_usuario INTEGER, mensaje TEXT, leido INTEGER DEFAULT 0);"
    };

    char *errMsg;
    for (int i = 0; i < sizeof(sqls)/sizeof(sqls[0]); i++) {
        if (sqlite3_exec(db, sqls[i], 0, 0, &errMsg) != SQLITE_OK) {
            printf("Error creando tablas: %s\n", errMsg);
            sqlite3_free(errMsg);
            return SQLITE_ERROR;
        }
    }
    return SQLITE_OK;
}