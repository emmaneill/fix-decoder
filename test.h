#ifndef TEST_H_
#define TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "hashTable.h"

#define MAXBUFLEN 200
//TODO Set this to disk block size 

unsigned char         gDelim;
int                   gMsgCount;
hash_table_t*         gHashTable;

    void append(char* s, char c);

    void find_delim(char** ptr);

    void parse_string_value(char** ptr, char* valStr);

    char* get_next_tag( char** ptr);

#endif // TEST_H_

