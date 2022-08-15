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

// Distancia maxima de busqueda de sugerencias
#define MAX_SEARCH_DISTANCE 3

// Retorna una copia de dato
typedef void *(*CopyFunction)(void *data);

// Retorna un entero negativo si data1 < data2, 0 si son iguales, 1 sino
typedef int (*CompareFunction)(void *data1, void *data2);

// Libera la memoria usada por data
typedef void (*DestroyFunction)(void *data);

// Retorna un entero positivo para data
typedef unsigned (*HashFunction)(void *data);

typedef void (*VisitFunction)(void *data);

// Funcion de representacion del dato, con argumento extra
typedef void (*VisitFunctionExtra)(void *data, void *extra);

// Retorna el mismo puntero que recibe
void *id(void *p);

// No efectua ninguna accion
void null(void *p);

/**
 * Funcion de hash para strings propuesta por Kernighan & Ritchie en "The C
 * Programming Language (Second Ed.)".
 */
unsigned KRHash(char *s);

/*
 * Retorna una copia del string
*/
char* copy_str(const char *str);

/*
 * Retorna una copia del int
*/
int* copy_int(int *p);

/*
int min(const int x, const int y);

int min3(int x, int y, int z);

int min4(const int x, const int y, const int z, const int w);
*/

void lower_str(char *str);

#endif /* __UTILS_H__ */
