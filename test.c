#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "test.h"


unsigned char   gDelim = (unsigned char)0x01;
int             gMsgCount =0;
hash_table_t*   gHashTable=NULL;

extern hash_table_t* parse_doc(char *docname);

void append(char* s, char c)
{
        if (*s)
        {
            int len = strlen(s);
            s[len] = c;
            s[len+1] = '\0';
        }
        else
        {
            s[0] = c;
            s[1] = '\0';
        }
}

void find_delim(char** ptr)
{
    int found =0;
    while (**ptr)
    {
        if (**ptr == gDelim)
        {
            ++(*ptr);
            found = 1;
            break;
        }
        else
        {
            ++(*ptr);
        }
    }
}

void parse_string_value(char** ptr, char* valStr)
{
    valStr[0] = '\0';

    while(**ptr != gDelim)
    {
        if(**ptr!= '=')
        {
            append(valStr, **ptr);
        }
        ++(*ptr);
    }
}

char* get_next_tag( char** ptr)
{
    int tagLen = 0;
    char* buf = malloc(4);
    *(char*)buf = 0;
    
    while (**ptr)
    {
        if (**ptr != '=')
        {
            ++(*ptr);
            ++tagLen;
        }
        else
        {
            strncat(buf, *ptr-tagLen, tagLen);
            break;
        }
    }
    if (strcmp(buf, "8") ==0)
    {
        gMsgCount++;
        printf("Msgcount: %d\n", gMsgCount);

    }
    //Push pointer past '=' 
    ++(*ptr);
    return (buf);
}


int main(){

   gHashTable= parse_doc("/var/userspace/eneill/phi/c/test-data/fix.xml");
    const char *filename="/var/userspace/eneill/phi/c/test-data/fix.example";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) 
    {
        char tmpString[100];
        snprintf(tmpString, sizeof tmpString, "Failed to open file %s", filename);
        perror(tmpString);
        return 1;
    }

    int bodyLen =0;
    int controlChar[3];
    char buf[1000];


    char testSource[10000 + 1];
    char source[MAXBUFLEN + 1];

    int arrayList[447];

    if (fp != NULL) 
    {
        size_t newLen;
        int begin = 1;
        int foundDelim = 0;
        while( (newLen = fread(source, sizeof(char), MAXBUFLEN, fp)) > 0)
        {
           char *ptr;
           ptr = source;
           char* tagID = malloc(4 * sizeof(char));
           int remaining = strlen(ptr);
           if (newLen < MAXBUFLEN)
           {
                ptr[newLen-1] = '\0';
           }
            
            if (begin)
            {
                //No SOH at the beginning of the file, need to find first tag (beginString)
                tagID = get_next_tag(&ptr);
                if (*tagID != '8')
                {
                    printf("Error: Did not find beginString field at start of file\n");
                    return 2;
                }
                printf("TagID : %s ", tagID);
                tagType type= find_tag_type(gHashTable, tagID);
                char valStr[16];
                switch (type)
                {
                    case is_string:
                    {
                        parse_string_value(&ptr, valStr);
                        printf("String value is %s \n", valStr);
                        break;
                    }
                    default:
                    {
                        printf("Error: Did not find string field at start of file\n");
                        return 2;
                    }
                }   
                begin = 0;
                find_delim(&ptr);
            }
                  
           //Delimit based on '=' and <SOH> control char
           //Or we are at end of the file
           int endOfFile = (feof(fp));
           while (((remaining >= 30) || (endOfFile)) && (ptr != '\0'))
           {
                tagID = get_next_tag(&ptr);
                //Based on tag.type - call parseValue
                printf("TagID : %s", tagID);
               tagType type= find_tag_type(gHashTable, tagID);

               switch (type)
                {
                    case is_string:
                    {
                        char valStr[16];
                        parse_string_value(&ptr, valStr);
                        printf("String value is %s \n", valStr);
                        break;
                    }
                    case is_int:
                    {
                        int valInt;
                        valInt = atoi(ptr);
                        printf("Int value is %d \n", valInt);
                        break;
                    }
                    case is_double:
                    {
                        double num = strtod(ptr, NULL);
                        printf("Double values is %f\n", num);
                        break;
                    }
                    case is_char:
                    {
                        char tmpChar;
                        tmpChar = ptr[0];
                        printf("Char value is %c\n", tmpChar);
                        break;
                    }
                }
                /*Push pointer to <SOH> or end of field value */
                find_delim(&ptr);

                if (!*ptr)
                {
                    return 0;
                }
                remaining = strlen(ptr) ;
            }
            //Don't rewind input file if we're in the last buffer section of the file
           if ((newLen == MAXBUFLEN) && (remaining <= 30) && (!endOfFile))
           {
              fseek(fp, remaining*(-1), SEEK_CUR);
           }
                free(tagID);
        }
    }
     
        fclose(fp);
        return 0;

}
