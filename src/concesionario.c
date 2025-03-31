#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "concesionario.h"
#include "file.h"
#include "vehiculos.h"
#include "database.h"
#include "usuarios.h"
#include "servicios.h"
#include "menubasico.h"
#include "menuadministrativo.h"


char nombreUsuarioAutenticado[50] = "";


void mostrarMenuPrincipal() {
    printf("\nMenu Principal:\n");
    printf("1. Registro de usuario\n");
    printf("2. Iniciar sesion\n");
    printf("3. Salir\n");
}


int seleccionarOpcionPrincipal() {
    int opcion;
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}


int main() {
    sqlite3 *db;
    
    int rc = abrirDB(&db);
    if (rc != SQLITE_OK) {
        printf("No se pudo abrir la base de datos.\n");
        return 1;
    }


    rc = crearTablaUsuarios(db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    rc = crearTablaPlantilla(db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }


    int opcionDeustoMotors;
    FILE* archivo;
    cargarVehiculosDesdeArchivo(db,archivo);
    inicializarArchivo(&archivo);


    int opcion;
    do {
        mostrarMenuPrincipal();
        opcion = seleccionarOpcionPrincipal();


        switch (opcion) {
            case 1: {
                registrarUsuarioMenu(db);  // Usar la función definida en usuarios.c
                break;
            }
            case 2: {
                if (iniciarSesionMenu(db, nombreUsuarioAutenticado)) {
                    printf("Bienvenido, %s!\n", nombreUsuarioAutenticado);

                    if (strcmp(nombreUsuarioAutenticado, "DeustoMotors") == 0) {
                        menuAdministrativo(db);
                    } else {
                        menuBasico();
                    }
                } else {
                    printf("Inicio de sesion fallido. Por favor, intente nuevamente.\n");
                }
                break;
            }
            case 3:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida.\n");
                break;
        }


    } while (opcion != 3);


    fclose(archivo);
    sqlite3_close(db);
    return 0;
}
