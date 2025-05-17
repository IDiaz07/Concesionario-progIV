#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "database.h"
#include "menubasico.h"

#pragma comment(lib, "ws2_32.lib")  // Solo si usas Visual Studio

#define PUERTO 12345
#define BUFFER_SIZE 1024
/*
void manejarCliente(SOCKET cliente_fd, sqlite3* db) {
    char buffer[1024];
    char nombreUsuario[50];
    int idUsuario;

    while (1) {
        // Menú principal
        send(cliente_fd, "Menu Principal:\n1. Registro\n2. Iniciar sesion\n3. Salir\nSeleccione una opcion: ", 95, 0);
        recv(cliente_fd, buffer, sizeof(buffer), 0);

        if (strncmp(buffer, "1", 1) == 0) {
            registrarUsuarioMenuRemoto(db, cliente_fd);
        }
        else if (strncmp(buffer, "2", 1) == 0) {
            iniciarSesionMenuRemoto(db, cliente_fd, nombreUsuario);

            idUsuario = buscarIDUsuario(db, nombreUsuario);

            if (strcmp(nombreUsuario, "DeustoMotors") == 0) {
                menuAdministrativoRemoto(db, cliente_fd);
            } else {
                menuBasico(db, idUsuario);
            }
        }
        else if (strncmp(buffer, "3", 1) == 0) {
            send(cliente_fd, "Saliendo del programa...\n", 26, 0);
            break;
        }
        else {
            send(cliente_fd, "Opcion no valida.\n", 19, 0);
        }
    }
}

*/
int main() {
    WSADATA wsa;
    SOCKET servidor_fd, cliente_fd;
    struct sockaddr_in direccion;
    char buffer[BUFFER_SIZE];
    sqlite3 *db;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error inicializando Winsock.\n");
        return 1;
    }

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
    
     //manejarCliente(cliente_fd, db);
    
}
