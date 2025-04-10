#include <stdio.h>
#include "sqlite3.h"
#include <string.h>
#include "database.h"
#include <locale.h> 

int abrirDB(sqlite3 **db) {
    int rc = sqlite3_open("db.db", db);
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
        printf("Error en la preparacion de la sentencia: %s\n", sqlite3_errmsg(db));
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

int registrarVenta(sqlite3 *db,int id_usuario, int id_vehiculo, int precio_final){
 sqlite3_stmt *stmt;
 const char *sql= "INSERT INTO ventas(id_usuario, id_vehiculo, precio_final) VALUES(?,?,?)";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error en la preparación de la sentencia: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt,1,id_usuario);
    sqlite3_bind_int(stmt,2,id_vehiculo);
    
    sqlite3_bind_int(stmt,3,precio_final);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Error al registrar usuario: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    sqlite3_finalize(stmt);
    printf("Gracias por comprar un vehiculo!!!");
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


int buscarIDUsuario(sqlite3 *db,const char *nombre_usuario ){
sqlite3_stmt *stmt;

int id = -1;
const char *sql= "SELECT id FROM usuarios WHERE nombre_usuario=?";
int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error en la preparación de la sentencia: %s\n", sqlite3_errmsg(db));
        return rc;
    }
     sqlite3_bind_text(stmt, 1, nombre_usuario, -1, SQLITE_STATIC);
     



     rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0); 
    } else {
        printf("Usuario no encontrado en la base de datos.\n");
    }

    sqlite3_finalize(stmt);
    return id;

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
int buscarIDVehiculo(sqlite3 *db, const char *marca, const char *modelo, int anio) { 
    sqlite3_stmt *stmt;
    int id = -1;  
    const char *sql = "SELECT id FROM vehiculos WHERE marca=? AND modelo=? AND anio=?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        printf("Error en la preparación de la sentencia: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, marca, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, modelo, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, anio);
    

   rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    } else if (rc == SQLITE_DONE) {
        printf("Vehículo no encontrado en la base de datos.\n");
    } else {
        printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return id;
}

int crearTablaPlantilla(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS plantilla ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "nombre TEXT NOT NULL, "
                      "cargo TEXT NOT NULL, "
                      "salario REAL NOT NULL);";

    char *errMsg;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al crear la tabla plantilla: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    return SQLITE_OK;
}

int mostrarPlantilla(sqlite3 *db) {
    setlocale(LC_NUMERIC, ""); // Habilitar separadores de miles

    sqlite3_stmt *stmt;
    const char *sql = "SELECT nombre, cargo, salario FROM plantilla;";

    // Preparar consulta
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    printf("\n--- Plantilla del Concesionario ---\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char *nombre = (const char *)sqlite3_column_text(stmt, 0);
        const char *cargo = (const char *)sqlite3_column_text(stmt, 1);
        double salario = sqlite3_column_double(stmt, 2);

        // Verificar si salario tiene un valor válido
        if (salario == 0) {
            printf("⚠ Advertencia: salario es 0 o no se ha leído correctamente.\n");
        }

        printf("Nombre: %s | Cargo: %s | Salario: %.2f\n",
               nombre ? nombre : "Desconocido",
               cargo ? cargo : "Desconocido",
               salario);
    }

    // Verificar si hubo errores en la ejecución
    if (rc != SQLITE_DONE) {
        printf("Error al recuperar datos: %s\n", sqlite3_errmsg(db));
    }

    // Liberar memoria
    sqlite3_finalize(stmt);

    return SQLITE_OK;
}

float obtenerPrecioVehiculo(sqlite3 *db, const char *marca, const char *modelo) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT precio FROM vehiculos WHERE marca = ? AND modelo = ?;";
    double precio = -1;  

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return precio;
    }

    
    sqlite3_bind_text(stmt, 1, marca, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, modelo, -1, SQLITE_STATIC);

    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        precio = sqlite3_column_double(stmt, 0); 
    } else {
        printf("Vehículo no encontrado en la base de datos.\n");
    }

    sqlite3_finalize(stmt);
    return precio;
}

int crearTablaNotificaciones(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS notificaciones ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "id_usuario INTEGER,"
                        "mensaje TEXT,"
                        "leido INTEGER DEFAULT 0,"
                        "fecha_hora TEXT,"
                        "FOREIGN KEY (id_usuario) REFERENCES usuarios(id)"
                        ");";

    char *errMsg;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al crear la tabla de notificaciones: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    return SQLITE_OK;
}


int insertarNotificacion(sqlite3 *db, int id_usuario, const char *mensaje) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO notificaciones (id_usuario, mensaje) VALUES (?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt, 1, id_usuario);
    sqlite3_bind_text(stmt, 2, mensaje, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Error insertando notificación: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return rc;
}

void mostrarNotificaciones(sqlite3 *db, int idUsuario) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT mensaje FROM notificaciones WHERE id_usuario = ? AND leido = 0;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, idUsuario);

    printf("\n--- Notificaciones No Leidas ---\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char *mensaje = (const char *)sqlite3_column_text(stmt, 0);
        printf("Mensaje: %s\n", mensaje);
    }

    if (rc != SQLITE_DONE) {
        printf("Error al recuperar las notificaciones: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void eliminarTodasLasNotificaciones(sqlite3 *db, int id_usuario) {
    char *errMsg = NULL;
    const char *sql = "DELETE FROM notificaciones WHERE id_usuario = ?;";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, id_usuario);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Error al eliminar las notificaciones del usuario %d: %s\n", id_usuario, sqlite3_errmsg(db));
    } else {
        printf("Todas las notificaciones del usuario %d han sido eliminadas.\n", id_usuario);
    }
    
    sqlite3_finalize(stmt);
}

void eliminarNotificacionPorID(sqlite3 *db, int id_usuario, int id_notificacion) {
    char sql[200];
    snprintf(sql, sizeof(sql), "DELETE FROM notificaciones WHERE id = ? AND id_usuario = ?;");
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id_notificacion);
    sqlite3_bind_int(stmt, 2, id_usuario);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Error al eliminar la notificación con ID %d para el usuario %d: %s\n", id_notificacion, id_usuario, sqlite3_errmsg(db));
    } else {
        printf("Notificacion con ID %d eliminada correctamente para el usuario %d.\n", id_notificacion, id_usuario);
    }

    sqlite3_finalize(stmt);
}
