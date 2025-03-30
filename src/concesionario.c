#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "concesionario.h"
#include "file.h"
#include "vehiculos.h"
#include "database.h"
#include "usuarios.h"
#include "servicios.h"

char nombreUsuarioAutenticado[50] = "";

void mostrarMenuDeustoMotors() {
    printf("\nMenu Administrativo:\n");
    printf("1. Enviar mensaje a los usuarios\n");
    printf("2. Ver todos los vehiculos vendidos\n");
    printf("3. Aniadir vehiculo\n");
    printf("4. Mirar Plantilla\n");
    printf("5. Salir\n");
}

int seleccionarOpcionDeustoMotors() {
    int opcion;
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

void mostrarMenu() {
    printf("\nMenu DeustoMotors:\n");
    printf("1. Mostrar Vehiculos\n");
    printf("2. Servicios\n");
    printf("3. Prueba de Manejo\n");
    printf("4. Contacto\n");
    printf("5. Salir\n");
}

int seleccionarOpcion() {
    int opcion;
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

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

    int opcionConcesionario;
    int opcionDeustoMotors;
    FILE* archivo;
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
                        // Menú especial para DeustoMotors
                        int opcionDeustoMotors;
                        do {
                            mostrarMenuDeustoMotors();
                            opcionDeustoMotors = seleccionarOpcionDeustoMotors();

                            switch (opcionDeustoMotors) {
                                case 1:
                                    // Implementar enviarMensajeAUsuarios()
                                    break;
                                case 2:
                                    // Implementar verVehiculosVendidos()
                                    break;
                                case 3:
                                    anadirVehiculo(archivo);
                                    break;
                                case 4:
                                    // Implementacion Plantilla
                                    break;
                                case 5:
                                    printf("Saliendo del programa...\n");
                                    break;
                                default:
                                    printf("Opción no válida.\n");
                            }
                        } while (opcionDeustoMotors != 5);
                    } else {
                        // Menú básico para otros usuarios
                        int opcionConcesionario;
                        do {
                            mostrarMenu();
                            opcionConcesionario = seleccionarOpcion();

                            switch (opcionConcesionario) {
                                case 1:{
                                int subopcion;
                                    printf("\n1. Ver todos los vehiculos\n");
                                    printf("2. Busqueda avanzada\n");
                                    printf("Elige una opcion: ");
                                    scanf("%d", &subopcion);
                                    if (subopcion == 1) {
                                        mostrarVehiculos(archivo);  
                                    } else if (subopcion == 2) {
                                        filtrarVehiculos(archivo);  
                                    } else {
                                        printf("Opcion no valida.\n");
                                    }
                                    break;}
                                case 2:
                                    MenuServicios();
                                    break;
                                case 3:
                                    // Implementar metodo
                                    break;
                                case 4:
                                    // Implementar metodo
                                    break;
                                case 5:
                                    printf("Saliendo del programa...\n");
                                    break;
                                default:
                                    printf("Opcion no valida.\n");
                            }
                        } while (opcionConcesionario != 5);
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