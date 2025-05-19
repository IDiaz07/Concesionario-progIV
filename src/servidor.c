// servidor.c
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "menubasico.h"
#include "database.h"

#pragma comment(lib, "ws2_32.lib")

#define PUERTO 12345
#define BUFFER_SIZE 1024

void manejarCliente(SOCKET cliente_fd, sqlite3* db) {
    char buffer[BUFFER_SIZE];
    char nombreUsuario[50];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesRecibidos <= 0) {
            printf("Cliente desconectado o error en recv.\n");
            break;
        }
        buffer[bytesRecibidos] = '\0';

        if (strncmp(buffer, "REGISTRAR", 9) == 0) {
            char user[50], pass[50], email[100];
            int args = sscanf(buffer, "REGISTRAR %s %s %s", user, pass, email);
            if (args == 3) {
                int rc = registrarUsuario(db, user, pass, email, cliente_fd);
                if (rc == SQLITE_OK) {
                    send(cliente_fd, "Exito|Usuario registrado\n", 24, 0);
                } else {
                send(cliente_fd, "Error|No se pudo registrar usuario\n", 35, 0);
            }
            } else {
                send(cliente_fd, "Error|Formato REGISTRAR incorrecto\n", 35, 0);
            }
        } else if (strncmp(buffer, "LOGIN", 5) == 0) {
            char user[50], pass[50];
            int args = sscanf(buffer, "LOGIN %s %s", user, pass);
            if (args == 2) {
                int rc = verificarUsuario(db, user, pass, cliente_fd);
                if (rc == SQLITE_OK) {
                    int idUsuario = buscarIDUsuario(db, user, cliente_fd); // extrae ID del usuario
                    if (idUsuario != -1) {
                        send(cliente_fd, "Exito|Login correcto|\n", 24, 0);
                        Sleep(100);
                        menuBasico(db, user, idUsuario, cliente_fd);
                    }
                } else {
                    send(cliente_fd, "Error|Credenciales incorrectas\n", 33, 0);
                }
            }
        } else if (strncmp(buffer, "SALIR", 5) == 0) {
            send(cliente_fd, "Desconectando...\n", 18, 0);
            break;
        } else {
            send(cliente_fd, "Error|Comando no reconocido\n", 28, 0);
        }
    }
}

int crearTablas(sqlite3* db);

int main() {
    WSADATA wsa;
    SOCKET servidor_fd, cliente_fd;
    struct sockaddr_in direccion;
    sqlite3 *db;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error iniciando Winsock\n");
        return 1;
    }

    if (abrirDB(&db) != SQLITE_OK) {
        printf("No se pudo abrir la base de datos.\n");
        WSACleanup();
        return 1;
    }

    if (crearTablas(db) != SQLITE_OK) {
        printf("Error creando tablas en la base de datos.\n");
        sqlite3_close(db);
        WSACleanup();
        return 1;
    }

    servidor_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor_fd == INVALID_SOCKET) {
        printf("Error creando socket servidor.\n");
        sqlite3_close(db);
        WSACleanup();
        return 1;
    }

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(PUERTO);

    if (bind(servidor_fd, (struct sockaddr*)&direccion, sizeof(direccion)) == SOCKET_ERROR) {
        printf("Error en bind.\n");
        closesocket(servidor_fd);
        sqlite3_close(db);
        WSACleanup();
        return 1;
    }

    if (listen(servidor_fd, 1) == SOCKET_ERROR) {
        printf("Error en listen.\n");
        closesocket(servidor_fd);
        sqlite3_close(db);
        WSACleanup();
        return 1;
    }

    printf("Servidor escuchando en puerto %d...\n", PUERTO);

    cliente_fd = accept(servidor_fd, NULL, NULL);
    if (cliente_fd == INVALID_SOCKET) {
        printf("Error en accept.\n");
        closesocket(servidor_fd);
        sqlite3_close(db);
        WSACleanup();
        return 1;
    }

    printf("Cliente conectado.\n");

    manejarCliente(cliente_fd, db);

    closesocket(cliente_fd);
    closesocket(servidor_fd);
    sqlite3_close(db);
    WSACleanup();

    return 0;
}
