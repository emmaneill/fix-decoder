

typedef union {
        int valueInt;
        float valueFloat;
        char valueString[16];
        char valueChar;
    } tagValue;
    
typedef enum {
        is_int = 0,
        is_string,
        is_double,
        is_char,
        is_unknown
    } tagType;
    
typedef struct tagEntry {
        int tagID;
        tagType type;
        tagValue value;
        struct tagEntry *next;
    } tagStruct;


