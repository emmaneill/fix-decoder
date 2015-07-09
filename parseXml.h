#ifndef PARSEXML_H_   /* Include guard */
#define PARSEXML_H_


#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "hashTable.h"

hash_table_t* parse_doc ( char *docname );
void parse_fields (xmlDocPtr doc, xmlNodePtr cur );

#endif // PARSEXML_H_
