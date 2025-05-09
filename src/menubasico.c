#include <stdio.h>
#include <string.h>
#include "menubasico.h"
#include "file.h"
#include "servicios.h"
#include "database.h"
#include "contacto.h"
#include "notificaciones.h"

void mostrarMenu() {
    printf("\nMenu DeustoMotors:\n");
    printf("1. Mostrar Vehiculos\n");
    printf("2. Servicios\n");
    printf("3. Prueba de Manejo\n");
    printf("4. Contacto\n");
    printf("5. Notificaciones\n");
    printf("6. Salir\n");
}


int seleccionarOpcion() {
    int opcion;
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

void menuBasico(sqlite3 *db, int idUsuario){
    // Menú básico para otros usuarios
    FILE* archivo;
    inicializarArchivo(&archivo);
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
                // Implementar metodo PruebaManejo();
                break;
            case 4:
                mostrarContacto();
                break;
            case 5:
                mostrarNotificaciones(db, idUsuario);
                menuNoti(db);
                break;
            case 6:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while (opcionConcesionario != 6);
}


