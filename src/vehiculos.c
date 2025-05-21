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


void ComprarVehiculo(sqlite3 *db, SOCKET cliente_fd){
    Vehiculo vehiculoComprado;
    char nombreUsuario[50];
    printf("Introduce la marca del vehiculo: ");
    scanf("%s", vehiculoComprado.marca);

    printf("Introduce el modelo del vehiculo: ");
    scanf("%s", vehiculoComprado.modelo);

    printf("Introduce el anio del vehiculo: ");
    scanf("%d", &vehiculoComprado.anio);

    
 float precio =obtenerPrecioVehiculo(db,vehiculoComprado.marca,vehiculoComprado.modelo);
    
    printf("Introduzca su nombre de usuario: ");
    scanf("%s", nombreUsuario);
  int id_usuario=  buscarIDUsuario(db, nombreUsuario, cliente_fd);
  int id_vehiculo= buscarIDVehiculo(db,vehiculoComprado.marca,vehiculoComprado.modelo,vehiculoComprado.anio);
  registrarVenta(db,id_usuario,id_vehiculo,precio);

    
    
    

}
void filtrarVehiculos(FILE* archivo) {
    char criterio[50];
    char valor[50];
    char linea[256];
    Vehiculo vehiculo;

    printf("\nFiltrar vehculos por: marca, modelo, anio: ");
    scanf("%s", criterio);
    printf("Ingrese el valor a buscar: ");
    scanf("%s", valor);

    rewind(archivo);
    printf("\nVehiculos encontrados:\n");
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        sscanf(linea, "%[^;];%[^;];%d;%f", vehiculo.marca, vehiculo.modelo, &vehiculo.anio, &vehiculo.precio);

        if ((strcmp(criterio, "marca") == 0 && strcmp(vehiculo.marca, valor) == 0) ||
            (strcmp(criterio, "modelo") == 0 && strcmp(vehiculo.modelo, valor) == 0) ||
            (strcmp(criterio, "anio") == 0 && atoi(valor) == vehiculo.anio) ||
            (strcmp(criterio, "precio") == 0 && atof(valor) == vehiculo.precio)) {
            printf("%s %s %d %.2f\n", vehiculo.marca, vehiculo.modelo, vehiculo.anio, vehiculo.precio);
        }
    }
}
