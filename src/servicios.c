#include <stdio.h>
#include "servicios.h"
#include "mantenimiento.h"  // Asegúrate de incluir este archivo
#include "file.h"
#include "vehiculos.h"

// Mostrar el menú de servicios de DeustoMotors
void mostrarMenuServicios() {
    printf("\nMenu Servicios:\n");
    printf("1. Mantenimiento y Reparacion\n");
    printf("2. Garantias\n");
    printf("3. Comprar \n");
    printf("4. Calificar Servicios\n");
    printf("5. Salir\n");
}

// Seleccionar la opción en el menú
int MenuServicios() {

    FILE* archivo;
   
    inicializarArchivo(&archivo);

    sqlite3 *db;
    
    int rc = abrirDB(&db);
    if (rc != SQLITE_OK) {
        printf("No se pudo abrir la base de datos.\n");
        return 1;
    }
    int opcion;
    do {
        mostrarMenuServicios();
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion); // Leer la opción seleccionada

        switch (opcion) {
        case 1:
            mantenimientoYReparacion();  // Llamada a la función de mantenimiento
            break;
        case 2:
            garantias();
            break;

        case 3:
            mostrarVehiculos(archivo);
            ComprarVehiculo(db);
            break;
        case 4:
            calificarServicios();
            break;
        case 5:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opcion no valida.\n");
        }
    } while (opcion != 5);

    fclose(archivo);
    sqlite3_close(db);
}

// Método para la opción de Garantías
void garantias() {
    printf("Accion seleccionada: Garantias\n");
    printf("Mostrando informacion sobre garantias...\n");
}

//Metodo calificar servicios
void calificarServicios() {
    int calificacion;
    char comentario[255];
    
    printf("\nIngrese una calificacion (1-5): ");
    scanf("%d", &calificacion);
    
    if (calificacion < 1 || calificacion > 5) {
        printf("Calificacion invalida. Debe estar entre 1 y 5.\n");
        return;
    }
    
    printf("Ingrese un comentario sobre el servicio: ");
    getchar(); // Limpiar buffer de entrada
    fgets(comentario, sizeof(comentario), stdin);
    
    printf("Gracias por su calificacion! Valor: %d\nComentario: %s\n", calificacion, comentario);
}
