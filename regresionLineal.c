/*

Nombre del programa: regresionLineal.c
Autor: César Jalpa Villanueva
Modificado por:

El programa muestra la grafica con los puntos de datos y la recta que se ajusta a ellos.
Los parámetros de la recta se calculan utilizando el método de los minimos cuadrados.
Los datos se encuentran en el archivo datos.txt . 
En cada renglón del archivo hay un par de coordenadas separadas por un espacio: x y .
El programa de graficación gnuplot se invoca para realizar la grafica de los datos y la recta ajustada.
Se asume que el programa gnuplot es invocable desde cualquier shell
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/* Devuelve la suma de las entradas de un arreglo "arreglo" de tamaño "capacidad".
*/
double sumaEntradas(double *arreglo, int capacidad){
    double suma = 0;

    for(int i=0; i < capacidad; i++) suma += arreglo[i];

    return suma;
}

/* Devuelve el producto de las entradas de dos arreglos (arr1, arr2) de tamaño "capacidad".
*/
double sumaProductoEntradas(double *arr1, double *arr2, int capacidad){
    double suma = 0;

    for(int i=0; i < capacidad; i++) suma += arr1[i]*arr2[i];

    return suma;
}



int cuentaEntradas(char archivo[]){
    FILE *ap_archivo;

    ap_archivo = fopen(archivo, "r"); // Se abre en modo lectura el archivo de datos
    if(ap_archivo == NULL){
        printf("\aError al abrir el archivo de datos \n");
        exit(-2);
    }
    int numeroDePuntos = 0;
    double a, b;
    while (!feof(ap_archivo))
    {
        fscanf(ap_archivo, "%lg %lg", &a, &b);
        if(!feof(ap_archivo))  ++numeroDePuntos;
    }
    fclose(ap_archivo);
    return numeroDePuntos;
}


/*
Regresión lineal con los datos contenidos en el archivo cuyo nombre está contenido en el string datos
*/
void muestraRegresionLineal(char* datos){
    /* 
    Crear un script gnuplot en un archivo (p.ej.: script.gp).
    El contenido del script seran dos lineas:
    f(x)=m*x + b
    plot ’datos’ using 1:2 title ’Datos’ with points, f(x) title ’Recta ajustada’

    Los parametros m y b deben calcularse primero con el metodo de los minimos cuadrados y 'datos' es el nombre
    del archivo que contiene los datos incluido en el string datos.
    
    Despues de guardar el script se debe invocar la funcion 
    system("gnuplot -p script.gp")
    */
   /*
   Para el calculo de los parametros m y b
   - abrir el archivo para contar el numero de puntos que contiene
   - leer los datos y almacenarlos en dos arreglos
   - calcular los parametros aplicando las formulas de los minimos cuadrados
   */
    
    int numeroDePuntos = cuentaEntradas(datos);
    printf("Numero de puntos: %d\n", numeroDePuntos);
    double x[numeroDePuntos], y[numeroDePuntos];
    FILE *ap_archivo;

    ap_archivo = fopen(datos, "r"); // Se abre en modo lectura el archivo de datos
    if(ap_archivo == NULL){
        printf("\aError al abrir el archivo de datos \n");
        exit(-2);
    }
    
    for (int i=0; i < numeroDePuntos && !feof(ap_archivo); i++)
    {
        fscanf(ap_archivo, "%lg %lg", &x[i], &y[i]);
        printf("%d: %g %g\n", i, x[i], y[i]);
    }
    fclose(ap_archivo);

    double sumaX, sumaY, sumaXX, sumaXY;
    double m, b;

    printf("Calculando regresion  lineal\n\n");

    sumaX = sumaEntradas(x, numeroDePuntos);
    sumaY = sumaEntradas(y, numeroDePuntos);
    sumaXX = sumaProductoEntradas(x,x, numeroDePuntos);
    sumaXY = sumaProductoEntradas(x, y, numeroDePuntos);
    
    double denominador = numeroDePuntos*sumaXX-sumaX*sumaX;
    m = (numeroDePuntos * sumaXY - sumaX*sumaY)/denominador;
    b = (sumaY*sumaXX-sumaX*sumaXY)/denominador;

    printf("m = %f, b = %f\n\n", m,b);

    ap_archivo = fopen("script.gp", "w");
    if (ap_archivo == NULL) {
    printf("Error en la  creacion del script gnuplot \n");
    exit(-1);
    }
    /*
    Script gnuplot:
    f(x)=m*x + b
    plot ’nombreArchivo’ using 1:2 title ’Titulo’ with points, f(x) title ’Titulo’
    */
    fprintf(ap_archivo, "f(x)= %lf * x + (%lf) \n", m, b);
    fprintf(ap_archivo, "plot '%s' using 1:2 title 'Puntos de datos' with points, f(x) title 'Recta ajustada'", datos);
    fclose(ap_archivo);
    system("gnuplot -p script.gp");
}



#define NUM_PTS 10
int main(char *argv, int argc){

    muestraRegresionLineal("datos.txt");
}