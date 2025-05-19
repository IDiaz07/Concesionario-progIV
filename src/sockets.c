// sockets.c
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "sockets.h"  // Tu archivo de cabecera

void iniciar_sockets() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        fprintf(stderr, "Error al iniciar Winsock.\n");
        exit(1);
    }
}

void cerrar_sockets() {
    WSACleanup();
}

SOCKET crear_socket_servidor(const char* ip, int puerto) {
    SOCKET servidor_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor_fd == INVALID_SOCKET) {
        fprintf(stderr, "Error al crear socket: %d\n", WSAGetLastError());
        exit(1);
    }

    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = inet_addr(ip);
    direccion.sin_port = htons(puerto);

    if (bind(servidor_fd, (struct sockaddr*)&direccion, sizeof(direccion)) == SOCKET_ERROR) {
        fprintf(stderr, "Error en bind: %d\n", WSAGetLastError());
        closesocket(servidor_fd);
        exit(1);
    }

    if (listen(servidor_fd, 5) == SOCKET_ERROR) {
        fprintf(stderr, "Error en listen: %d\n", WSAGetLastError());
        closesocket(servidor_fd);
        exit(1);
    }

    return servidor_fd;
}

SOCKET aceptar_cliente(SOCKET servidor_fd) {
    struct sockaddr_in cliente_addr;
    int cliente_len = sizeof(cliente_addr);
    SOCKET cliente_fd = accept(servidor_fd, (struct sockaddr*)&cliente_addr, &cliente_len);
    if (cliente_fd == INVALID_SOCKET) {
        fprintf(stderr, "Error en accept: %d\n", WSAGetLastError());
    }
    return cliente_fd;
}

void cerrar_socket(SOCKET sockfd) {
    closesocket(sockfd);
}
 