#include "includes.h"
#include "excel.h"
#include "parse.h"

int main(void) {
	char* line = (char*)malloc(100 * sizeof(char)); 
	cell** sheet = (cell**)malloc(100 * sizeof(cell*)); 
	for (int i = 0; i < 10; i++)	
		sheet[i] = (cell*)malloc(100 * sizeof(cell));	
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			sheet[i][j].value = 0; 
	FILE* inputFile = NULL; 
	while (1) {
		printf(">"); 
		fgets(line, 100, stdin); 
		if (strlen(line) == 0)
			continue; 
		char* command = get_word(&line); 
		_strlwr(command); 
		parseCommand(command, &line, sheet, &inputFile); 
	}
	return 0; 
}