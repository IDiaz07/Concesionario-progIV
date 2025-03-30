#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

int abrirDB(sqlite3** db);
int crearTablaUsuarios(sqlite3* db);
int registrarUsuario(sqlite3* db, const char* nombre_usuario, const char* contrasena, const char* email);
int verificarUsuario(sqlite3* db, const char* nombre_usuario, const char* contrasena);
int crearTablaVentas(sqlite3 *db);
int registrarVenta(sqlite3 *db,int id_usuario, int id_vehiculo, const char *fecha, double precio_final);
int crearTablaVehiculos(sqlite3 *db);
int registrarVehiculo(sqlite3 *db,char *marca, char *modelo, int anio, int precio);
#endif
