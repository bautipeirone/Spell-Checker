#ifndef __DICT_H__
#define __DICT_H__

#include "../structures/hashtable.h"
#include "../structures/trie.h"
#include <stdio.h>

/*
 * Lee una palabra del archivo y la almacena en buf
*/
int read_word(FILE* fp, char buf[MAX_LEN_WORD]);

/*
 * Lee el archivo path con formato diccionario y almacena las palabras en una
 * tabla hash (o Trie)
*/
Trie create_dictionary(const char* path);



#endif /* __DICT_H__ */