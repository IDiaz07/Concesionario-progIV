#include <stdio.h>
#include "sqlite3.h"
#include "../sqlite/sqlite3.h"

void inicializarBaseDeDatos(sqlite3** db) {
    if (sqlite3_open("concesionario.db", db) != SQLITE_OK) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        exit(1);
    }

    const char* sqlCrearTabla = 
        "CREATE TABLE IF NOT EXISTS Vehiculos (" 
        "id INTEGER PRIMARY KEY AUTOINCREMENT," 
        "marca TEXT NOT NULL," 
        "modelo TEXT NOT NULL," 
        "anio INTEGER NOT NULL," 
        "precio REAL NOT NULL);";

    char* mensajeError = NULL;
    if (sqlite3_exec(*db, sqlCrearTabla, 0, 0, &mensajeError) != SQLITE_OK) {
        printf("Error al crear la tabla: %s\n", mensajeError);
        sqlite3_free(mensajeError);
        exit(1);
    }
}
