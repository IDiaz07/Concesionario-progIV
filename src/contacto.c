#include <stdio.h>
#include "contacto.h"
#include <winsock.h>

void mostrarContacto(SOCKET cliente_fd) { // Añadir socket como parámetro
    char buffer[512];
    snprintf(buffer, sizeof(buffer), 
        "\n===== Contacto DeustoMotors =====\n"
        "Direccion: Avda. de las Universidades, 24. 48007 - Bilbao, Espanya\n"
        "Telefono: +34 944 123 456\n"
        "Email: atencionalcliente@deustomotors.com\n"
        "Horario de atencion: Lunes a Viernes, 9:00 - 18:00\n"
        "=================================\n");
    send(cliente_fd, buffer, strlen(buffer), 0); // Enviar por socket
}
