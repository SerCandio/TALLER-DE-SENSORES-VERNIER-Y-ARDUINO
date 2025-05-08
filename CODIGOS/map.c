// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>


static float map(const float x, const float in_min, const float in_max, const float out_min, const float out_max);


int main() {
    // Write C code here
    float voltsDC=4.49f;
    
    printf("El valor de Volts DC : %.3f\r\n", voltsDC);
    
    float presion=map(voltsDC, 0.5f, 4.5f, 0.0f, 1600.0f);
    
    printf("El valor de la Presion [kPa] es: %.2f", presion);

    return 0;
}


static float map(const float x, const float in_min, const float in_max, const float out_min, const float out_max){
    
    //La funcion map tiene la forma: Y = m*Xin + b 
    //                              m: pendiente
    //                              b : intercepto
    //Se usa para convertir un dato en cierto rango de entrada
    //hacia otro de salida
    
    return ((out_max - out_min) / (in_max - in_min)) * (x - in_min) + out_min;
    
}