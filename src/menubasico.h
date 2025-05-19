#ifndef MENUBASICO_H
#define MENUBASICO_H

#include "sqlite3.h"
#include "sockets.h"

void menuBasico(sqlite3 *db, const char *usuario, int idUsuario, SOCKET cliente_fd);
void MenuServicios(sqlite3 *db, const char *usuario, SOCKET cliente_fd);

#endif