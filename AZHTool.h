#pragma once

/************************************************************************/
/* Some useful functions dealing the file and the identity.             */
/************************************************************************/

MAOVariable *firstVariament = NULL;
FILE *inStream = NULL;
void GetOneLine(char **line) {
	int tempCount = 0;
	for (tempCount = 0; tempCount < MAX_LINE_LEN; tempCount++)
		*(*line + tempCount) = 0;
	char tempChar;
	tempCount = 0;
	do {
		tempChar = fgetc(inStream);
		if (tempChar == EOF) {
			fclose(inStream);
			exit(0);
		}
		if (tempChar == ' ' || tempChar == '\t')
			if (!strcmp(*line, "int") || !strcmp(*line, "double"))
				**line = '#';
		if (!isspace(tempChar)) {
			*(*line + tempCount) = tempChar;
			tempCount++;
		}
	} while (tempChar != ';');
}

/************************************************************************/
/* Init a viriable with 0                                              */
/************************************************************************/
int VariableInit(int valueType, char *tempLine, int initialAdd, MAOVariable *curStruct) {
	int i = 0;
	curStruct->type = valueType;
	memset(curStruct->name, 0, MAX_VARIABLE_LEN * sizeof(char));
	if (initialAdd == 0)
		initialAdd = valueType == 1 ? 3 : 6;
	i = 0;
	do {
		if (isalnum(*(tempLine + initialAdd))) {
			curStruct->name[i] = *(tempLine + initialAdd);
			initialAdd++;
			i++;
		};
		if (strcmp("int", curStruct->name) == 0
			|| strcmp("double", curStruct->name) == 0
			|| strcmp("print", curStruct->name) == 0) {
			printf("Reserved Variable Name!");
			exit(2);
		}
		if (*(tempLine + initialAdd) == ',')
			return initialAdd + 1;
		if (*(tempLine + initialAdd) == ';')
			return 0;
	} while (1);
}

