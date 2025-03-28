#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "vehiculos.h"

void guardarVehiculo(FILE* archivo, Vehiculo* vehiculo) {
    fprintf(archivo, "%s;%s;%d;%.2f\n", vehiculo->marca, vehiculo->modelo, vehiculo->anio, vehiculo->precio);
    fflush(archivo); 
}

void inicializarArchivo(FILE** archivo) {
    *archivo = fopen("vehiculos.txt", "a+");
    if (*archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        exit(1);
    }
}

void mostrarVehiculos(FILE* archivo) {
    char linea[256];
    rewind(archivo);
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        printf("%s", linea);
    }
}

void exportarAFichero(FILE* archivo, const char* nombreFichero) {
    FILE* archivoCSV = fopen(nombreFichero, "w");
    if (archivoCSV == NULL) {
        printf("No se pudo crear el archivo CSV.\n");
        return;
    }

    char linea[256];
    rewind(archivo);
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        fprintf(archivoCSV, "%s", linea);
    }

    fclose(archivoCSV);
    printf("Vehículos exportados a %s.\n", nombreFichero);
}