#ifndef __UTILS_H__
#define __UTILS_H__

// Importacion general de librerias
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define NCHARS 26 // Cantidad de caracters en el alfabeto sin la ñ

// Numero de sugerencias a hacer por palabra incorrecta
#define NUM_SUGGESTS 5

// Longitud maxima de una palabra en el diccionario/entrada
#define MAX_LEN_WORD 35

// Retorna una copia de dato
typedef void *(*CopyFunction)(void *data);

// Retorna un entero negativo si data1 < data2, 0 si son iguales, 1 sino
typedef int (*CompareFunction)(void *data1, void *data2);

// Libera la memoria usada por data
typedef void (*DestroyFunction)(void *data);

// Retorna un entero positivo para data
typedef unsigned (*HashFunction)(void *data);

// Funcion de representacion del dato, con argumento extra
typedef void (*VisitFunctionExtra)(void *data, void *extra);

/**
 * Funcion de hash para strings propuesta por Kernighan & Ritchie en "The C
 * Programming Language (Second Ed.)".
 */
unsigned KRHash(char *s);

#endif /* __UTILS_H__ */
