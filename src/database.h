#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

int abrirDB(sqlite3** db);
int crearTablaUsuarios(sqlite3* db);
int registrarUsuario(sqlite3* db, const char* nombre_usuario, const char* contrasena, const char* email);
int verificarUsuario(sqlite3* db, const char* nombre_usuario, const char* contrasena);

#endif
