#ifndef MANTENIMIENTO_H
#define MANTENIMIENTO_H

typedef struct {
    char tipoServicio[50];
    char fecha[11];
    char hora[6];
} Cita;
void mantenimientoYReparacion();

#endif
