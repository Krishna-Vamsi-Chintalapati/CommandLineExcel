#include "includes.h"
#include "excel.h"
#include "parse.h"
#include <stack>

int getCol(char* col) {
	int val = 0; 
	_strlwr(col); 
	for (int i = 0; col[i] != '\0' && col[i] != '\n'; i++) {
		val = val * 10 + (col[i] - 'a' + 1); 
	}
	return val - 1; 
}

void set(cell** sheet, char* cell, char* expression) {
	char* val = (char*)malloc(10 * sizeof(char));
	int pos = 0;
	int i = 0; 
	while (isalpha(cell[i])) {
		val[pos] = cell[i];
		i++; 
		pos++;
	}
	val[pos] = '\0';
	int col = getCol(val);
	pos = 0;
	while (isdigit(cell[i])) {
		val[pos] = cell[i];
		pos++;
		i++;
	}
	val[pos] = '\0';
	int row = atoi(val) - 1;
	if (!isnum(expression)) {
		sheet[row][col].formula = expression;
		sheet[row][col].value = -1; 
		return; 
	}
	sheet[row][col].value = atoi(expression); 
}

int get(cell** sheet, char* cell) {
	char* val = (char*)malloc(10 * sizeof(char));
	int pos = 0;
	int i = 0;
	while (isalpha(cell[i])) {
		val[pos] = cell[i];
		i++;
		pos++;
	}
	val[pos] = '\0';
	int col = getCol(val);
	pos = 0;
	while (isdigit(cell[i])) {
		val[pos] = cell[i];
		pos++;
		i++;
	}
	val[pos] = '\0';
	int row = atoi(val) - 1;
	if (sheet[row][col].value != -1)
		return sheet[row][col].value; 
	return evaluate(sheet, row, col); 
}

void import(cell** sheet, FILE* file) {
	int i = 0, j = 0; 
	char* expression = (char*)malloc(20 * sizeof(char)); 
	int ch; 
	int pos = 0; 
	ch = getc(file); 
	while (!feof(file) && ch != EOF) {
		if (ch == ',') {
			expression[pos++] = '\0'; 
			sheet[i][j].formula = expression; 
			sheet[i][j].value = -1; 
			expression = (char*)malloc(20 * sizeof(char));
			j++; 
			pos = 0;
		}
		else if (ch == '\n') {
			expression[pos++] = '\0';
			sheet[i][j].formula = expression;
			sheet[i][j].value = -1;
			expression = (char*)malloc(20 * sizeof(char));
			i++; 
			j = 0; 
			pos = 0; 
		}
		else {
			expression[pos++] = ch; 
		}
		ch = fgetc(file);
	}
}

void exportSheet(cell** sheet, FILE* file) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 9; j++) {
			if (sheet[i][j].value == -1)
				fprintf_s(file, "%s,", sheet[i][j].formula);
			else
				fprintf_s(file, "%d,", sheet[i][j].value);
		}
		fprintf_s(file, "%d\n", sheet[i][9].value); 
	}
	fclose(file); 
}

void print(cell** sheet) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (sheet[i][j].value != -1)
				printf("%5d ", sheet[i][j].value);
			else
				printf("%5d ", evaluate(sheet, i, j)); 
		}
		printf("\n"); 
	}
}

int evaluate(cell** sheet, int row, int col) {
	if (sheet[row][col].value != -1)
		return sheet[row][col].value;
	return calculate(sheet, sheet[row][col].formula);
}

int applyOp(int a, int b, char op) {
	switch (op) {
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	case '/': return a / b;
	}
}

int isoperator(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
		return 1; 
	return 0; 
}

int precedence(char op) {
	if (op == '+' || op == '-')
		return 1;
	if (op == '*' || op == '/')
		return 2;
	return 0;
}

int calculate(cell** sheet, char* formula) {
	int i;
	std::stack <int> values;
	std::stack <char> ops;

	for (i = 0; i < formula[i] != '\0' && formula[i] != '\n'; i++) {
		if (formula[i] == '(') {
			ops.push(formula[i]);
		}
		else if (isdigit(formula[i])) {
			int val = 0;
			while (i < formula[i] != '\0' && formula[i] !='\n' && isdigit(formula[i])) {
				val = (val * 10) + (formula[i] - '0');
				i++;
			}
			values.push(val);
			i--; 
		}
		else if (formula[i] == ')') {
			while (!ops.empty() && ops.top() != '(') {
				int val2 = values.top();
				values.pop();

				int val1 = values.top();
				values.pop();

				char op = ops.top();
				ops.pop();

				values.push(applyOp(val1, val2, op));
			}
			ops.pop();
		}
		else if (formula[i] >= 'a' && formula[i] <= 'z') {
			char* operand = (char*)malloc(50 * sizeof(int)); 
			int pos = 0; 
			while ((formula[i] >= 'a' && formula[i] <= 'z') || (isdigit(formula[i])))
				operand[pos++] = formula[i++]; 
			operand[pos] = '\0'; 
			i--; 
			char* val = (char*)malloc(10 * sizeof(char));
			int k = 0;
			int index = 0;
			while (isalpha(operand[index])) {
				val[k] = operand[index];
				index++;
				k++;
			}
			val[k] = '\0';
			int col = getCol(val);
			k = 0;
			while (isdigit(operand[index])) {
				val[k] = operand[index];
				k++;
				index++;
			}
			val[k] = '\0'; 
			int row = atoi(val) - 1; 
			values.push(evaluate(sheet, row, col)); 
		}
		else {
			while (!ops.empty() && precedence(ops.top()) >= precedence(formula[i])) {
				int val2 = values.top();
				values.pop();

				int val1 = values.top();
				values.pop();

				char op = ops.top();
				ops.pop();

				values.push(applyOp(val1, val2, op));
			}

			ops.push(formula[i]);
		}
	}
	while (!ops.empty()) {
		int val2 = values.top();
		values.pop();

		int val1 = values.top();
		values.pop();

		char op = ops.top();
		ops.pop();

		values.push(applyOp(val1, val2, op));
	}
	return values.top();
}