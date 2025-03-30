#ifndef USUARIOS_H
#define USUARIOS_H

#include "sqlite3.h"

void registrarUsuarioMenu(sqlite3 *db);
int iniciarSesionMenu(sqlite3 *db, char *nombreUsuarioAutenticado);

#endif
