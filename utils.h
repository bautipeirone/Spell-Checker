#ifndef __UTILS_H__
#define __UTILS_H__

#define NCHARS 26 // Caracters en el alfabeto sin la ñ

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

/**
 * Funcion de hash para strings propuesta por Kernighan & Ritchie en "The C
 * Programming Language (Second Ed.)".
 */
unsigned KRHash(char *s);

#endif /* __UTILS_H__ */
