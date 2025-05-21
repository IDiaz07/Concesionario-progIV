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
        send(cliente_fd, "Error preparando la consulta|", 30, 0);
        return rc;
    }

    sqlite3_bind_text(stmt, 1, nombre_usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        send(cliente_fd, "Error|Usuario ya existe o datos inválidos|", 44, 0);
        return rc;
    }

    send(cliente_fd, "Exito|Usuario registrado|", 26, 0);
    return SQLITE_OK;
}

int verificarUsuario(sqlite3 *db, const char *nombre_usuario, const char *contrasena, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM usuarios WHERE nombre_usuario = ? AND contrasena = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        send(cliente_fd, "Error|Problema con la base de datos|", 37, 0);
        return rc;
    }

    sqlite3_bind_text(stmt, 1, nombre_usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_ROW) {
        send(cliente_fd, "Exito|Login correcto|", 23, 0);
        return SQLITE_OK;
    }

    send(cliente_fd, "Error|Credenciales incorrectas|", 33, 0);
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
    if (crearTablaUsuarios(db) != SQLITE_OK) return -1;
    if (crearTablaVehiculos(db) != SQLITE_OK) return -1;
    if (crearTablaVentas(db) != SQLITE_OK) return -1;
    if (crearTablaPlantilla(db) != SQLITE_OK) return -1;
    if (crearTablaNotificaciones(db) != SQLITE_OK) return -1;
    return SQLITE_OK;
}


int crearTablaUsuarios(sqlite3* db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS usuarios (id INTEGER PRIMARY KEY AUTOINCREMENT, nombre_usuario TEXT UNIQUE, contrasena TEXT, email TEXT);";
    char *errMsg;
    return sqlite3_exec(db, sql, 0, 0, &errMsg);
}

int crearTablaVentas(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS ventas (id INTEGER PRIMARY KEY AUTOINCREMENT, id_usuario INTEGER, id_vehiculo INTEGER, precio_final REAL, fecha TIMESTAMP DEFAULT CURRENT_TIMESTAMP);";
    char *errMsg;
    return sqlite3_exec(db, sql, 0, 0, &errMsg);
}

int crearTablaVehiculos(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS vehiculos (id INTEGER PRIMARY KEY AUTOINCREMENT, marca TEXT, modelo TEXT, anio INTEGER, precio REAL);";
    char *errMsg;
    return sqlite3_exec(db, sql, 0, 0, &errMsg);
}

int crearTablaPlantilla(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS plantilla (id INTEGER PRIMARY KEY AUTOINCREMENT, marca TEXT, modelo TEXT, anio INTEGER, precio_sugerido REAL);";
    char *errMsg;
    return sqlite3_exec(db, sql, 0, 0, &errMsg);
}

int crearTablaNotificaciones(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS notificaciones (id INTEGER PRIMARY KEY AUTOINCREMENT, id_usuario INTEGER, mensaje TEXT, leido INTEGER DEFAULT 0);";
    char *errMsg;
    return sqlite3_exec(db, sql, 0, 0, &errMsg);
}


//-------------------- CONSULTA Y OPERACIÓN --------------------
int registrarVenta(sqlite3 *db, int id_usuario, int id_vehiculo, int precio_final) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO ventas (id_usuario, id_vehiculo, precio_final) VALUES (?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return -1;

    sqlite3_bind_int(stmt, 1, id_usuario);
    sqlite3_bind_int(stmt, 2, id_vehiculo);
    sqlite3_bind_int(stmt, 3, precio_final);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : rc;
}

int vehiculoExiste(sqlite3 *db, const char *marca, const char *modelo, int anio) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM vehiculos WHERE marca=? AND modelo=? AND anio=?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return -1;

    sqlite3_bind_text(stmt, 1, marca, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, modelo, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, anio);

    int rc = sqlite3_step(stmt);
    int count = (rc == SQLITE_ROW) ? sqlite3_column_int(stmt, 0) : -1;
    sqlite3_finalize(stmt);
    return count;
}

