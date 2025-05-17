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

    send(cliente_fd, "Nombre de usuario: ", 20, 0);
    recv(cliente_fd, buffer, sizeof(buffer), 0);
    sscanf("%s", nombre_usuario);
 
    send(cliente_fd, "Contrasenia: ", 13, 0);
    recv(cliente_fd, buffer, sizeof(buffer), 0);
    sscanf("%s", contrasena);

    send(cliente_fd, "Email: ", 13, 0);
    recv(cliente_fd, buffer, sizeof(buffer), 0);
    sscanf("%s", email);

    int rc = registrarUsuario(db, nombre_usuario, contrasena, email);
    if (rc == SQLITE_OK) {
        send(cliente_fd,"Usuario registrado exitosamente.\n",34,0);
    } else {
        send(cliente_fd,"Error al registrar el usuario.\n",32,0);
    }
}

int iniciarSesionMenu(sqlite3 *db, char *nombreUsuarioAutenticado, SOCKET cliente_fd) {
    char nombre_usuario[50];
    char contrasena[50];
    char buffer[256];

    send(cliente_fd, "Usuario: ", 9, 0);
    recv(cliente_fd, buffer, sizeof(buffer), 0);
    sscanf("%s", nombre_usuario);

    send(cliente_fd, "Contrase\xC3\xB1a: ", 13, 0);
    recv(cliente_fd, buffer, sizeof(buffer), 0);
    sscanf("%s", contrasena);

    int rc = verificarUsuario(db, nombre_usuario, contrasena);
    if (rc == SQLITE_OK) {
        send(cliente_fd, "Inicio de sesion exitoso.\n", 28, 0);
        
        strcpy(nombreUsuarioAutenticado, nombre_usuario);  // Copiar el nombre de usuario autenticado
        return 1;  // Retorna 1 si el inicio de sesión es exitoso
    } else {
       send(cliente_fd, "Inicio de sesion incorrecto.\n", 28, 0);
        return 0;  // Retorna 0 si el inicio de sesión falla
    }
}


