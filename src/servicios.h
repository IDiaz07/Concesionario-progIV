#ifndef SERVICIOS_H
#define SERVICIOS_H

#include <stdio.h>
#include "sqlite3.h"
#include "sockets.h"

void mantenimientoYReparacionRemoto(sqlite3 *db, SOCKET cliente_fd);
void ComprarVehiculoRemoto(sqlite3 *db, SOCKET cliente_fd);
void garantiasRemoto(SOCKET cliente_fd);
void calificarServiciosRemoto(SOCKET cliente_fd);


void mostrarMenuServicios(SOCKET cliente_fd);
void MenuServicios(sqlite3 *db, const char *usuario, SOCKET cliente_fd);


#endif
