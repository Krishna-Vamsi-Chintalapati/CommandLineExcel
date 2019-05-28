#pragma once
#include "includes.h"

typedef struct cell {
	int value; 
	char* formula; 
} cell;

int get(cell**, char*); 
void set(cell**, char*, char*); 
void print(cell**); 
void import(cell**, FILE*); 
void exportSheet(cell**, FILE*); 
int evaluate(cell**, int, int); 
int calculate(cell**, char*); 
char* convertIntoPostfix(char*); 