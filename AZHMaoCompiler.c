#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "AZHStack.h"
#include "AZHTool.h"

void ReadFile();

/************************************************************************/
/* Program Entrance                                                     */
/************************************************************************/
int main(int argc, char *argv[]) {
	inStream = fopen(argv[1], "r");
	if (!inStream) {
		printf("Error: Can't find the file.");
		exit(1);
	}
	else
	{
		char * tempStr = argv[1];
		int inLength = strlen(tempStr);
		if (inLength<=4)
		{
			printf("Invalid file.");
		}
		else
		{
			
			if (tempStr[inLength-1]=='o'&&
				tempStr[inLength-2]=='a'&&
				tempStr[inLength-3]=='m'&&
				tempStr[inLength-4]=='.')
			{
				printf("***************************************************\n");
				printf("**************** Result As Follows ****************\n");
				printf("***************************************************\n\n\n");
				ReadFile();
			}
			else
			{
				printf("Invalid File Format! Can Only Read .mao file!!!");
			}
		}
	}
}

/************************************************************************/
/* Read File Function                                                   */
/************************************************************************/
void ReadFile()
{
	MAOVariable *currentVariable = NULL;
	MAOVariable *previousVariable = NULL;

	char *curline = (char *)calloc(MAX_LINE_LEN, sizeof(char));

	int isDeclaringFlag = 0;
	int tempCount = 0;
	GetOneLine(&curline);
	while (isDeclaringFlag = IsDeclaring(curline)) {
		do {
			currentVariable = (MAOVariable *)calloc(1, sizeof(MAOVariable));
			if (!firstVariament)
				firstVariament = currentVariable;
			if (previousVariable != NULL)
				previousVariable->nextVar = currentVariable;
			tempCount = VariableInit(isDeclaringFlag, curline, tempCount, currentVariable);
			currentVariable->nextVar = NULL;
			previousVariable = currentVariable;
		} while (tempCount != 0);
		GetOneLine(&curline);
	};
	MAOVariable *seekVar = (MAOVariable *)calloc(1, sizeof(MAOVariable));
	while (1)
	{
		if (IsPrintLine(curline))
		{
			Print(curline, seekVar);
		}
		else
		{
			tempCount = 0;
			CalcuEquation(curline, &tempCount);
		}
		GetOneLine(&curline);
	}
}