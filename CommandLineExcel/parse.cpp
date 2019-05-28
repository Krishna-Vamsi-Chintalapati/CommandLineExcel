#include "includes.h"
#include "parse.h"
#include "excel.h"

char* removeSpaces(char** line) {
	int pos = 0; 
	char* ans = (char*)malloc(strlen(*line) * sizeof(char)); 
	while (**line != '\0' && **line != '\n') {
		if (**line != ' ')
			ans[pos++] = **line; 
		(*line)++; 
	}
	ans[pos] = '\0'; 
	return ans; 
}

int isnum(char* expression) {
	int pos = 0;
	for (pos = 0; expression[pos] != '\0' && expression[pos] != '\n'; pos++)
		if (expression[pos] < '0' || expression[pos] > '9')
			return 0;
	return 1;
}

int isalpha(char ch) {
	if ((ch < 'a' || ch > 'z') && (ch < 'A' || ch > 'Z'))
		return 0; 
	return 1; 
}

int isdigit(char ch) {
	if (ch < '0' || ch > '9')
		return 0;
	return 1;
}

void get_operator(char** line) {
	while (**line == ' ')
		(*line)++; 
	(*line)++; 
}

void parseCommand(char* command, char** line, cell** sheet, FILE** inputFile) {
	if (!(strcmp(command, "set"))) {
		char* cell = get_word(line);
		get_operator(line);
		char* expression = removeSpaces(line); 
		set(sheet, cell, expression);
	}
	else if (!(strcmp(command, "get"))) {
		char* cell = get_word(line); 
		char* remaining = get_word(line); 
		if (remaining[0] != '\0') {
			printf("Invalid command format\n"); 
			return; 
		}
		printf("%d\n", get(sheet, cell)); 
	}
	else if (!(strcmp(command, "print"))) {
		char* remaining = get_word(line); 
		if (remaining[0] != '\0') {
			printf("Invalid command format\n"); 
			return; 
		}
		print(sheet); 
	}
	else if (!(strcmp(command, "import"))) {
		char* fileName = get_word(line); 
		strcat(fileName, ".csv"); 
		*inputFile = fopen(fileName, "r+"); 
		if (*inputFile == NULL) {
			printf("File does not exist\n"); 
			return; 
		}
		import(sheet, *inputFile); 
	}
	else if (!(strcmp(command, "export"))) {
		char* fileName = get_word(line); 
		char* remaining = get_word(line); 
		strcat(fileName, ".csv"); 
		if (remaining[0] != '\0') {
			printf("Invalid command format\n"); 
			return; 
		}
		FILE* fileptr = fopen(fileName, "w+"); 
		exportSheet(sheet, fileptr); 
	}
	else if (!(strcmp(command, "save"))) {
		if (*inputFile == NULL) {
			printf("No file is imported\n"); 
			return; 
		}
		exportSheet(sheet, *inputFile); 
	}
	else if (!(strcmp(command, "exit"))) {
		exit(0); 
	}
	else {
		printf("Invalid command\n"); 
		return; 
	}
}

char* get_word(char** line) {
	if (line == NULL)
		return NULL;
	while (**line == ' ')
		(*line)++;
	char* word = (char*)malloc(10 * sizeof(char));
	int pos = 0;
	while ((**line >= 'a' && **line <= 'z') || (**line >= '0' && **line <= '9')) {
		word[pos++] = **line;
		(*line)++;
	}
	word[pos] = '\0';
	return word;
}