int mostrarPlantilla(sqlite3 *db, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT nombre, cargo, salario FROM plantilla;";
    char fila[256];
    char bufferGrande[4096] = {0};  // Para acumular todos los resultados

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        const char *err = sqlite3_errmsg(db);
        snprintf(fila, sizeof(fila), "Error al consultar la plantilla: %s\n", err);
        send(cliente_fd, fila, strlen(fila), 0);
        return -1;
    }

    int hayResultados = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *nombre = (const char *)sqlite3_column_text(stmt, 0);
        const char *cargo = (const char *)sqlite3_column_text(stmt, 1);
        double salario = sqlite3_column_double(stmt, 2);

        snprintf(fila, sizeof(fila), "Nombre: %s, Cargo: %s, Salario: %.2f\n", nombre, cargo, salario);
        strncat(bufferGrande, fila, sizeof(bufferGrande) - strlen(bufferGrande) - 1);
        hayResultados = 1;
    }

    sqlite3_finalize(stmt);

    if (!hayResultados) {
        strncat(bufferGrande, "No hay empleados en la plantilla.\n", sizeof(bufferGrande) - strlen(bufferGrande) - 1);
    }

    strncat(bufferGrande, "FIN_PLANTILLA\n", sizeof(bufferGrande) - strlen(bufferGrande) - 1);

    send(cliente_fd, bufferGrande, strlen(bufferGrande), 0);

    return SQLITE_OK;
}



float obtenerPrecioVehiculo(sqlite3 *db, const char *marca, const char *modelo) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT precio FROM vehiculos WHERE marca=? AND modelo=?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return -1;

    sqlite3_bind_text(stmt, 1, marca, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, modelo, -1, SQLITE_STATIC);

    float precio = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        precio = (float)sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return precio;
}

int buscarIDVehiculo(sqlite3 *db, const char *marca, const char *modelo, int anio) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM vehiculos WHERE marca=? AND modelo=? AND anio=?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return -1;

    sqlite3_bind_text(stmt, 1, marca, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, modelo, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, anio);

    int id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return id;
}

int insertarNotificacion(sqlite3 *db, int id_usuario, const char *mensaje) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO notificaciones (id_usuario, mensaje) VALUES (?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return -1;

    sqlite3_bind_int(stmt, 1, id_usuario);
    sqlite3_bind_text(stmt, 2, mensaje, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : rc;
}

void eliminarTodasLasNotificaciones(sqlite3 *db, int id_usuario, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM notificaciones WHERE id_usuario = ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        const char *errMsg = sqlite3_errmsg(db);
        char msg[256];
        snprintf(msg, sizeof(msg), "Error|Preparación fallida: %s\n", errMsg);
        send(cliente_fd, msg, strlen(msg), 0);
        return;
    }

    sqlite3_bind_int(stmt, 1, id_usuario);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        const char *errMsg = sqlite3_errmsg(db);
        char msg[256];
        snprintf(msg, sizeof(msg), "Error|Fallo al eliminar: %s\n", errMsg);
        send(cliente_fd, msg, strlen(msg), 0);
    } else {
        send(cliente_fd, "Exito|Notificaciones eliminadas\n", 33, 0);
    }

    sqlite3_finalize(stmt);
}


void eliminarNotificacionPorID(sqlite3 *db, int id_usuario, int id_notificacion, SOCKET cliente_fd) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM notificaciones WHERE id_usuario = ? AND id = ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        send(cliente_fd, "Error|No se pudo preparar la consulta|", 37, 0);
        return;
    }

    sqlite3_bind_int(stmt, 1, id_usuario);
    sqlite3_bind_int(stmt, 2, id_notificacion);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        send(cliente_fd, "Error|No se pudo eliminar la notificacion|", 42, 0);
    } else {
        send(cliente_fd, "Exito|Notificacion eliminada|", 29, 0);
    }
} 