/************************************************************************/
/* Judge whether the line is declaring variables                        */
/************************************************************************/
int IsDeclaring(char *tempLine) {
	int type = 0;
	if (!strncmp(tempLine, "#nt", 3))
		type = 1;
	else if (!strncmp(tempLine, "#ouble", 6))
		type = 2;
	return type;
}
/************************************************************************/
/* Judging Operator                                                     */
/************************************************************************/
int GetOperator(char ch) {
	int no = 0;
	switch (ch) {
	case ';':
		no = -1;
		break;
	case '=':
		no = 1;
		break;
	case '(':
		no = 2;
		break;
	case '+':
	case '-':
		no = 3;
		break;
	case '*':
	case '/':
		no = 4;
		break;
	case'<':
	case'>':
		no = 5;
		break;
	case ')':
		no = 60;
		break;
	default:
		no = 0;
		break;
	}
	return no;
}
/************************************************************************/
/* Search for viriable from stack                                       */
/************************************************************************/
MAOVariable *SearchFromStack(char *name) {
	MAOVariable *checker = firstVariament;
	while (checker->nextVar != NULL) {
		if (!strcmp(checker->name, name))
			break;
		checker = checker->nextVar;
	}
	if (!checker) {
		printf("Error: Some Variable Undeclared!");
		fclose(inStream);
		exit(3);
	}
	else
		return checker;
}
/************************************************************************/
/* Judge wheter to print                                                */
/************************************************************************/
States IsPrintLine(char *tempLine) {
	int initialAdd = 5;
	if (!strncmp(tempLine, "print(", 6))
		return RIGHT;
	else
		return ERROR;
}
/************************************************************************/
/* Judge Whether it is a double viriable                                */
/************************************************************************/
States IsDouble(char *num) {
	unsigned int i;
	for (i = 0; i < strlen(num); i++)
		if (*(num + i) == '.')
			return RIGHT;
	return ERROR;
}
/************************************************************************/
/* Customized Print function                                            */
/************************************************************************/
void Print(char *tempLine, MAOVariable *foundVariable) {
	int initialAdd;
	int i = 0;
	char *content = (char *)calloc(MAX_FIGURE_BITS, sizeof(char));
	initialAdd = 6;
	while (*(tempLine + initialAdd) >= '+' && *(tempLine + initialAdd) <= '9'
		&& i < MAX_FIGURE_BITS) {
		*(content + i) = *(tempLine + initialAdd);
		i++;
		initialAdd++;
	}
	if (*content) {
		if (IsDouble(content)) {
			printf("%lf\n", atof(content));
			free(content);
			return;
		}
		else {
			printf("%d\n", atoi(content));
			free(content);
			return;
		}
	}
	while (*(tempLine + initialAdd) != ')') {
		*(content + i) = *(tempLine + initialAdd);
		i++;
		initialAdd++;
	}
	foundVariable = SearchFromStack(content);
	if (foundVariable->type == 1)
		printf("%d\n", foundVariable->valueInt);
	else
		printf("%lf\n", foundVariable->valueDouble);
	free(content);
	return;
}
/************************************************************************/
/* Caculator Function                                                   */
/************************************************************************/
void Calculator(LinkStack *Res, LinkStack *Oprtr, const int maxPri) {
	MAOElement elementOne, elementTwo, optHelper;
	double flagOne, flagTwo;

	while (!IsStackEmpty(*Oprtr)) {
		memset(&elementOne, 0, sizeof(MAOElement));
		memset(&elementTwo, 0, sizeof(MAOElement));
		memset(&optHelper, 0, sizeof(MAOElement));
		PopFromStack(Oprtr, &optHelper);
		if (optHelper.optor == '(')
			break;
		if (optHelper.optor == '<') {
			Res->top->elementData.valueDouble *= -1;
			Res->top->elementData.valueInt *= -1;
		}
		else if (optHelper.optor != '>') {
			PopFromStack(Res, &elementTwo);
			PopFromStack(Res, &elementOne);
			if (elementOne.dataType == 1)
				flagOne = elementOne.valueInt;
			else
				flagOne = elementOne.valueDouble;
			if (elementTwo.dataType == 1)
				flagTwo = elementTwo.valueInt;
			else
				flagTwo = elementTwo.valueDouble;
			switch (optHelper.optor) {
			case'+':
				flagOne += flagTwo;
				break;
			case'-':
				flagOne -= flagTwo;
				break;
			case'*':
				flagOne *= flagTwo;
				break;
			case'/':
				if (flagTwo == 0) {
					printf("Cannot Divided by 0\n");
					fclose(inStream);
					exit(4);
				}
				else
					flagOne /= flagTwo;
				break;
			default:
				break;
			}
			if (elementOne.dataType * elementTwo.dataType == 1)
				elementOne.valueInt = flagOne;
			else {
				elementOne.dataType = 2;
				elementOne.valueDouble = flagOne;
			}
			PushToStack(Res, elementOne);
		}
		if (maxPri != 60 && GetOperator(Oprtr->top->elementData.optor) < maxPri)
			break;
	}
}
double CalcuEquation(char *line, int *totalAdd) {
	LinkStack Oprtr;
	LinkStack Res;
	InitStack(&Oprtr);
	InitStack(&Res);
	MAOElement TempElem;

	int tempCount = 0;
	int i = 0;
	MAOVariable *seeker = firstVariament;
	char *content = (char *)calloc(MAX_FIGURE_BITS, sizeof(char));;
	do {
		memset(content, 0, MAX_FIGURE_BITS * sizeof(char));
		memset(&TempElem, 0, sizeof(MAOElement));
		i = 0;
		if (isalpha(*(line + tempCount))) {
			do {
				*(content + i) = *(line + tempCount);
				tempCount++;
				i++;
			} while (!GetOperator(*(line + tempCount)));
			seeker = SearchFromStack(content);
			TempElem.dataType = seeker->type;
			if (seeker->type == 1)
				TempElem.valueInt = seeker->valueInt;
			else
				TempElem.valueDouble = seeker->valueDouble;
			PushToStack(&Res, TempElem);
		}
		else if (isdigit(*(line + tempCount))) {
			do {
				*(content + i) = *(line + tempCount);
				tempCount++;
				i++;
			} while (!GetOperator(*(line + tempCount)));
			if (IsDouble(content)) {
				TempElem.dataType = 2;
				TempElem.valueDouble = atof(content);
			}
			else {
				TempElem.dataType = 1;
				TempElem.valueInt = atoi(content);
			}
			PushToStack(&Res, TempElem);
		}
		else if (*(line + tempCount) != '=') {
			if ((GetOperator(*(line + tempCount)) == 3)
				&& (GetOperator(*(line + tempCount - 1)) % 10 != 0)) {
				if (*(line + tempCount) == '-')
					*(line + tempCount) = '<';
				else
					*(line + tempCount) = '>';
			}
			TempElem.optor = *(line + tempCount);

			if (*(line + tempCount) == ')')
				Calculator(&Res, &Oprtr, GetOperator(*(line + tempCount)));
			else if (*(line + tempCount) == '(' || GetOperator(*(line + tempCount)) >
				GetOperator(Oprtr.top->elementData.optor))
				PushToStack(&Oprtr, TempElem);
			else {
				Calculator(&Res, &Oprtr, GetOperator(*(line + tempCount)));
				PushToStack(&Oprtr, TempElem);
			}

			tempCount++;
		}
		else {
			PopFromStack(&Res, &TempElem);
			if (seeker->type == 1) {
				seeker->valueInt = CalcuEquation(line + tempCount + 1,
					&tempCount);
				TempElem.valueInt = seeker->valueInt;
			}
			else {
				seeker->valueDouble = CalcuEquation(line + tempCount + 1, &tempCount);
				TempElem.valueDouble = seeker->valueDouble;
			}
			PushToStack(&Res, TempElem);
		}
	} while (*(line + tempCount) != ';');
	free(content);
	while (!IsStackEmpty(Oprtr))
		Calculator(&Res, &Oprtr, GetOperator(Oprtr.top->elementData.optor));
	PopFromStack(&Res, &TempElem);
	if (IsStackEmpty(Oprtr))
		ClearStack(&Oprtr);
	if (IsStackEmpty(Res))
		ClearStack(&Res);
	*totalAdd = *totalAdd + tempCount + 1;
	if (TempElem.dataType == 1)
		return (double)TempElem.valueInt;
	else
		return TempElem.valueDouble;
}
