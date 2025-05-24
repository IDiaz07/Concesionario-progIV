#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include "sqlite3.h"

void pruebaManejo(sqlite3 *db, SOCKET cliente_fd, const char* usuario) {
    char buffer[256];
    const char *menu =
        "\n=== Prueba de Manejo ===\n"
        "1. Reservar cita\n"
        "2. Mostrar cita\n"
        "Seleccione una opcion: ";

    send(cliente_fd, menu, strlen(menu), 0);
    int n = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) return;
    buffer[n] = '\0';
    int opcion = atoi(buffer);

    if (opcion == 1) {
        // Mostrar vehículos disponibles
        FILE *archivo = fopen("vehiculos.txt", "r");
        if (!archivo) {
            send(cliente_fd, "Error al abrir vehiculos.txt\n", 29, 0);
            return;
        }

        send(cliente_fd, "Vehículos disponibles:\n", 24, 0);
        while (fgets(buffer, sizeof(buffer), archivo)) {
            send(cliente_fd, buffer, strlen(buffer), 0);
        }
        fclose(archivo);

        // Pedir vehículo
        send(cliente_fd, "\nEscriba marca y modelo separados por punto y coma (ej: Toyota;Corolla): ", 72, 0);
        n = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) return;
        buffer[n] = '\0';
        char vehiculo[100];
        strcpy(vehiculo, buffer);

        // Pedir fecha
        send(cliente_fd, "Ingrese la fecha (DD/MM/AAAA): ", 32, 0);
        n = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) return;
        buffer[n] = '\0';
        char fecha[20];
        strcpy(fecha, buffer);

        // Pedir hora
        send(cliente_fd, "Ingrese la hora (HH:MM): ", 26, 0);
        n = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) return;
        buffer[n] = '\0';
        char hora[10];
        strcpy(hora, buffer);

        // Guardar en la base de datos
        char sql[256];
        sprintf(sql, "INSERT INTO citas (usuario, vehiculo, fecha, hora) VALUES ('%s', '%s', '%s', '%s');",
                usuario, vehiculo, fecha, hora);

        char *errMsg = 0;
        if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
            send(cliente_fd, "Error al guardar la cita.\n", 27, 0);
            sqlite3_free(errMsg);
        } else {
            send(cliente_fd, "Cita reservada con exito.\n", 27, 0);
        }

    } else if (opcion == 2) {
        // Buscar cita en la base de datos
        char sql[256];
        sprintf(sql, "SELECT vehiculo, fecha, hora FROM citas WHERE usuario='%s';", usuario);

        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            int row_found = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                row_found = 1;
                const unsigned char *vehiculo = sqlite3_column_text(stmt, 0);
                const unsigned char *fecha = sqlite3_column_text(stmt, 1);
                const unsigned char *hora = sqlite3_column_text(stmt, 2);

                char salida[256];
                snprintf(salida, sizeof(salida),
                         "Cita encontrada:\nVehiculo: %s\nFecha: %s\nHora: %s\n",
                         vehiculo, fecha, hora);
                send(cliente_fd, salida, strlen(salida), 0);
            }

            if (!row_found) {
                send(cliente_fd, "No hay ninguna cita registrada.\n", 32, 0);
            }

            sqlite3_finalize(stmt);
        } else {
            send(cliente_fd, "Error al consultar cita.\n", 26, 0);
        }
    } else {
        send(cliente_fd, "Opcion no valida.\n", 19, 0);
    }
}
