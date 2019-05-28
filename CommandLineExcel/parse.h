#include "includes.h"
#include "excel.h"

void parseCommand(char*, char**, cell**, FILE**); 
char* get_word(char**); 
char* removeSpaces(char**); 
int isdigit(char); 
int isalpha(char); 
int isnum(char*);