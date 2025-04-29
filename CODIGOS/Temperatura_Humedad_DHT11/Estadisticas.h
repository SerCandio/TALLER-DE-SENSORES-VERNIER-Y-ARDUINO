#ifndef Estadisticas_h
#define Estadisticas_h
 
#include "Arduino.h"

//Tipo de dato de las muestras
#define MUESTRAS_T              float

/*
 Funciones prototipo
 Acces: Public
*/
extern MUESTRAS_T promediar(const MUESTRAS_T *pMuestras, const uint16_t N);
extern MUESTRAS_T maximo(const MUESTRAS_T *pMuestras, const uint16_t N);
extern MUESTRAS_T minimo(const MUESTRAS_T *pMuestras, const uint16_t N);
 
#endif