// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Numero de muestras
#define NUMERO_MUESTRAS         8U

//Tipo de dato de las muestras
#define MUESTRAS_T              float


//Variables Globales : RAM
//Puede ser modificado/mostrado su valor mientras en programa se ejecute

static  float muestras[NUMERO_MUESTRAS]={9.25f,10.35f,10.0f,10.55f,7.15f,6.35f,12.77f,1.15f};

/*
 Funciones prototipo
*/
extern MUESTRAS_T promediar(const MUESTRAS_T *pMuestras, const uint16_t N);
extern MUESTRAS_T maximo(const MUESTRAS_T *pMuestras, const uint16_t N);
extern MUESTRAS_T minimo(const MUESTRAS_T *pMuestras, const uint16_t N);


int main() {
    // Write C code here
    muestras[0] = 0.0f;
    printf("Las muestras son: {");
    for(int i=0; i<NUMERO_MUESTRAS;i++){
        printf(" %.2f",muestras[i]);
        if(i<(NUMERO_MUESTRAS-1)) printf(",");
    }
    puts(" }\r\n");
    
    //*pMuestras=muestras ; muestras en realidad es @muestras
    
    printf("La @Addr base en RAM del array muestras es: %p\r\n", muestras);
    
    printf("El promedio de las muestras es : %.2f\r\n", 
            promediar(muestras, NUMERO_MUESTRAS)
    );
    
    printf("El MAX de las muestras es : %.2f\r\n", 
            maximo(muestras, NUMERO_MUESTRAS)
        );
        
    printf("El MIN de las muestras es : %.2f\r\n", 
            minimo(muestras, NUMERO_MUESTRAS)
        );
    
    puts("\r\n");
    
    //========================================================
        
    return 0;
}

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