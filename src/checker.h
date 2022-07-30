#ifndef __CHECKER_H__
#define __CHECKER_H__

#include "../utils.h"
#include "../structures/hashtable.h"
#include "../structures/trie.h"
#include "../structures/gqueue.h"
#include "../structures/bheap.h"
#include "dictionary.h"


/*
 * Dada una palabra que no se encuentra en el diccionario, busca sugerencias
 * en base a las 5 reglas propuestas: insercion, reemplazo, eliminacion,
 * intercambio y separacion. Las sugerencias son agregadas al arreglo de
 * sugerencias de la estructura word. La funcion se detiene cuando se hayan
 * encontrado NUM_SUGGESTS sugerencias o se hayan probado todas las palabras
 * de hasta distancia 3. En el arreglo, las palabras se encuentran ordenadas
 * segun su distancia a la palabra original
*/
void make_suggests(WrongWord word, Trie dictionary, unsigned size);

/*
 * Algoritmo para obtener distancia de edicion de dos strings.
 * Nota: Para los casos evidentes de distancia mayor a 3, el algoritmo
 * se detiene antes de ejecutarse, devolviendo un entero grande

 * Tiempo: O(n * m) donde len(str1) = n y len(str2) = m
 * Memoria: O(n * m) donde len(str1) = n y len(str2) = m
*/
unsigned edit_distance(const char* str1, const char* str2);

unsigned distance(char *str1, char *str2, unsigned len1, unsigned len2);

/*
 * Analiza si una palabra esta en el diccionario, si no lo esta, busca en la
 * tabla para ver si la palabra ya fue previamente corregida, y sino busca
 * 5 sugerencias (en lo posible) agregandolas a la tabla
*/
void check_word(const char* str, HashTable corrected_words, Trie dict);


/*
 * Chequea palabra por palabra del archivo path si esta es valida tomando
 * como referencia el diccionario. Retorna una tabla hash con las
 * sugerencias hechas para las palabras con errores de escritura
*/
HashTable check_file(const char* input, Trie dictionary, unsigned size);

/*

*/
BHeap calculate_distances(Trie root, unsigned dict_size, char *str, int len);

#endif /* __CHECKER_H__ */
