#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "concesionario.h"
#include "file.h"
#include "vehiculos.h"
#include "database.h"
#include "usuarios.h"
#include "servicios.h"
#include "menubasico.h"
#include "menuadministrativo.h"


char nombreUsuarioAutenticado[50] = "";


void mostrarMenuPrincipal() {
    printf("\nMenu Principal:\n");
    printf("1. Registro de usuario\n");
    printf("2. Iniciar sesion\n");
    printf("3. Salir\n");
}


int seleccionarOpcionPrincipal() {
    int opcion;
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}


