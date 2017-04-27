//Calculator by Yarden Shoham 2017
#define _CRT_SECURE_NO_WARNINGS
#define USER_INPUT_SIZE 1500
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
double factorial(int number);
char* fixAndDeleteIllegalChars(char* str, bool showChanges);
void checkIllegalMath(char *str);
void checkForAllocationFailure(char *ptr);
double solveMath(char *problem, bool showCalculations, unsigned spaces);
char* insertVALUEintoSTRinsteadOfPTR1toPTR2(double value, char *str, char *ptr1, char *ptr2);
void strcpyUntilPlaceInMem(char *dest, char *source, char *place);
char* changeLettersToActualValue(char *str, bool showChanges);
char* fixAsterisks(char *str, bool showChanges);
char* insertAsteriskAfter(char *str, char *ptr);
void lowerCase(char *str, bool showChanges);
char* modifyAndCheckForErrors(char *expression, bool showChanges);
double solveExpression(char *expression, bool showCalculations);
unsigned printBeautifully(char *s, unsigned spaces);
void myInputBuffFlush(void);
int main()
{
	char *userInput = (char *)calloc(USER_INPUT_SIZE, sizeof(char));
	bool showWork;
	char userChar;
	checkForAllocationFailure(userInput);
	printf("Welcome to Yarden's calculator! Enter a mathematical expression to get its result.\n*To enter the number Pi use 'p'.\n> ");
	fgets(userInput, USER_INPUT_SIZE, stdin);
	myInputBuffFlush();
	printf("Would you like the program to show you how it works? Enter Y or N.\n> ");
	userChar = toupper(getchar());
	myInputBuffFlush();
	while (userChar != 'Y' && userChar != 'N')
	{
		printf("Invalid input, try again.\n> ");
		userChar = toupper(getchar());
		myInputBuffFlush();
	}
	showWork = userChar == 'Y' ? true : false;
	userInput = modifyAndCheckForErrors(userInput, showWork);
	printf("The answer is %g.\n", solveExpression(userInput, showWork));
	return EXIT_SUCCESS;
}
double factorial(int number) //Returns the factorial of a given number.
{
	double result = 1;
	for (; number > 1; number--) result *= number;
	return result;
}
char* fixAndDeleteIllegalChars(char* str, bool showChanges) //Returns a given string with illegal mathematical characters removed and fixes weird parentheses.
{
	char *result = (char *)calloc(strlen(str) + 1, sizeof(char));
	unsigned int i, j;
	checkForAllocationFailure(result);
	for (i = 0, j = 0; i < strlen(str); i++)
		if (isdigit(str[i]) || str[i] == '.' || str[i] == 'p' || str[i] == 'e' || str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '!' || str[i] == '^' || str[i] == '/' || str[i] == '(' || str[i] == ')')
			result[j++] = str[i];
		else if (str[i] == '[' || str[i] == '{')
			result[j++] = '(';
		else if (str[i] == ']' || str[i] == '}')
			result[j++] = ')';
	if (showChanges && strcmp(result, str)) printBeautifully(result, 0);
	free(str);
	result = (char *)realloc(result, ++j * sizeof(char));
	checkForAllocationFailure(result);
	return result;
}
void checkIllegalMath(char *str) //Prints an error message and exits the program if there are illegal math operations in str.
{
	unsigned int i, j;
	int parentheses = 0;
	if (strlen(str) == 1)
	{
		printf("ERROR: EXPRESSION TOO SHORT.\n");
		free(str);
		exit(1);
	}
	if (!isdigit(*str) && *str != '-' && *str != '(')
	{
		printf("ERROR: FIRST OBJECT IS NOT A NUMBER.\n");
		free(str);
		exit(1);
	}
	if (!isdigit(str[strlen(str) - 1]) && str[strlen(str) - 1] != '!' && str[strlen(str) - 1] != ')')
	{
		printf("ERROR: LAST OBJECT IS NOT A NUMBER.\n");
		free(str);
		exit(1);
	}
	for (i = 0; i < strlen(str); i++)
	{
		if (str[i] == '(' && str[i + 1] == ')')
		{
			printf("ERROR: EMPTY PARENTHESES.\n");
			free(str);
			exit(1);
		}
		if (((str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '^' || str[i] == '/') && (str[i + 1] == '+' || str[i + 1] == '*' || str[i + 1] == '^' || str[i + 1] == '!' || str[i + 1] == '/')) || (str[i] == '-' && str[i + 1] == '-'))
		{
			printf("ERROR: CONSECUTIVE OPERATORS.\n");
			free(str);
			exit(1);
		}
		if (str[i] == '(' && str[i + 1] == '-')
			for (j = i + 2; j < strlen(str) - 1; j++)
				if (str[j] == ')' && str[j + 1] == '!')
				{
					printf("ERROR: NEGATIVE FACTORIAL.\n");
					free(str);
					exit(1);
				}
				else if (str[j] == '+' || str[j] == '-' || str[j] == '*' || str[j] == '^' || str[j] == '/' || str[j] == '(' || str[j] == ')') break;
				if (str[i] == '(') parentheses++;
				if (str[i] == ')') parentheses--;
				if (str[i] == '.')
				{
					if (i >= 1)
						if (!isdigit(str[i - 1]))
						{
							printf("ERROR: MISPLACED DECIMAL POINT.\n");
							free(str);
							exit(1);
						}
					if (!isdigit(str[i + 1]))
					{
						printf("ERROR: MISPLACED DECIMAL POINT.\n");
						free(str);
						exit(1);
					}
					for (j = i + 1; j < strlen(str); j++)
						if (str[j] == '.')
						{
							printf("ERROR: MISPLACED DECIMAL POINT.\n");
							free(str);
							exit(1);
						}
						else if (str[j] == '!')
						{
							printf("ERROR: DECIMAL POINT BEFORE FACTORIAL.\n");
							free(str);
							exit(1);
						}
						else if (str[j] == '+' || str[j] == '-' || str[j] == '*' || str[j] == '^' || str[j] == '/' || str[j] == '(' || str[j] == ')')
							break;
				}
	}
	if (parentheses)
	{
		printf("ERROR: UNEVEN AMOUNT OF PARENTHESES.\n");
		free(str);
		exit(1);
	}
}
void checkForAllocationFailure(char *ptr) //Exits from the program if ptr == NULL.
{
	if (!ptr)
	{
		printf("ERROR: MEMORY ALLOCATION FAILED.\n");
		exit(1);
	}
}
double solveMath(char *problem, bool showCalculations, unsigned spaces) //Returns the number the expression problem is equal to. Can't handle parentheses.
{
	unsigned int i, j, k;
	char *tempStr = (char *)calloc(15 * strlen(problem), sizeof(char));
	double tempNum1, tempNum2;
	checkForAllocationFailure(tempStr);
	//Calculating factorial.
	for (i = *problem == '-' ? 1 : 0; i < strlen(problem); i++) {
		if (problem[i] == '!')
			for (j = i ? i - 1 : i; j >= 0; j--)
				if (j == 0 && isdigit(problem[j]))
				{
					strcpyUntilPlaceInMem(tempStr, problem, problem + i);
					problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(factorial(atoi(tempStr)), problem, problem, problem + i);
					if (showCalculations) printBeautifully(problem, spaces);
					i = 0;
					break;
				}
				else if (!isdigit(problem[j]) && problem[j] != '.')
				{
					strcpyUntilPlaceInMem(tempStr, problem + j + 1, problem + i);
					problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(factorial(atoi(tempStr)), problem, problem + j + 1, problem + i);
					if (showCalculations) printBeautifully(problem, spaces);
					i = 0;
					break;
				}
	}
	//Calculating power.
	for (i = *problem == '-' ? 1 : 0; i < strlen(problem); i++)
		if (problem[i] == '^')
		{
			for (j = i ? i - 1 : i; j >= 0; j--)
				if (j == 0)
				{
					strcpyUntilPlaceInMem(tempStr, problem, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
				else if (!isdigit(problem[j]) && problem[j] != '.')
				{
					strcpyUntilPlaceInMem(tempStr, problem + j + 1, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
			for (k = problem[i + 1] == '-' ? i + 2 : i + 1; k < strlen(problem) + 1; k++)
				if (!isdigit(problem[k]) && problem[k] != '.')
				{
					strcpyUntilPlaceInMem(tempStr, problem + i + 1, problem + k);
					tempNum2 = atof(tempStr);
					break;
				}
			if (j)
				problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(pow(tempNum1, tempNum2), problem, problem + j + 1, problem + k - 1);
			else
				problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(pow(tempNum1, tempNum2), problem, problem, problem + k - 1);
			if (showCalculations) printBeautifully(problem, spaces);
			i = *problem == '-' ? 0 : -1;
		}
	//Calculating multiplication and division.
	for (i = *problem == '-' ? 1 : 0; i < strlen(problem); i++)
		if (problem[i] == '*' || problem[i] == '/')
		{
			for (j = i ? i - 1 : i; j >= 0; j--)
				if (j == 0)
				{
					strcpyUntilPlaceInMem(tempStr, problem, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
				else if (!isdigit(problem[j]) && problem[j] != '.')
				{
					strcpyUntilPlaceInMem(tempStr, problem + j + 1, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
			for (k = problem[i + 1] == '-' ? i + 2 : i + 1; k < strlen(problem) + 1; k++)
				if (!isdigit(problem[k]) && problem[k] != '.')
				{
					strcpyUntilPlaceInMem(tempStr, problem + i + 1, problem + k);
					tempNum2 = atof(tempStr);
					break;
				}
			if (j)
				problem = problem[i] == '*' ? insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 * tempNum2, problem, problem + j + 1, problem + k - 1) : insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 / tempNum2, problem, problem + j + 1, problem + k - 1);
			else
				problem = problem[i] == '*' ? insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 * tempNum2, problem, problem, problem + k - 1) : insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 / tempNum2, problem, problem, problem + k - 1);
			if (showCalculations) printBeautifully(problem, spaces);
			i = *problem == '-' ? 0 : -1;
		}
	//Calculating addition and subtraction.
	for (i = *problem == '-' ? 1 : 0; i < strlen(problem); i++)
		if (problem[i] == '+' || problem[i] == '-')
		{
			for (j = i ? i - 1 : i; j >= 0; j--)
				if (j == 0)
				{
					strcpyUntilPlaceInMem(tempStr, problem, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
				else if (!isdigit(problem[j]) && problem[j] != '.')
				{
					strcpyUntilPlaceInMem(tempStr, problem + j + 1, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
			for (k = problem[i + 1] == '-' ? i + 2 : i + 1; k < strlen(problem) + 1; k++)
				if (!isdigit(problem[k]) && problem[k] != '.')
				{
					strcpyUntilPlaceInMem(tempStr, problem + i + 1, problem + k);
					tempNum2 = atof(tempStr);
					break;
				}
			if (problem[i] == '+') {
				if (j)
					problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 + tempNum2, problem, problem + j + 1, problem + k - 1);
				else
					problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 + tempNum2, problem, problem, problem + k - 1);
			}
			else
			{
				if (j)
					problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 - tempNum2, problem, problem + j + 1, problem + k - 1);
				else
					problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 - tempNum2, problem, problem, problem + k - 1);
			}
			if (showCalculations) printBeautifully(problem, spaces);
			i = *problem == '-' ? 0 : -1;
		}
	tempNum1 = atof(problem);
	free(problem);
	free(tempStr);
	return tempNum1;
}
void strcpyUntilPlaceInMem(char *dest, char *source, char *place) //Copies source to dest until reaching either the pointer place or '\0'.
{
	for (;*source && source != place; source++, dest++) *dest = *source;
	*dest = '\0';
}
char* insertVALUEintoSTRinsteadOfPTR1toPTR2(double value, char *str, char *ptr1, char *ptr2) //Returns a given string with the part from ptr1 to ptr2 replaced by value.
{
	char *part1 = (char *)calloc(strlen(str) + 1, sizeof(char)), *part2 = (char *)calloc(strlen(str) + 1, sizeof(char)), *result = (char *)calloc(10 * strlen(str), sizeof(char));
	checkForAllocationFailure(part1);
	checkForAllocationFailure(part2);
	checkForAllocationFailure(result);
	strcpyUntilPlaceInMem(part1, str, ptr1);
	if (ptr2 + 1 <= str + strlen(str))
		strcpy(part2, ptr2 + 1);
	else
		strcpy(part2, "\0");
	sprintf(result, "%s%f%s", part1, value, part2);
	free(part1);
	free(part2);
	free(str);
	result = (char *)realloc(result, (strlen(result) + 1) * sizeof(char));
	checkForAllocationFailure(result);
	return result;
}
char* fixAsterisks(char *str, bool showChanges) //Adds '*' to a given string in appropriate places.
{
	char initialString[USER_INPUT_SIZE];
	strcpy(initialString, str);
	unsigned int i;
	for (i = 0; i < strlen(str); i++)
		if (((str[i] == 'e' || str[i] == 'p') && (isdigit(str[i + 1]) || str[i + 1] == 'e' || str[i + 1] == 'p' || str[i + 1] == '(')) || (isdigit(str[i]) && str[i + 1] == '('))
			str = insertAsteriskAfter(str, str + i);
	for (i = 1; i < strlen(str); i++)
		if (((str[i] == 'e' || str[i] == 'p') && (isdigit(str[i - 1]) || str[i - 1] == 'e' || str[i - 1] == 'p' || str[i - 1] == ')')) || (str[i - 1] == ')' && isdigit(str[i])))
			str = insertAsteriskAfter(str, str + i - 1);
	if (showChanges && strcmp(initialString, str)) printBeautifully(str, 0);
	return str;
}
char* insertAsteriskAfter(char *str, char *ptr) //Returns a given string with '*' inserted immediately after ptr. 
{
	char *part1 = (char *)calloc(strlen(str) + 1, sizeof(char)), *part2 = (char *)calloc(strlen(str) + 1, sizeof(char)), *result = (char *)calloc(10 * strlen(str), sizeof(char));
	checkForAllocationFailure(part1);
	checkForAllocationFailure(part2);
	checkForAllocationFailure(result);
	strcpyUntilPlaceInMem(part1, str, ptr + 1);
	strcpy(part2, ptr + 1);
	sprintf(result, "%s*%s", part1, part2);
	free(part1);
	free(part2);
	free(str);
	result = (char *)realloc(result, (strlen(result) + 1) * sizeof(char));
	checkForAllocationFailure(result);
	return result;
}
char* changeLettersToActualValue(char *str, bool showChanges) //Returns a pointer to a given string with 'p' as "3.14159265359" and 'e' as "2.71828182845".
{
	unsigned int i;
	char initialString[USER_INPUT_SIZE];
	strcpy(initialString, str);
	for (i = 0; i < strlen(str); i++) {
		if (str[i] == 'p') str = insertVALUEintoSTRinsteadOfPTR1toPTR2(3.14159265359, str, str + i, str + i);
		else if (str[i] == 'e') str = insertVALUEintoSTRinsteadOfPTR1toPTR2(2.718281828, str, str + i, str + i);
	}
	if (showChanges && strcmp(initialString, str)) printBeautifully(str, 0);
	return str;
}
void lowerCase(char *str, bool showChanges) //Converts every char to lower case.
{
	char initialString[USER_INPUT_SIZE], *initialStringPointer = str;
	strcpy(initialString, str);
	for (; *initialStringPointer; initialStringPointer++) *initialStringPointer = tolower(*initialStringPointer);
	if (showChanges && strcmp(initialString, str)) printBeautifully(str, 0);
}
char* modifyAndCheckForErrors(char *expression, bool showChanges) //Returns a given string so it will solvable. Unless it has illegal math operations in it.
{
	lowerCase(expression, showChanges);
	expression = changeLettersToActualValue(fixAsterisks(fixAndDeleteIllegalChars(expression, showChanges), showChanges), showChanges);
	checkIllegalMath(expression);
	return expression;
}
double solveExpression(char *expression, bool showCalculations) //Returns the number the expression problem is equal to.
{
	unsigned i, j, spaces;
	char *tempStr;
	//Simplify all parentheses.
	for (i = 1; i < strlen(expression); i++)
		if (expression[i] == ')')
			for (j = i - 1; 1; j--)
				if (expression[j] == '(')
				{
					tempStr = (char *)calloc(strlen(expression) + 1, sizeof(char));
					checkForAllocationFailure(tempStr);
					strcpyUntilPlaceInMem(tempStr, expression + j + 1, expression + i);
					if (showCalculations) spaces = printBeautifully(expression, USER_INPUT_SIZE);
					expression = insertVALUEintoSTRinsteadOfPTR1toPTR2(solveMath(tempStr, showCalculations, spaces), expression, expression + j, expression + i);
					if (showCalculations) printBeautifully(expression, 0);
					i = 1;
					break;
				}
	//Calculate final value.
	return solveMath(expression, showCalculations, 0);
}
unsigned printBeautifully(char *s, unsigned spaces)
{
	unsigned i, j, point, printIndex = 0;
	char tempNum[USER_INPUT_SIZE], stringToPrint[USER_INPUT_SIZE] = { 0 };
	if (spaces != USER_INPUT_SIZE) for (i = 0; i < spaces; i++) putchar(' ');
	for (i = 0; s[i]; i++)
	{
		if (isdigit(s[i]))
		{
			for (j = i + 1; j <= strlen(s); j++)
				if (!isdigit(s[j]) && s[j] != '.')
				{
					strcpyUntilPlaceInMem(tempNum, s + i, s + j);
					break;
				}
			i = j - 1;
			if (fmod(atof(tempNum), 1.0)) //If floating point number.
			{
				for (j = strlen(tempNum) - 1; tempNum[j] == '0'; j--);
				tempNum[j + 1] = '\0';
				for (point = 1; tempNum[point] != '.'; point++);
				if (strlen(tempNum + point + 1) == 1) sprintf(stringToPrint + printIndex, tempNum);
				else sprintf(stringToPrint + printIndex, "%.2f", atof(tempNum));
			}
			else //If integer.
				sprintf(stringToPrint + printIndex, "%g", atof(tempNum));
			printIndex = strlen(stringToPrint);
		}
		else
		{
			if (s[i] == '+' && s[i + 1] == '-') i++;
			sprintf(stringToPrint + printIndex, "%c", s[i]);
			printIndex++;
		}
	}
	if (spaces != USER_INPUT_SIZE)
	{
		printf(stringToPrint);
		putchar('\n');
	}
	for (i = 1; i < printIndex; i++)
		if (stringToPrint[i] == ')')
			for (; ; i--)
				if (stringToPrint[i] == '(')
					return (i + 1);
	return 0;
}
void myInputBuffFlush(void) //Cleans the buffer.
{
	int i, size, ch;
	fseek(stdin, 0, SEEK_END);
	size = ftell(stdin);
	rewind(stdin);
	for (i = 0; i < size; i++)
		ch = getchar();
}
