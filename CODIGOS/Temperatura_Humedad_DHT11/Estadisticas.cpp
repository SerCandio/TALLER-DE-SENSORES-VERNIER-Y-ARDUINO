#include "Estadisticas.h"


extern MUESTRAS_T promediar(const MUESTRAS_T *pMuestras, const uint16_t N){
    MUESTRAS_T suma=0;
    
    for(uint16_t i=0; i<N;i++){
        suma+=*pMuestras++;
    }
    
    return suma/((MUESTRAS_T) N);
    
}

extern MUESTRAS_T maximo(const MUESTRAS_T *pMuestras, const uint16_t N){
    MUESTRAS_T max=*pMuestras;
    
    // Iterate through the array, starting from the second element
    for (uint16_t i = 1; i < N; i++){
        *pMuestras++; //Update pointer in every iteration
        if(*pMuestras>max){
            max=*pMuestras;// Update max if current element is larger
        }
    }
    
    return max;
}

extern MUESTRAS_T minimo(const MUESTRAS_T *pMuestras, const uint16_t N){
   MUESTRAS_T min=*pMuestras;
   
   // Iterate through the array, starting from the second element
   for (uint16_t i = 1; i < N; i++){
       *pMuestras++;//Update pointer in every iteration
       if(*pMuestras<min){
           min=*pMuestras;// Update min if current element is larger
       }
   }
   
   return min;
   
}