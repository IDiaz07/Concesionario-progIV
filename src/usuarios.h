#ifndef USUARIOS_H
#define USUARIOS_H

#include "sqlite3.h"
#include "sockets.h"

void registrarUsuarioMenu(sqlite3 *db, SOCKET cliente_fd);
int iniciarSesionMenu(sqlite3 *db, char *nombreUsuarioAutenticado, SOCKET cliente_fd);

#endif
