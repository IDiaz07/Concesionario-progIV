#ifndef USUARIOS_H
#define USUARIOS_H

#include "sqlite3.h"

void registrarUsuarioMenu(sqlite3 *db);
void iniciarSesionMenu(sqlite3 *db);

#endif
