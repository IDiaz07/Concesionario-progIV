#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vehiculos.h"
#include "database.h"
#include "file.h"

void anadirVehiculo(sqlite3 *db, FILE *archivo, SOCKET cliente_fd) {
    Vehiculo nuevoVehiculo;
    char buffer[256];
    int bytesRecibidos;

    // Marca
    send(cliente_fd, "Introduce la marca del vehiculo: ", 34, 0);
    bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecibidos <= 0) return;
    buffer[bytesRecibidos] = '\0';
    buffer[strcspn(buffer, "\r\n")] = '\0'; // limpia \n
    strcpy(nuevoVehiculo.marca, buffer);

    // Modelo
    send(cliente_fd, "Introduce el modelo del vehiculo: ", 35, 0);
    bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecibidos <= 0) return;
    buffer[bytesRecibidos] = '\0';
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strcpy(nuevoVehiculo.modelo, buffer);

    // Año
    send(cliente_fd, "Introduce el anio del vehiculo: ", 33, 0);
    bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecibidos <= 0) return;
    buffer[bytesRecibidos] = '\0';
    nuevoVehiculo.anio = atoi(buffer);

    // Precio
    send(cliente_fd, "Introduce el precio del vehiculo: ", 35, 0);
    bytesRecibidos = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecibidos <= 0) return;
    buffer[bytesRecibidos] = '\0';
    nuevoVehiculo.precio = atoi(buffer);

    // Guardar
    guardarVehiculo(archivo, nuevoVehiculo, cliente_fd);
    crearTablaVehiculos(db);
    registrarVehiculo(db, nuevoVehiculo.marca, nuevoVehiculo.modelo, nuevoVehiculo.anio, nuevoVehiculo.precio, cliente_fd);
}


void ComprarVehiculoRemoto(sqlite3 *db, SOCKET cliente_fd) {
    Vehiculo vehiculoComprado;
    char buffer[256];
    char nombreUsuario[50];
    int bytes;

    // Pedir marca
    send(cliente_fd, "Introduce la marca del vehiculo: ", 34, 0);
    bytes = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) return;
    buffer[bytes] = '\0';
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strcpy(vehiculoComprado.marca, buffer);

    // Pedir modelo
    send(cliente_fd, "Introduce el modelo del vehiculo: ", 35, 0);
    bytes = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) return;
    buffer[bytes] = '\0';
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strcpy(vehiculoComprado.modelo, buffer);

    // Pedir año
    send(cliente_fd, "Introduce el anio del vehiculo: ", 33, 0);
    bytes = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) return;
    buffer[bytes] = '\0';
    vehiculoComprado.anio = atoi(buffer);

    // Obtener precio
    float precio = obtenerPrecioVehiculo(db, vehiculoComprado.marca, vehiculoComprado.modelo);
    if (precio <= 0) {
        send(cliente_fd, "Vehiculo no encontrado o precio invalido.\n", 42, 0);
        return;
    }

    // Pedir nombre de usuario
    send(cliente_fd, "Introduzca su nombre de usuario: ", 34, 0);
    bytes = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) return;
    buffer[bytes] = '\0';
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strcpy(nombreUsuario, buffer);

    // Obtener IDs y registrar venta
    int id_usuario = buscarIDUsuario(db, nombreUsuario, cliente_fd);
    int id_vehiculo = buscarIDVehiculo(db, vehiculoComprado.marca, vehiculoComprado.modelo, vehiculoComprado.anio);

    if (id_usuario == -1 || id_vehiculo == -1) {
        send(cliente_fd, "Error: Usuario o vehiculo no encontrado.\n", 41, 0);
        return;
    }

    registrarVenta(db, id_usuario, id_vehiculo, precio);

    // Confirmación
    char msg[128];
    snprintf(msg, sizeof(msg), "Compra registrada con éxito. Precio: $%.2f\n", precio);
    send(cliente_fd, msg, strlen(msg), 0);
}



