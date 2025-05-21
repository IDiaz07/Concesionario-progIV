#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "vehiculos.h"
#include "database.h"
#include <winsock2.h>

void inicializarArchivo(FILE** archivo, SOCKET cliente_fd) {
    *archivo = fopen("vehiculos.txt", "a+");
    if (*archivo == NULL) {
        send(cliente_fd, "No se pudo abrir el archivo.\n", 28, 0);
        exit(1);
    }
}

void guardarVehiculo(FILE *archivo, Vehiculo v, SOCKET cliente_fd) {
    if (archivo == NULL) {
        send(cliente_fd, "Error: No se puede abrir el archivo.\n", 37, 0);
        return;
    }
    
    fprintf(archivo, "%s;%s;%d;%d\n", v.marca, v.modelo, v.anio, v.precio);
    fflush(archivo);
    send(cliente_fd, "Vehiculo guardado correctamente.\n", 33, 0);
}

void mostrarVehiculos(FILE* archivo, SOCKET cliente_fd) {
    char linea[256];
    rewind(archivo);

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        send(cliente_fd, linea, strlen(linea), 0);
    }

    
    const char* fin = "FIN_ENVIO\n";
    send(cliente_fd, fin, strlen(fin), 0);
}


void FiltrarMarca(FILE* archivo, SOCKET cliente_fd) {
    char filtro[50];
    char linea[256];
    char marca[50];
    char buffer[256];

    send(cliente_fd, "Introduce la marca: ", 19, 0);
    int bytes = recv(cliente_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) return;
    buffer[bytes] = '\0';
    sscanf(buffer, "%s", filtro);

    rewind(archivo);
    while (fgets(linea, sizeof(linea), archivo) != NULL) {

        sscanf(linea, "%[^;]", marca);

        if (strcmp(marca, filtro) == 0) {
            send(cliente_fd, linea, strlen(linea), 0);
        }
    }
}



void cargarVehiculosDesdeArchivo(sqlite3 *db, FILE *archivo, SOCKET cliente_fd) {
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
        v.precio = atoi(precio);


       

        if (!vehiculoExiste(db, v.marca, v.modelo, v.anio)) {
           
            registrarVehiculo(db, v.marca, v.modelo, v.anio, v.precio, cliente_fd);

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

void cargarPlantillaDesdeArchivo(sqlite3 *db, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo %s.\n", nombreArchivo);
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        char nombre[100] = {0}, cargo[50] = {0}, salarioStr[20] = {0};
        double salario = 0.0;

        // Parsear línea
        char *split = strtok(linea, ";\n");
        if (split) strcpy(nombre, split);

        split = strtok(NULL, ";\n");
        if (split) strcpy(cargo, split);

        split = strtok(NULL, ";\n");
        if (split) {
            strcpy(salarioStr, split);
            salario = atof(salarioStr);
        }

        // Verificar si ya existe la persona (por nombre)
        sqlite3_stmt *stmt;
        const char *sql_check = "SELECT COUNT(*) FROM plantilla WHERE nombre = ?;";
        if (sqlite3_prepare_v2(db, sql_check, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                int existe = sqlite3_column_int(stmt, 0);
                sqlite3_finalize(stmt);
                if (existe > 0) {
                    printf("Empleado '%s' ya existe. Se omite.\n", nombre);
                    continue;
                }
            } else {
                sqlite3_finalize(stmt);
            }
        }

        // Insertar si no existe
        char sqlInsert[256];
        snprintf(sqlInsert, sizeof(sqlInsert),
                 "INSERT INTO plantilla (nombre, cargo, salario) VALUES ('%s', '%s', %.2f);",
                 nombre, cargo, salario);

        char *errMsg = 0;
        if (sqlite3_exec(db, sqlInsert, 0, 0, &errMsg) != SQLITE_OK) {
            printf("Error al insertar datos: %s\n", errMsg);
            sqlite3_free(errMsg);
        } else {
            printf("Empleado '%s' insertado correctamente.\n", nombre);
        }
    }

    fclose(archivo);
    printf("Carga de plantilla finalizada.\n");
}