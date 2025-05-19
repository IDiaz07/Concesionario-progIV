// servidor.c
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "database.h"
#include "file.h"

#pragma comment(lib, "ws2_32.lib")

#define PUERTO 12345
#define BUFFER_SIZE 1024

void manejarCliente(SOCKET cliente_fd, sqlite3* db) {
    char buffer[BUFFER_SIZE];
    char nombreUsuario[50];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(cliente_fd, buffer, sizeof(buffer), 0);

        if (strncmp(buffer, "LOGIN", 5) == 0) {
            char user[50], pass[50];
            sscanf(buffer, "LOGIN %s %s", user, pass);
            int ok = verificarUsuario(db, user, pass);
            if (ok == SQLITE_OK) {
                strcpy(nombreUsuario, user);
                send(cliente_fd, "Inicio de sesion exitoso.\n", 28, 0);
            } else {
                send(cliente_fd, "Inicio de sesion incorrecto.\n", 30, 0);
            }
        } else if (strncmp(buffer, "LISTAR", 6) == 0) {
            FILE* archivo;
            inicializarArchivo(&archivo, cliente_fd);
            mostrarVehiculos(archivo, cliente_fd);
            fclose(archivo);
        } else if (strncmp(buffer, "SALIR", 5) == 0) {
            send(cliente_fd, "Desconectando...\n", 18, 0);
            break;
        } else {
            send(cliente_fd, "Comando no reconocido.\n", 24, 0);
        }
    }
}

int main() {
    WSADATA wsa;
    SOCKET servidor_fd, cliente_fd;
    struct sockaddr_in direccion;
    sqlite3 *db;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    if (abrirDB(&db) != SQLITE_OK) {
        printf("No se pudo abrir la base de datos.\n");
        return 1;
    }

    servidor_fd = socket(AF_INET, SOCK_STREAM, 0);
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(PUERTO);

    bind(servidor_fd, (struct sockaddr*)&direccion, sizeof(direccion));
    listen(servidor_fd, 1);

    printf("Servidor escuchando en puerto %d...\n", PUERTO);
    cliente_fd = accept(servidor_fd, NULL, NULL);
    printf("Cliente conectado.\n");

    manejarCliente(cliente_fd, db);

    closesocket(cliente_fd);
    closesocket(servidor_fd);
    WSACleanup();
    return 0;
}