void filtrarVehiculos(sqlite3 *db, SOCKET cliente_fd) {
char criterio[50], valor[50], buffer[256];

    // Pedir criterio
    const char *msg1 = "\nFiltrar vehiculos por -> marca, modelo, anio o precio: ";
    send(cliente_fd, msg1, strlen(msg1), 0);
    int n1 = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (n1 <= 0) return;
    buffer[n1] = '\0';
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strcpy(criterio, buffer);

    // Pedir valor
    const char *msg2 = "Ingrese el valor a buscar: ";
    send(cliente_fd, msg2, strlen(msg2), 0);
    int n2 = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (n2 <= 0) return;
    buffer[n2] = '\0';
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strcpy(valor, buffer);

    // Buscar en archivo
    FILE *archivo = fopen("vehiculos.txt", "r");
    if (!archivo) {
        send(cliente_fd, "Error al abrir el archivo.\n", 28, 0);
        send(cliente_fd, "FIN\n", 4, 0);
        return;
    }

    // Enviar encabezado
    send(cliente_fd, "\nVehiculos encontrados:\n", 25, 0);

    char linea[256];
    int encontrados = 0;
    char *token;
    while (fgets(linea, sizeof(linea), archivo)) {
        char *marca, *modelo, *anio_str, *precio_str;
        int anio;
        float precio;

        // Separar por ;
        token = strtok(linea, ";\n");
        if (!token) continue;
        marca = token;

        token = strtok(NULL, ";\n");
        if (!token) continue;
        modelo = token;

        token = strtok(NULL, ";\n");
        if (!token) continue;
        anio_str = token;
        anio = atoi(anio_str);

        token = strtok(NULL, ";\n");
        if (!token) continue;
        precio_str = token;
        precio = atof(precio_str);

        int match = 0;
        if (strcmp(criterio, "marca") == 0 && _stricmp(marca, valor) == 0) match = 1;
        else if (strcmp(criterio, "modelo") == 0 && _stricmp(modelo, valor) == 0) match = 1;
        else if (strcmp(criterio, "anio") == 0 && atoi(valor) == anio) match = 1;
        else if (strcmp(criterio, "precio") == 0 && atof(valor) == precio) match = 1;

        if (match) {
            snprintf(buffer, sizeof(buffer), "%s %s %d $%.2f\n", marca, modelo, anio, precio);
            send(cliente_fd, buffer, strlen(buffer), 0);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        send(cliente_fd, "No se encontraron vehiculos.\n", 30, 0);
    }

    fclose(archivo);
    send(cliente_fd, "FIN\n", 4, 0);
}
void mostrarVehiculosVendidos(sqlite3 *db, SOCKET cliente_fd) {
    const char *sql =
        "SELECT  u.nombre_usuario, v.marca, v.modelo, v.anio, ve.precio_final "
        "FROM ventas ve "
        "JOIN usuarios u ON ve.id_usuario = u.id "
        "JOIN vehiculos v ON ve.id_vehiculo = v.id "
        "ORDER BY  nombre_usuario;";

    sqlite3_stmt *stmt;
    char buffer[512];

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        send(cliente_fd, "Error al preparar consulta de ventas\n", 38, 0);
        return;
    }

    char usuarioAnterior[100] = "";
    int primerUsuario = 1;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *nombre = (const char *)sqlite3_column_text(stmt, 0);
        const char *marca  = (const char *)sqlite3_column_text(stmt, 1);
        const char *modelo = (const char *)sqlite3_column_text(stmt, 2);
        int anio           = sqlite3_column_int(stmt, 3);
        float precio       = (float)sqlite3_column_double(stmt, 4);

        if (strcmp(nombre, usuarioAnterior) != 0) {
            if (!primerUsuario) {
                send(cliente_fd, "\n", 1, 0);
            }
            snprintf(buffer, sizeof(buffer), "Nombre de usuario: %s\n", nombre);
            send(cliente_fd, buffer, strlen(buffer), 0);
            strcpy(usuarioAnterior, nombre);
            primerUsuario = 0;
        }

        snprintf(buffer, sizeof(buffer), " - %s %s %d $%.2f\n", marca, modelo, anio, precio);
        send(cliente_fd, buffer, strlen(buffer), 0);
    }

    sqlite3_finalize(stmt);

    send(cliente_fd, "FIN_VENTAS\n", 11, 0);
}




