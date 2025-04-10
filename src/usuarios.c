#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuarios.h"
#include "database.h"

void registrarUsuarioMenu(sqlite3 *db) {
    char nombre_usuario[50];
    char contrasena[50];
    char email[100];

    printf("Introduce el nombre de usuario: ");
    scanf("%s", nombre_usuario);

    printf("Introduce la contrasenia: ");
    scanf("%s", contrasena);

    printf("Introduce el email: ");
    scanf("%s", email);

    int rc = registrarUsuario(db, nombre_usuario, contrasena, email);
    if (rc == SQLITE_OK) {
        printf("Usuario registrado exitosamente.\n");
    } else {
        printf("Error al registrar el usuario.\n");
    }
}

int iniciarSesionMenu(sqlite3 *db, char *nombreUsuarioAutenticado) {
    char nombre_usuario[50];
    char contrasena[50];

    printf("Introduce el nombre de usuario: ");
    scanf("%s", nombre_usuario);

    printf("Introduce la contrasena: ");
    scanf("%s", contrasena);

    int rc = verificarUsuario(db, nombre_usuario, contrasena);
    if (rc == SQLITE_OK) {
        printf("Inicio de sesion exitoso.\n");
        strcpy(nombreUsuarioAutenticado, nombre_usuario);  // Copiar el nombre de usuario autenticado
        return 1;  // Retorna 1 si el inicio de sesión es exitoso
    } else {
        printf("Nombre de usuario o contrasenia incorrectos.\n");
        return 0;  // Retorna 0 si el inicio de sesión falla
    }
}


