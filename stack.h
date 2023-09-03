#ifndef STACK_H
#define STACK_H

int isEmpty(int stack[], int top){
	if(top == -1)
		return 1;
	else
		return 0;
}

int isFull(int top, int max){
	if(top == max)
		return 1;
	else
		return 0;
}

int pop(int stack[], int* top){
	if(isEmpty(stack, *top)){ 
		perror("reverse: Error: Stack Empty! Can't pop stack");
		exit(0);
	} 
	
	int temp = stack[*top];
	(*top)--;
	return temp;		
	
}

void push(int input, int stack[], int* top, int max){
	if(isFull(*top, max)){
		printf("Stack is full! Can't push stack.");
	} else {
		(*top)++;
		stack[*top] = input;
	}
}

#endif
