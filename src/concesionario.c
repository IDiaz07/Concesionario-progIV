#include <stdio.h>
#include <stdlib.h>
#include "concesionario.h"
#include "file.h"
#include "vehiculos.h"
#include "database.h"
#include "usuarios.h" 

void mostrarMenu() {
    printf("\nMenu:\n");
    printf("1. Aniadir vehiculo\n");
    printf("2. Mostrar vehiculos\n");
    printf("3. Exportar a archivo\n");
    printf("4. Salir\n");
}

void MostrarFiltro(){
        printf("1. Filtrar por marca\n");
        printf("2. Filtrar por dinero\n");
        printf("3. Filtrar por modelo\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");
       

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


int seleccionarOpcionFiltro(){
    int opcion;
    
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
    FILE* archivo;
    inicializarArchivo(&archivo);

    int opcionFiltro;
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
                iniciarSesionMenu(db);  // Usar la función definida en usuarios.c
                do {
                    mostrarMenu();
                    opcionConcesionario = seleccionarOpcion();


                    switch (opcionConcesionario) {
                        case 1:
                            anadirVehiculo(archivo);
                            break;
                        case 2:
                            mostrarVehiculos(archivo);
                            MostrarFiltro();

                             opcionFiltro = seleccionarOpcionFiltro();

                            switch (opcionFiltro)
                            {
                            case 1:
                            FiltrarMarca(archivo);
                                break;
                            
                            default:
                            printf("Opción no válida.\n");
                                break;
                            }
                            break;
                        case 3:
                            exportarAFichero(archivo, "vehiculos.csv");
                            break;
                        case 4:
                            printf("Saliendo del programa...\n");
                            break;
                        default:
                            printf("Opción no válida.\n");
                    }
                } while (opcionConcesionario != 4);
                break;
            }
            case 3:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }

    } while (opcion != 3);

    fclose(archivo);
    sqlite3_close(db);
    return 0;
}