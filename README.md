# Corrector ortografico

## Uso del programa
Para compilar el programa:
```
make
```
Para compilar con opciones de depurado para GDB o Valgrind
```
make debug
```

Para ejecutar el corrector, se debe proveer el archivo a corregir y el nombre
del archivo de salida, y ejectuarse como sigue
```
./main entrada.txt output.txt
```

## Descripcion
El programa leerá un texto de entrada junto con un archivo diccionario. El texto
será analizado palabra por palabra, y para aquellas palabras que no se
encuentren en el diccionario se generaran un numero de sugerencias dentro de un
rango de busqueda (por defecto hasta distancia 3). Estas sugerencias se obtienen
de combinar las 5 reglas de produccion: Insercion, Reemplazo, Eliminacion,
Trasposicion y Separacion.

Estas sugerencias se escriben en un archivo de salida, donde se indica cual es
la palabra a corregir, las lineas en que aparece dicha palabra y sus apropiadas
sugerencias
