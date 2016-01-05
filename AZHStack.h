#pragma once

//status
#define RIGHT 1
#define ERROR 0
//limit
#define MAX_LINE_LEN 1001
#define MAX_VARIABLE_LEN 21
#define MAX_FIGURE_BITS 330


typedef struct MAOVariable {
	int type;
	char name[MAX_VARIABLE_LEN];
	int valueInt;
	double valueDouble;
	struct MAOVariable *nextVar;
}MAOVariable;

typedef struct MAOElement {
	int dataType;
	char optor;
	int valueInt;
	double valueDouble;
} MAOElement;
typedef struct MAOStack {
	MAOElement elementData;
	struct MAOStack *next;
}MAOStack, *PLinkStack;
typedef struct {
	PLinkStack top;
	int count;
}LinkStack;

typedef int States;
States IsStackEmpty(LinkStack linker) {
	if (linker.count == 0)
		return RIGHT;
	else
		return ERROR;
}
States InitStack(LinkStack *newStack) {
	newStack->top = (PLinkStack)calloc(1, sizeof(MAOStack));
	newStack->count = 0;
	if (!newStack)
		return ERROR;
	else
		return RIGHT;
}
States PushToStack(LinkStack *Stack, MAOElement element) {
	PLinkStack newNode = (PLinkStack)calloc(1, sizeof(MAOStack));
	newNode->elementData = element;
	newNode->next = Stack->top;
	Stack->top = newNode;
	Stack->count++;
	return RIGHT;
}
States PopFromStack(LinkStack *Stack, MAOElement *recoverData) {
	if (!Stack->top)
		return ERROR;
	*recoverData = Stack->top->elementData;
	Stack->top = Stack->top->next;
	Stack->count--;
	return RIGHT;
}

States ClearStack(LinkStack *Stack) {
	PLinkStack next_node;
	while (Stack->top->next) {
		next_node = Stack->top->next;
		free(Stack->top);
		Stack->top = next_node;
	}
	Stack->count = 0;
	return RIGHT;
}