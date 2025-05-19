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
#include <winsock.h>


char nombreUsuarioAutenticado[50] = "";


void mostrarMenuPrincipal(SOCKET cliente_fd) { 
    char menu[] = "\nMenu Principal:\n1. Registro de usuario\n2. Iniciar sesion\n3. Salir\n";
    send(cliente_fd, menu, strlen(menu), 0);
}


int recibirOpcion(SOCKET cliente_fd) { // Recibir opción desde el cliente
    char buffer[10];
    recv(cliente_fd, buffer, sizeof(buffer), 0);
    return atoi(buffer);
}

