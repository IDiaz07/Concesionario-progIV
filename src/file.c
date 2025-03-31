#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "vehiculos.h"
#include "database.h"



void inicializarArchivo(FILE** archivo) {
    *archivo = fopen("vehiculos.txt", "a+");
    if (*archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        exit(1);
    }
}


void guardarVehiculo(FILE *archivo, Vehiculo v) {
    if (archivo == NULL) {
        printf("Error: No se puede abrir el archivo.\n");
        return;
    }

    fprintf(archivo, "%s;%s;%d;%.2f\n", v.marca, v.modelo, v.anio, v.precio);
    fflush(archivo); 
}

void mostrarVehiculos(FILE* archivo) {
    char linea[256];
    rewind(archivo);
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        printf("%s", linea);
    }
}

void FiltrarMarca(FILE* archivo){
    char filtro[50];
    char linea[256];
    char marca[50];
     printf("Introduce la marca: ");
    scanf("%s", filtro);
    rewind(archivo);
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        
        sscanf(linea, "%49s[^;]", marca);

        
        if (strcmp(marca, filtro) == 0) {
            printf("%s \n", linea);  
    

    }
}
}



void cargarVehiculosDesdeArchivo(sqlite3 *db, FILE *archivo) {
    archivo = fopen("vehiculos.txt", "r");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        Vehiculo v;
        char anio[10], precio[10];

        
        char *split = strtok(linea, ";");
        if (split) strcpy(v.marca, split);

        split = strtok(NULL, ";");
        if (split) strcpy(v.modelo, split);

        split = strtok(NULL, ";");
        if (split) strcpy(anio, split);
        v.anio = atoi(anio); 

        split = strtok(NULL, ";");
        if (split) strcpy(precio, split);
        v.precio = atof(precio); 

        
       

        if (!vehiculoExiste(db, v.marca, v.modelo, v.anio)) {
           
            registrarVehiculo(db, v.marca, v.modelo, v.anio, v.precio);
        } else {
            printf("El vehiculo %s %s %d ya esta registrado.\n", v.marca, v.modelo, v.anio);
        }
    }
    

   

    fclose(archivo);
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