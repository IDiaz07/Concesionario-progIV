#ifndef NOTIFICACIONES_H
#define NOTIFICACIONES_H

#include "sqlite3.h"

void mostrarNotificaciones(sqlite3 *db, int id_usuario);
void menuNoti(sqlite3 *db, int id_usuario);

#endif