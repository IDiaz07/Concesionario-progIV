#ifndef MANTENIMIENTO_H
#define MANTENIMIENTO_H

// Definición de la estructura Cita
typedef struct {
    char tipoServicio[50];
    char fecha[11];
    char hora[6];
} Cita;

// Declaración de la función para hacer una reserva de mantenimiento
void mantenimientoYReparacion();

#endif
