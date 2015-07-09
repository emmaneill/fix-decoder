#include "hashTable.h"

hash_table_t* init_hash_table(int size)
{
    hash_table_t *new_table;
    
    if (size<1) return NULL; /* invalid size for table */

    /* Attempt to allocate memory for the table structure */
    if ((new_table = malloc(sizeof(hash_table_t))) == NULL) {
        return NULL;
    }
    
    /* Attempt to allocate memory for the table itself */
    if ((new_table->table = malloc(sizeof(node_t *) * size)) == NULL) {
        return NULL;
    }

    /* Initialize the elements of the table */
    for(int i=0; i<size; i++) new_table->table[i] = NULL;

    /* Set the table's size */
    new_table->size = size;

    return new_table;
}


unsigned int hash(hash_table_t *hashtable, char *str)
{
    unsigned int hashval;
    
    /* we start our hash out at 0 */
    hashval = 0;

    /* for each character, we multiply the old hash by 31 and add the current
 *      * character.  Remember that shifting a number left is equivalent to 
 *           * multiplying it by 2 raised to the number of places shifted.  So we 
 *                * are in effect multiplying hashval by 32 and then subtracting hashval.  
 *                     * Why do we do this?  Because shifting and subtraction are much more 
 *                          * efficient operations than multiplication.
 *                               */
    for(; *str != '\0'; str++) hashval = *str + (hashval << 5) - hashval;

    /* we then return the hash value mod the hashtable size so that it will
 *      * fit into the necessary range
 *           */
    return hashval % hashtable->size;

}

tagType find_tag_type(hash_table_t *hashtable, char* myStr)
{
    node_t *list;
    unsigned int hashval = hash(hashtable, myStr);

    /* Go to the correct list based on the hash value and see if str is
 *      * in the list.  If it is, return return a pointer to the list element.
 *           * If it isn't, the item isn't in the table, so return NULL.
 *                */
    for(list = hashtable->table[hashval]; list != NULL; list = list->next) {
        if (strcmp(myStr, list->str) == 0) return list->type;
    }
    return (is_unknown);
}

int add_string(hash_table_t *hashtable, char *str, tagType type)
{
    node_t *new_list;
    unsigned int hashval = hash(hashtable, str);

    /* Attempt to allocate memory for list */
    if ((new_list = malloc(sizeof(node_t))) == NULL) return 1;

    /* Does item already exist? */
    if (find_tag_type(hashtable, str) != is_unknown)
    {
        return 2;
    }
    /* Insert into list */
    new_list->type = type;
    
    new_list->str = strdup(str);
    new_list->next = hashtable->table[hashval];
    hashtable->table[hashval] = new_list;

    return 0;
}
