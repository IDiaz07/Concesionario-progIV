#ifndef MENUADMINISTRATIVO_H
#define MENUADMINISTRATIVO_H

#include "sqlite3.h"
#include "sockets.h"

void menuAdministrativo(sqlite3 *db, SOCKET cliente_fd);

#endif