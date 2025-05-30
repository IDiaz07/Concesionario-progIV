#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "file.h"
#include "concesionario.h"
#include "servicios.h"
#include "database.h"
#include "mensajesusuarios.h"

#define BUFFER_SIZE 256

void menuAdministrativo(sqlite3 *db, SOCKET cliente_fd) {
    FILE* archivo;
    inicializarArchivo(&archivo, cliente_fd);

    char buffer[BUFFER_SIZE];
    int opcionDeustoMotors;

    do {
        const char *menu =
            "\n=== Menu Administrativo ===\n"
            "1. Enviar mensaje a los usuarios\n"
            "2. Ver todos los vehiculos vendidos\n"
            "3. Aniadir vehiculo\n"
            "4. Mirar Plantilla\n"
            "5. Cerrar Sesion\n"
            "Seleccione una opcion: ";
        send(cliente_fd, menu, strlen(menu), 0);

        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;

        opcionDeustoMotors = atoi(buffer);

        switch (opcionDeustoMotors) {
            case 1:
                enviarMensajeAUsuarios(db, cliente_fd);  // Si imprime por consola, adaptalo también
                break;
            case 2:
                mostrarVehiculosVendidos(db, cliente_fd);
                break;
            case 3:
                anadirVehiculo(db, archivo, cliente_fd);  // ya espera cliente_fd
                break;
            case 4:
                cargarPlantillaDesdeArchivo(db, "plantilla.txt");
                mostrarPlantilla(db, cliente_fd);
                break;
            case 5:
                send(cliente_fd, "Saliendo del menu administrativo...\n", 36, 0);
                break;
            default:
                send(cliente_fd, "Opcion no valida.\n", 19, 0);
                break;
        }
    } while (opcionDeustoMotors != 5);

    fclose(archivo);
}
