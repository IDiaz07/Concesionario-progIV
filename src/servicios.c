#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "servicios.h"
#include "mantenimiento.h"
#include "file.h"
#include "vehiculos.h"
#include "database.h"

void mostrarMenuServicios(SOCKET cliente_fd) {
    const char* menu =
        "\nMenu Servicios:\n"
        "1. Mantenimiento y Reparacion\n"
        "2. Garantias\n"
        "3. Comprar\n"
        "4. Calificar Servicios\n"
        "5. Salir\n"
        "Selecciona una opcion: ";
    send(cliente_fd, menu, strlen(menu), 0);
}

// Menú principal de servicios (servidor)
void MenuServiciosRemoto(SOCKET cliente_fd) {
    FILE* archivo;
    inicializarArchivo(&archivo, cliente_fd);

    sqlite3 *db;
    if (abrirDB(&db) != SQLITE_OK) {
        send(cliente_fd, "Error al abrir la base de datos.\n", 34, 0);
        return;
    }

    char buffer[256];
    int opcion = 0;

    do {
        mostrarMenuServicios(cliente_fd);
        memset(buffer, 0, sizeof(buffer));
        recv(cliente_fd, buffer, sizeof(buffer), 0);
        opcion = atoi(buffer);

        switch (opcion) {
            case 1:
                mantenimientoYReparacionRemoto(cliente_fd);
                break;
            case 2:
                garantiasRemoto(cliente_fd);
                break;
            case 3:
                mostrarVehiculos(archivo, cliente_fd);
                ComprarVehiculoRemoto(db, cliente_fd);
                break;
            case 4:
                calificarServiciosRemoto(cliente_fd);
                break;
            case 5:
                send(cliente_fd, "Saliendo del menu de servicios...\n", 35, 0);
                break;
            default:
                send(cliente_fd, "Opcion no valida.\n", 19, 0);
                break;
        }

    } while (opcion != 5);

    fclose(archivo);
    sqlite3_close(db);
}

// Garantías por socket
void garantiasRemoto(SOCKET cliente_fd) {
    const char* mensaje =
        "Accion seleccionada: Garantias\n"
        "Mostrando informacion sobre garantias...\n"
        "- Todos nuestros vehículos tienen garantía de 2 años o 30,000 km.\n"
        "- Las reparaciones en concesionarios oficiales están cubiertas.\n";
    send(cliente_fd, mensaje, strlen(mensaje), 0);
}

// Calificación por socket
void calificarServiciosRemoto(SOCKET cliente_fd) {
    char buffer[256];
    int calificacion;
    char respuesta[512];

    send(cliente_fd, "Ingrese una calificacion (1-5): ", 32, 0);
    recv(cliente_fd, buffer, sizeof(buffer), 0);
    calificacion = atoi(buffer);

    if (calificacion < 1 || calificacion > 5) {
        send(cliente_fd, "Calificacion invalida. Debe estar entre 1 y 5.\n", 48, 0);
        return;
    }

    send(cliente_fd, "Ingrese un comentario sobre el servicio: ", 41, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(cliente_fd, buffer, sizeof(buffer), 0);

    snprintf(respuesta, sizeof(respuesta),
        "Gracias por su calificacion!\nValor: %d\nComentario: %s\n",
        calificacion, buffer);

    send(cliente_fd, respuesta, strlen(respuesta), 0);
}
