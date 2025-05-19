#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuarios.h"
#include "database.h"
#include <winsock2.h>

void registrarUsuarioMenu(sqlite3 *db, SOCKET cliente_fd) {
    char nombre_usuario[50];
    char contrasena[50];
    char email[100];
    char buffer[256];
    int bytesRecibidos;

    send(cliente_fd, "Nombre de usuario: ", strlen("Nombre de usuario: "), 0);
    bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecibidos <= 0) return;  // Manejo básico de error
    buffer[bytesRecibidos] = '\0';     // Terminar cadena correctamente
    sscanf(buffer, "%49s", nombre_usuario);

    send(cliente_fd, "Contraseña: ", strlen("Contraseña: "), 0);
    bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecibidos <= 0) return;
    buffer[bytesRecibidos] = '\0';
    sscanf(buffer, "%49s", contrasena);

    send(cliente_fd, "Email: ", strlen("Email: "), 0);
    bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecibidos <= 0) return;
    buffer[bytesRecibidos] = '\0';
    sscanf(buffer, "%99s", email);

    int rc = registrarUsuario(db, nombre_usuario, contrasena, email, cliente_fd);
    if (rc == SQLITE_OK) {
        send(cliente_fd, "Usuario registrado exitosamente.\n", strlen("Usuario registrado exitosamente.\n"), 0);
    } else {
        send(cliente_fd, "Error al registrar el usuario.\n", strlen("Error al registrar el usuario.\n"), 0);
    }
}

int iniciarSesionMenu(sqlite3 *db, char *nombreUsuarioAutenticado, SOCKET cliente_fd) {
    char nombre_usuario[50];
    char contrasena[50];
    char buffer[256];
    int bytesRecibidos;

    send(cliente_fd, "Usuario: ", strlen("Usuario: "), 0);
    bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecibidos <= 0) return 0;
    buffer[bytesRecibidos] = '\0';
    sscanf(buffer, "%49s", nombre_usuario);

    send(cliente_fd, "Contraseña: ", strlen("Contraseña: "), 0);
    bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecibidos <= 0) return 0;
    buffer[bytesRecibidos] = '\0';
    sscanf(buffer, "%49s", contrasena);

    int rc = verificarUsuario(db, nombre_usuario, contrasena, cliente_fd);
    if (rc == SQLITE_OK) {
        send(cliente_fd, "Inicio de sesion exitoso.\n", strlen("Inicio de sesion exitoso.\n"), 0);
        strcpy(nombreUsuarioAutenticado, nombre_usuario);
        return 1;
    } else {
        send(cliente_fd, "Inicio de sesion incorrecto.\n", strlen("Inicio de sesion incorrecto.\n"), 0);
        return 0;
    }
}
