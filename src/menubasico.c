#include <stdio.h>
#include <string.h>
#include "menubasico.h"
#include "file.h"
#include "servicios.h"
#include "database.h"
#include "contacto.h"
#include "notificaciones.h"
#include "pruebamanejo.h"

void menuBasico(sqlite3 *db, const char *usuario, int idUsuario, SOCKET cliente_fd) {
    FILE* archivo;
    inicializarArchivo(&archivo, cliente_fd);
    char buffer[256];
    int opcionConcesionario;



    do {
        // Enviar menú al cliente
        const char *menu =
            "\n=== Menu DeustoMotors ===\n"
            "1. Mostrar Vehiculos\n"
            "2. Servicios\n"
            "3. Prueba de Manejo\n"
            "4. Contacto\n"
            "5. Notificaciones\n"
            "6. Salir\n"
            "Seleccione una opcion: ";
        send(cliente_fd, menu, strlen(menu), 0);

        // Recibir opción del cliente
        memset(buffer, 0, sizeof(buffer));
        int bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesRecibidos <= 0) {
            break; // desconectado o error
        }

        opcionConcesionario = atoi(buffer);

        switch (opcionConcesionario) {
            case 1: {
                const char *sub =
                    "\n1. Ver todos los vehiculos\n"
                    "2. Busqueda avanzada\n"
                    "Elige una opcion: ";
                send(cliente_fd, sub, strlen(sub), 0);

                memset(buffer, 0, sizeof(buffer));
                int bytes = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
                int subopcion = atoi(buffer);

                if (subopcion == 1) {
                    mostrarVehiculos(archivo, cliente_fd);
                } else if (subopcion == 2) {
                    filtrarVehiculos(db, cliente_fd);

                } else {
                    const char *msg = "Opcion no valida.\n";
                    send(cliente_fd, msg, strlen(msg), 0);
                }
                break;
            }
            case 2:
                MenuServicios(db, usuario, cliente_fd);
                break;
            case 3:
                 pruebaManejo(db, cliente_fd, usuario);
                break;
            case 4:
                mostrarContacto(cliente_fd);
                break;
            case 5:
                mostrarNotificaciones(db, idUsuario, cliente_fd);
                menuNoti(db, idUsuario, cliente_fd);
                break;
            case 6:
                send(cliente_fd, "Saliendo del menu DeustoMotors...\n", 36, 0);
                break;
            default:
                send(cliente_fd, "Opcion no valida.\n", 19, 0);
        }
    } while (opcionConcesionario != 6);
}
