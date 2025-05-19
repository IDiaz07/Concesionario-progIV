// sockets.h
#ifndef SOCKETS_H
#define SOCKETS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <winsock2.h>

void iniciar_sockets();
void cerrar_sockets();
SOCKET crear_socket_servidor(const char* ip, int puerto);
SOCKET aceptar_cliente(SOCKET servidor_fd);
void cerrar_socket(SOCKET sockfd);

#ifdef __cplusplus
}
#endif

#endif
