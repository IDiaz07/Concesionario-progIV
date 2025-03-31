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


int crearTablaVehiculos(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS vehiculos ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "marca TEXT NOT NULL, "
                      "modelo TEXT NOT NULL, "
                      "anio INTEGER NOT NULL, "
                      "precio REAL NOT NULL "
                     ");";

    char *errMsg;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al crear la tabla de vehiculos: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    return SQLITE_OK;
}
int registrarVehiculo(sqlite3 *db, char *marca, char *modelo, int anio, int precio) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO vehiculos (marca, modelo, anio, precio) VALUES (?, ?, ?, ?)";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error en la preparación de la sentencia: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_text(stmt, 1, marca, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, modelo, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, anio);  
    sqlite3_bind_int(stmt, 4, precio);
   

    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Error al registrar vehículo: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    sqlite3_finalize(stmt);

     printf("Vehiculo registrado con existo");
    return SQLITE_OK;
   
}

int crearTablaVentas(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS ventas ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "id_usuario INTEGER NOT NULL, "
                      "id_vehiculo INTEGER NOT NULL, "
                      "fecha TEXT NOT NULL, "
                      "precio_final REAL NOT NULL, "
                      "FOREIGN KEY (id_usuario) REFERENCES usuarios(id), "
                      "FOREIGN KEY (id_vehiculo) REFERENCES vehiculos(id));";

    char *errMsg;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al crear la tabla de ventas: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    return SQLITE_OK;
}

int registrarVenta(sqlite3 *db,int id_usuario, int id_vehiculo, const char *fecha, double precio_final){
 sqlite3_stmt *stmt;
 const char *sql= "INSERT INTO ventas(id_usuario, id_vehiculo, fecha, precio_final) VALUES(?,?,?,?)";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error en la preparación de la sentencia: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt,1,id_usuario);
    sqlite3_bind_int(stmt,2,id_vehiculo);
    sqlite3_bind_text(stmt,3,fecha,-1, SQLITE_STATIC);
    sqlite3_bind_double(stmt,4,precio_final);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Error al registrar usuario: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    sqlite3_finalize(stmt);
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
    return SQLITE_ERROR; 
}


int vehiculoExiste(sqlite3 *db, const char *marca, const char *modelo, int anio) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM vehiculos WHERE marca = ? AND modelo = ? AND anio = ?";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error en la preparación de la sentencia: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    
    sqlite3_bind_text(stmt, 1, marca, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, modelo, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, anio);

    
    rc = sqlite3_step(stmt);
    int existe = 0;
    if (rc == SQLITE_ROW) {
        existe = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return existe > 0; 
}
