#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <stdlib.h>
#include "parseXml.h"

int gNodeCount = 0;
//Array of strings
hash_table_t* hashTable = NULL;

hash_table_t* parse_doc ( char *docname )
{
        xmlDocPtr doc;
        xmlNodePtr cur;
        doc = xmlParseFile ( docname );

        if ( doc == NULL )
        {   
                fprintf ( stderr, "Document not parsed successfully. \n" );
                return(NULL);
        }   
        cur = xmlDocGetRootElement ( doc );

        if ( cur == NULL )
        {   
                fprintf ( stderr, "empty document \n" );
                xmlFreeDoc ( doc );
                return(NULL);
        }   

        if ( xmlStrcmp ( cur->name, ( const xmlChar * ) "fix" ) )
        {
                fprintf ( stderr, "Document of the wrong type root node != ");
                xmlFreeDoc(doc);
                return (NULL);

        }
        cur = cur -> xmlChildrenNode;
        hashTable = init_hash_table(500);
        if (hashTable == NULL)
        {
            printf("Error: No hashTable initialized\n");
        }
        while ( cur != NULL )
        {
                if ( !(xmlStrcmp ( cur->name, ( const xmlChar * ) "fields" ) ) )
                {
                        //Get number of nodes
                        gNodeCount = xmlChildElementCount(cur);
                        //Allocate an array of size gNodeCount -1, set each entry to be is_string, is_double etc.
                        parse_fields ( doc, cur );
                }
                cur = cur -> next;
        }
        xmlFreeDoc ( doc );
        return (hashTable);
}

void parse_fields ( xmlDocPtr doc, xmlNodePtr cur )
{

        xmlChar *key;
        cur = cur -> xmlChildrenNode;
        while ( cur != NULL )
        {   
                if ( ( !xmlStrcmp ( cur -> name, ( const xmlChar * ) "field" ) ) ) 
                {
                    //Get the tagID as a char buffer 
                    char* tagNumber = xmlGetProp(cur, "number");
                    char* tagString = xmlGetProp( cur, "type");

                    //TODO - 2nd hashtable to lookup tagType from tagString
                    if ((strcmp(tagString, "STRING") == 0 )|| (strcmp(tagString, "CURRENCY" ) ==0 ) 
                        || (strcmp(tagString, "EXCHANGE") ==0) || (strcmp(tagString, "LOCALMKTDATE") ==0) 
                        || (strcmp(tagString, "MONTHYEAR")==0) || (strcmp(tagString, "MULTIPLEVALUESTRING") ==0 ) 
                        || (strcmp(tagString, "UTCDATE") ==0) || (strcmp(tagString, "UTCTIMEONLY") ==0 ) 
                        || (strcmp(tagString, "UTCTIMESTAMP") ==0 ))
                    {
                        add_string(hashTable, tagNumber, is_string);
                    }
                    else if ((strcmp(tagString, "INT") == 0)|| (strcmp(tagString, "DAYOFMONTH") ==0))
                    {
                        add_string(hashTable, tagNumber, is_int);
                    }
                    else if ((strcmp(tagString, "AMT") == 0) || (strcmp(tagString, "PRICE") ==0 ) 
                            || (strcmp(tagString, "QTY") == 0) || (strcmp(tagString, "PRICEOFFSET") ==0)
                            || (strcmp(tagString, "FLOAT") ==0))
                    {
                        add_string(hashTable, tagNumber, is_double);
                    }
                    else if ((strcmp(tagString, "CHAR") ==0) || (strcmp(tagString, "BOOLEAN") ==0))
                    {
                        add_string(hashTable, tagNumber, is_char);
                    }
                    else
                    {
                        printf("Error: Unrecognised tag value type %s\n", tagString);
                        //TODO: Add support for tagType "DATA"
                    }
                }   
                cur = cur -> next;
        }   
        return ;
}
