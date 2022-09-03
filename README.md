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
./main entrada.txt salida.txt
```

A la hora de compilar, se puede pasar al compilador la bandera -DCHECK_DUP, lo que hará que el programa internamente lleve
registro de las combinaciones previamente usadas, para asi, reducir en teoria el numero de palabras a probar en el caso de no encontrar todas las sugerencias pedidas.

Esta opcion no es compilada por defecto, ya que en la practica ha 
reducido el tiempo de ejecucion del programa para la mayoria de archivos con los que se probó.

Si se quiere compilar con esta opcion, se puede ejecutar:
```
make clean
make CHECK_DUP=1
```
o
```
make -B CHECK_DUP=1
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
