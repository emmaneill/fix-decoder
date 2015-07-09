#ifndef HASHTABLE_H_   /* Include guard */
#define HASHTABLE_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "tagList.h"

typedef struct node
{
    char* str;
    tagType type;
    struct node *next;
} node_t;


typedef struct _hash_table_t_ {
    int size;       /* the size of the table */
    node_t **table; /* the table elements */
} hash_table_t;


    hash_table_t* init_hash_table(int size);
    unsigned int hash(hash_table_t *hashtable, char* str);


    tagType find_tag_type(hash_table_t *hashtable, char* str);
    int add_string(hash_table_t *hashtable, char* str, tagType type);


#endif // HASHTABLE_H_
