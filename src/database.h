#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include "sockets.h"

// Funciones de conexión a la base de datos
int abrirDB(sqlite3 **db);

// Usuarios
int registrarUsuario(sqlite3 *db, const char *nombre_usuario, const char *contrasena, const char *email, SOCKET cliente_fd);
int verificarUsuario(sqlite3 *db, const char *nombre_usuario, const char *contrasena, SOCKET cliente_fd);

// Vehículos
int registrarVehiculo(sqlite3 *db, char *marca, char *modelo, int anio, int precio, SOCKET cliente_fd);
void listarVehiculos(sqlite3 *db, SOCKET cliente_fd);

// Notificaciones
void mostrarNotificaciones(sqlite3 *db, int idUsuario, SOCKET cliente_fd);

// Utilidades
int buscarIDUsuario(sqlite3 *db, const char *nombre_usuario, SOCKET cliente_fd);

// Inicialización
int crearTablas(sqlite3 *db);
int crearTablaUsuarios(sqlite3* db);
int crearTablaVentas(sqlite3 *db);
int registrarVenta(sqlite3 *db,int id_usuario, int id_vehiculo, int precio_final);
int crearTablaVehiculos(sqlite3 *db);
int crearTablaCitas(sqlite3 *db);

int vehiculoExiste(sqlite3 *db, const char *marca, const char *modelo, int anio);

int crearTablaPlantilla(sqlite3 *db);
int mostrarPlantilla(sqlite3 *db, SOCKET cliente_fd);
float obtenerPrecioVehiculo(sqlite3 *db, const char *marca, const char *modelo);
int buscarIDVehiculo(sqlite3 *db,const char *marca, const char *modelo, int anio);

int crearTablaNotificaciones(sqlite3 *db);
int insertarNotificacion(sqlite3 *db, int id_usuario, const char *mensaje);

void eliminarTodasLasNotificaciones(sqlite3 *db, int id_usuario, SOCKET cliente_fd);
void eliminarNotificacionPorID(sqlite3 *db, int id_usuario, int id_notificacion, SOCKET cliente_fd);


#endif
