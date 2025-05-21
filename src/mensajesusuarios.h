#ifndef MENSAJESUSUARIOS_H
#define MENSAJESUSUARIOS_H

#include "sqlite3.h"

void enviarMensajeAUsuarios(sqlite3 *db, SOCKET cliente_fd);

#endif
