#ifndef NOTIFICACIONES_H
#define NOTIFICACIONES_H

#include "sqlite3.h"
#include <winsock2.h>

void mostrarNotificaciones(sqlite3 *db, int id_usuario, SOCKET cliente_fd);
void menuNoti(sqlite3 *db, int id_usuario, SOCKET cliente_fd);

#endif