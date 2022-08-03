#ifndef __CHECKER_H__
#define __CHECKER_H__

#include "../utils.h"
#include "../structures/hashtable.h"
#include "../structures/gqueue.h"
#include "../structures/bheap.h"
#include "../structures/dtree.h"
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
void make_suggests(WrongWord word, HashTable dictionary, DTree dist_dict);

/*
 * Chequea palabra por palabra del archivo path si esta es valida tomando
 * como referencia el diccionario. Retorna una tabla hash con las
 * sugerencias hechas para las palabras con errores de escritura
*/
HashTable check_file(const char* input, HashTable dictionary, DTree dist_dict);

#endif /* __CHECKER_H__ */
