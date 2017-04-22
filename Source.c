//TODO - negative fact check.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
double factorial(int number);
char* deleteIllegalChars(char* str);
void checkIllegalMath(char *str);
void checkForAllocationFailure(char *ptr);
double solveMath(char *problem);
char* insertVALUEintoSTRinsteadOfPTR1toPTR2(double value, char *str, char *ptr1, char *ptr2);
void strcpyUntilPlaceInMem(char *dest, char *source, char *place);
char* changeLettersToActualValue(char *str);
char* fixAsterisks(char *str);
char* insertAsteriskAfter(char *str, char *ptr);
void lowerCase(char *str);
char* modifyAndCheckForErrors(char *expression);
void main()
{
	char *test = (char *)calloc(1000, sizeof(char));
	strcpy(test, "2^-3");
	test = modifyAndCheckForErrors(test);
	solveMath(test);
}
double factorial(int number) //Returns the factorial of a given number.
{
	double result = 1;
	for (; number > 1; number--) result *= number;
	return result;
}
char* deleteIllegalChars(char* str) //Returns a given string with illegal mathematical characters removed.
{
	char *result = (char *)calloc(strlen(str) + 1, sizeof(char));
	unsigned int i, j;
	checkForAllocationFailure(result);
	for (i = 0, j = 0; i < strlen(str); i++)
		if (isdigit(str[i]) || str[i] == '.' || str[i] == 'p' || str[i] == 'e' || str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '!' || str[i] == '^' || str[i] == '/' || str[i] == '(' || str[i] == ')')
			result[j++] = str[i];
	free(str);
	result = (char *)realloc(result, ++j * sizeof(char));
	checkForAllocationFailure(result);
	return result;
}
void checkIllegalMath(char *str) //Prints an error message and exits the program if there are illegal math operations in str.
{
	unsigned int i, j;
	int parentheses = 0;
	for (i = 0; i < strlen(str) - 1; i++)
	{
		if ((str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '^' || str[i] == '/') && (str[i + 1] == '+' || str[i + 1] == '*' || str[i + 1] == '^' || str[i + 1] == '!' || str[i + 1] == '/'))
		{
			printf("ERROR: CONSECUTIVE OPERATORS.\n");
			free(str);
			exit(1);
		}
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
			for (j = i + 1; j < strlen(str) - 1; j++)
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
					else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '^' || str[i] == '/' || str[i] == '(' || str[i] == ')') break;
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
double solveMath(char *problem) //Returns the number the expression problem is equal to.
{
	unsigned int i, j, k;
	char *tempStr = (char *)calloc(30 * strlen(problem), sizeof(char));
	double tempNum1, tempNum2;
	checkForAllocationFailure(tempStr);
	//Calculating factorial.
	for (i = 0; i < strlen(problem); i++) {
		if (problem[i] == '!')
			for (j = i ? i - 1 : i; j >= 0; j--)
				if (j == 0 && isdigit(problem[j]))
				{
					strcpyUntilPlaceInMem(tempStr, problem, problem + i);
					problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(factorial(atoi(tempStr)), problem, problem, problem + i);
					printf("%s\n", problem);
					break;
				}
				else if (!isdigit(problem[j]))
				{
					strcpyUntilPlaceInMem(tempStr, problem + j + 1, problem + i);
					problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(factorial(atoi(tempStr)), problem, problem + j + 1, problem + i);
					i = 1;
					break;
				}
	}
	//Calculating power.
	for (i = 0; i < strlen(problem); i++)
		if (problem[i] == '^')
		{
			for (j = i ? i - 1 : i; j >= 0; j--)
				if (j == 0)
				{
					strcpyUntilPlaceInMem(tempStr, problem, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
				else if (!isdigit(problem[j]) && problem[j] != '.' && problem[j] != '-')
				{
					strcpyUntilPlaceInMem(tempStr, problem + j + 1, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
			for (k = i + 1; k < strlen(problem) + 1; k++)
				if (!isdigit(problem[k]) && problem[k] != '.' && problem[k] != '-')
				{
					strcpyUntilPlaceInMem(tempStr, problem + i + 1, problem + k);
					tempNum2 = atof(tempStr);
					break;
				}
			if (j)
				problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(pow(tempNum1, tempNum2), problem, problem + j + 1, problem + k - 1);
			else
				problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(pow(tempNum1, tempNum2), problem, problem, problem + k - 1);
			i = 1;
		}
	//Calculating multiplication and division.
	for (i = 0; i < strlen(problem); i++)
		if (problem[i] == '*' || problem[i] == '/')
		{
			for (j = i ? i - 1 : i; j >= 0; j--)
				if (j == 0)
				{
					strcpyUntilPlaceInMem(tempStr, problem, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
				else if (!isdigit(problem[j]) && problem[j] != '.' && problem[j] != '-')
				{
					strcpyUntilPlaceInMem(tempStr, problem + j + 1, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
			for (k = i + 1; k < strlen(problem) + 1; k++)
				if (!isdigit(problem[k]) && problem[k] != '.' && problem[k] != '-')
				{
					strcpyUntilPlaceInMem(tempStr, problem + i + 1, problem + k);
					tempNum2 = atof(tempStr);
					break;
				}
			if (j)
				problem = problem[i] == '*' ? insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 * tempNum2, problem, problem + j + 1, problem + k - 1) : insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 / tempNum2, problem, problem + j + 1, problem + k - 1);
			else
				problem = problem[i] == '*' ? insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 * tempNum2, problem, problem, problem + k - 1) : insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 / tempNum2, problem, problem, problem + k - 1);
			i = 1;
		}
	//Calculating addition and subtraction.
	for (i = 0; i < strlen(problem); i++)
		if (problem[i] == '+')
		{
			for (j = i ? i - 1 : i; j >= 0; j--)
				if (j == 0)
				{
					strcpyUntilPlaceInMem(tempStr, problem, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
				else if (!isdigit(problem[j]) && problem[j] != '.' && problem[j] != '-')
				{
					strcpyUntilPlaceInMem(tempStr, problem + j + 1, problem + i);
					tempNum1 = atof(tempStr);
					break;
				}
			for (k = i + 1; k < strlen(problem) + 1; k++)
				if (!isdigit(problem[k]) && problem[k] != '.' && problem[k] != '-')
				{
					strcpyUntilPlaceInMem(tempStr, problem + i + 1, problem + k);
					tempNum2 = atof(tempStr);
					break;
				}
			if (j)
				problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 + tempNum2, problem, problem + j + 1, problem + k - 1);
			else
				problem = insertVALUEintoSTRinsteadOfPTR1toPTR2(tempNum1 + tempNum2, problem, problem, problem + k - 1);
			i = 1;
		}
	tempNum1 = atof(problem)
	free(problem);
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
char* fixAsterisks(char *str) //Adds '*' to a given string in appropriate places.
{
	unsigned int i;
	for (i = 0; i < strlen(str); i++)
		if ((str[i] == 'e' || str[i] == 'p') && (isdigit(str[i + 1]) || str[i + 1] == 'e' || str[i + 1] == 'p'))
			str = insertAsteriskAfter(str, str + i);
	for (i = 1; i < strlen(str); i++)
		if ((str[i] == 'e' || str[i] == 'p') && (isdigit(str[i - 1]) || str[i - 1] == 'e' || str[i - 1] == 'p'))
			str = insertAsteriskAfter(str, str + i - 1);
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
char* changeLettersToActualValue(char *str) //Returns a pointer to a given string with 'p' as "3.141592653" and 'e' as "2.718281828".
{
	unsigned int i;
	for (i = 0; i < strlen(str); i++)
		if (str[i] == 'p') str = insertVALUEintoSTRinsteadOfPTR1toPTR2(3.141592653, str, str + i, str + i);
		else if (str[i] == 'e') str = insertVALUEintoSTRinsteadOfPTR1toPTR2(2.718281828, str, str + i, str + i);
	return str;
}
void lowerCase(char *str) //Converts every char to lower case.
{
	for (; *str; str++) *str = tolower(*str);
}
char* modifyAndCheckForErrors(char *expression) //Returns a given string so it will solvable. Unless it has illegal math operations in it.
{
	lowerCase(expression);
	expression = changeLettersToActualValue(fixAsterisks(deleteIllegalChars(expression)));
	checkIllegalMath(expression);
	return expression;
}