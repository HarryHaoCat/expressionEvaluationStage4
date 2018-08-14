/*************************************************************************
> File Name: expressionEvaluation1.c
> Author: HaoJie
> Mail: 954487858@qq.com
> Created Time: 2018/08/14  17h22m3s
************************************************************************/
#include <stdio.h>
#include "sequentialStack.h"
#include <ctype.h>
int Priority(char c);                                 //Calculate the priority value of the operators.
void printError(char* infixExpression, int index);    //Mark the location of error.
/*
The function converts infix expression into postfix expression.
Firstly, check if the infix expression is legal, if it's illegal, print error information and return FAILED.
If the infix expresssion is legal, convert it into postfix expression and retuen SUCCEEDED.
*/
int infixToPostfix(char* infixExpression, char postfixExpression[]) {
	for (int i = 0; i < 100; i++)
		postfixExpression[i] = '\0';
	int braceNum = 0;
	char preChar = 's';
	SequentialStackChar* s = (SequentialStackChar*)malloc(sizeof(SequentialStackChar));
	ElemType e;
	if (InitStack(s) == FAILED) {
		printf("<Error! Initialize stack fails>\n");
		return FAILED;
	}
	int numIndex = 0, index = 0;
	int operand = 0, blank = 0, leftBrace = 0, rightBrace = 0;
	while (infixExpression[index] != '\0') {
		/*
		Check whether the current character is illegal,
		if the current character isn't '+', '-', '*', '/', ' ', '\t', '(', ')' or number '0'-'9',
		then print proper error information and return FAILED to indicate illegal characters in the expression.
		*/
		if (!(isdigit(infixExpression[index]) || infixExpression[index] == '+' || infixExpression[index] == ' '
			|| infixExpression[index] == '-' || infixExpression[index] == '*' || infixExpression[index] == '/' 
			|| infixExpression[index] == '(' || infixExpression[index] == ')' || infixExpression[index] == '\t')) {
			printError(infixExpression, index);
			if ((infixExpression[index] > 0 && infixExpression[index] < 9)
				|| (infixExpression[index] >= 10 && infixExpression[index] < 32) || infixExpression[index] == 127){
				if (infixExpression[index] == '\b') 
					printf("<Error! Illegal escape character \'\\%o\' after \'%c\' in the expression>\n", infixExpression[index], infixExpression[index - 2]);
				else printf("<Error! Illegal escape character \'\\%o\' in the expression>\n", infixExpression[index]);
			}
			else printf("<Error! Illegal character \'%c\' in the expression>\n", infixExpression[index]);
			return FAILED;
		}
		/*
		Handle space and '\t'.
		If the last character is space or '\t' with existing operator before it, return FAILED and error message.
		*/
		else if (infixExpression[index] == ' ' || infixExpression[index] == '\t') {
			blank++;
			if(infixExpression[index + 1] == '\0' && (preChar == '+' || preChar == '-' || preChar == '*' || preChar == '/')){
				printf("<Error! No operand after operator \'%c\'>\n", preChar);
				return FAILED;
			}
		}
		/*
		If the current character is a digit,
			(1)the effective character before it is also a digit and no operator between them,
			(2)a brace ')' before a digit, and there is no operator between them,   
		else it's a legal digit, put it into the array postfixExpression.
		*/
		else if (isdigit(infixExpression[index])) {
			if ((infixExpression[index - 1] == ' ' || infixExpression[index - 1] == '\t')&& isdigit(preChar)) {
				printError(infixExpression, index - 1);
				printf("<Error! A space between two operands>\n");
				return FAILED;
			}
			else if(preChar == ')'){
				printError(infixExpression, index);
				printf("<Error! No operator between \')\' and operand>\n");
				return FAILED;			}
			else {
				postfixExpression[numIndex++] = infixExpression[index];
				if (!isdigit(infixExpression[index + 1])) {
					postfixExpression[numIndex++] = '#';
					operand++;
				}
			}
		}
		//If the current character is operator or brace,
		else if (infixExpression[index] == '+' || infixExpression[index] == '-' || infixExpression[index] == '*'
			|| infixExpression[index] == '/' || infixExpression[index] == '(' || infixExpression[index] == ')') {
			/*Handle illegal unary operator '+' and '-'.
			(1)A space follows a unary operator '+' or '-'.
			(2)An operaor '+', '-', '*', '/' follows an operator '+' or '-'.
			(3)There is only one operator '+' or '-' in the expression without operand.
			*/
			if (infixExpression[index] == '+' || infixExpression[index] == '-') {
				if ((preChar == 's' || preChar== '(') && (infixExpression[index + 1] == ' ' || infixExpression[index + 1] == '\t')) {
					printError(infixExpression, index + 1);
					printf("<Error! A space follows a unary \'%c\'>\n", infixExpression[index]);
					return FAILED;
				}
				else if (preChar == '+' || preChar == '-' || preChar == '*' || preChar == '/') {
					printError(infixExpression, index);
					printf("<Error! Operator \'%c\'  follows another operator \'%c\' in the expression>\n", infixExpression[index], preChar);
					return FAILED;
				}
				else if(infixExpression[index + 1] == '\0'){
					printError(infixExpression, index);
					printf("<Error! No operand after operator \'%c\'>\n", infixExpression[index]);
					return FAILED;
				}
			}
			/*
			Handle illegal brace ')'.
			(1)There is an operator before brace ')' without operand.
			(2)No expression in the braces.
			(3)No matched brace before the current brace.
			*/
			else if (infixExpression[index] == ')') {
				braceNum--;
				if(preChar == '+' || preChar == '-' || preChar == '*' || preChar == '/'){
					printError(infixExpression, index - 1);	
					printf("<Error! No operand after operator \'%c\'>\n", preChar);
					return FAILED;
				}
				else if(preChar == '('){
					printError(infixExpression, index - 1);	
					printf("<Error! No expression in the braces>\n");
					return FAILED;				}
				else if (preChar == 's' || braceNum < 0) {
					printError(infixExpression, index);
					printf("<Error! No matched \'(\' before \')\'>\n");
					return FAILED;
				}
			}
			/*
			Handle illegal brace '('.
			(1)There is a brace ')' before a '(' without operator between them.
			(2)There is a digit before a '(' without operator between them.
			*/
			else if (infixExpression[index] == '(') {
				braceNum++;
				if (preChar == ')') {
					printError(infixExpression, index);
					printf("<Error! No operator between \')\' and \'(\'>\n");
					return FAILED;
				}
				else if (isdigit(preChar)) {
					printError(infixExpression, index);
					printf("<Error! No operator between operand and \'(\'>\n");
					return FAILED;
				}
			}
			/*
			Handle illegal '*' and '/'
			(1)No opreands before operator '*' or '/'.
			(2)The current operator '*','/','+' or '-' follows another operator '*' or '/'.
			(3)No opreand after operator '*' or '/'.
			*/
			else if (infixExpression[index] == '*' || infixExpression[index] == '/') {
				if (preChar == 's' || (preChar != ')' && !isdigit(preChar) && preChar != '+' && preChar != '-' && preChar != '*' && preChar != '/')) /*&& (isdigit(next(infixExpression,index)) || next(infixExpression,index) == '(')*/ {
					printError(infixExpression, index);
					printf("<Error! No operand before operator \'%c\'>\n", infixExpression[index]);
					return FAILED;
				}
				else if (preChar == '+' || preChar == '-' || preChar == '*' || preChar == '/') {
					printError(infixExpression, index);
					printf("<Error! Operator \'%c\'  follows another operator \'%c\' in the expression>\n", infixExpression[index], preChar);
					return FAILED;
				}
				else if (infixExpression[index + 1] == '\0') {
					printError(infixExpression, index);
					printf("<Error! No operand after operator \'%c\'>\n", infixExpression[index]);
					return FAILED;
				}
			}
			//If the char stack is empty,
			if (StackEmpty(s) == SUCCEEDED) {
				/*
				If the current operator '+' or '-' is a unary operator, then put character '@' or '$' into stack respectively.
				*/
				if ((infixExpression[index] == '+' || infixExpression[index] == '-') && !isdigit(preChar) && preChar != ')') {
					if (Push(s, infixExpression[index] == '+' ? '@' : '$') == FAILED) {
						printf("<Error! The stack is full>\n");
						DestroyStack(s);
						return FAILED;
					}
				}
				/*
				Handle illegal braces '(' and ')'.
				*/
				else {
					if (infixExpression[index] == '(')
						leftBrace++;
					else if (infixExpression[index] == ')') {
						printError(infixExpression, index);
						printf("<Error! No matched \'(\' before \')\'>\n");
						return FAILED;
					}
					if (Push(s, infixExpression[index]) == FAILED) {
						printf("<Error! The stack is full>\n");
						DestroyStack(s);
						return FAILED;
					}
				}
			}
			//If the char stack is not empty, and the current character is '(', then push it into the char stack.
			else if (infixExpression[index] == '(') {
				if (Push(s, infixExpression[index]) == FAILED) {
					printf("<Error! The stack is full>\n");
					DestroyStack(s);
					return FAILED;
				}
				leftBrace++;
			}
			/*
			If the char stack is not empty and the current character is ')', '+', '-', '*', '/',
				(1)If the current character is ')', pop operators from the char stack to the postfix expression until meeting the open brace '('.
				(2)If the current character is '+', '-', '*', '/',
					(a)If the current character is prior to the top value of the char stack, then push it into the char stack,
					(b)else pop the top value of the stack to the postfix expression, repeat (2).
			*/
			else {
				if (infixExpression[index] == ')') {
					rightBrace++;
					GetTop(s, &e);
					while (e != '(') {
						if (Pop(s, &postfixExpression[numIndex++]) == FAILED) {
							printf("<Error! The stack is empty>\n");
							return FAILED;
						}
						GetTop(s, &e);
					}
					if (Pop(s, &e) == FAILED){
						printf("<Error! The stack is empty>\n");
						return FAILED;
					}
				}
				else {
					GetTop(s, &e);
					if (Priority(infixExpression[index]) > Priority(e)) {
						if (infixExpression[index] == '+' || infixExpression[index] == '-') {
							if (preChar == '(' && (isdigit(infixExpression[index + 1]) || infixExpression[index + 1] == '(')) {
								if (Push(s, infixExpression[index] == '+' ? '@' : '$') == FAILED) {
									printf("<Error! The stack is full>\n");
									DestroyStack(s);
									return FAILED;
								}
							}
							else {
								if (Push(s, infixExpression[index]) == FAILED) {
									printf("<Error! The stack is full>\n");
									DestroyStack(s);
									return FAILED;
								}
							}
						}
						else {
							if (Push(s, infixExpression[index]) == FAILED) {
								printf("<Error! The stack is full>\n");
								DestroyStack(s);
								return FAILED;
							}
						}
					}
					else if (Priority(infixExpression[index]) <= Priority(e)) {
						while (Priority(infixExpression[index]) <= Priority(e)) {
							if (Pop(s, &postfixExpression[numIndex++]) == FAILED) {
								printf("<Error! The stack is empty>\n");
								return FAILED;
							}
							if (GetTop(s, &e) == FAILED)
							{
								break;
							}
						}
						if (Push(s, infixExpression[index]) == FAILED) {
							printf("<Error! The stack is full>\n");
							DestroyStack(s);
							return FAILED;
						}
						preChar = infixExpression[index];
						index++;
						continue;
					}
				}
			}
		}
		if(infixExpression[index] != ' ' && infixExpression[index] != '\t'){
			preChar = infixExpression[index];
		}
		index++;
	}
	//Move the rest characters from the char stack to the postfix expression.
	while (StackEmpty(s) == FAILED) {
		if (Pop(s, &postfixExpression[numIndex++]) == FAILED) {
			printf("<Error! The stack is empty>\n");
			return FAILED;
		}
	}
	//DestroyStack the char stack.
	DestroyStack(s);
	//Check if braces are matched.
 	if (leftBrace != rightBrace) {
		if (leftBrace < rightBrace) {
			printf("<Error! No matched \'(\' before \')\'>\n");
		}
		else {
			printf("<Error! No matched \')\' after \'(\'>\n");
		}
		return FAILED;
	}
	//Check if the input is empty.
	if (blank == index) {
		printf("<Error! The input is empty>\n");
		return FAILED;
	}
	else return SUCCEEDED;
}
/*
Compute the value of postfix expression.
Traverse the postfix expression, if it's an operand, push it into a double stack.
If it's a binary operator, pop two operands from the double stack, and calculate them, then push it back to the double stack.
If it's a unary operator, pop one operand from the double stack, and calculate it, then push it back to the double stack.
when traversing is over, pop the result from the stack to the value.
*/
int computeValueFromPostfix(char* postfixExpression, double *value) {
	int index = 0;
	SequentialStackDouble* Is = (SequentialStackDouble*)malloc(sizeof(SequentialStackDouble));
	if (InitStackDouble(Is) == FAILED) {
		printf("<Error! Initialize stack fails>\n");
		return FAILED;
	}
	while (postfixExpression[index] != '\0') {
		//If the current character is digit, push it into a double stack.
		if (isdigit(postfixExpression[index])) {
			int temp = index + 1;
			double tempValue = (double)(postfixExpression[index] - '0');;
			while (postfixExpression[temp] != '#') {
				tempValue = 10.0 *tempValue + (postfixExpression[temp] - '0');
				temp++;
				index++;
			}
			if (PushDouble(Is, tempValue) == FAILED) {
				printf("<Error! The stack is full>\n");
				DestroyStackDouble(Is);
				return FAILED;
			}
			index++;
			continue;
		}
		//If the current character is a '#', then skip it.
		else if (postfixExpression[index] == '#') {
			index++;
			continue;
		}
		/*
		If the current character is operator '+', '-', '*', '/', get two operands from the stack to calculate, then put it back to the stack.
		*/
		else if (postfixExpression[index] == '+' || postfixExpression[index] == '-' || postfixExpression[index] == '*' || postfixExpression[index] == '/') {   //·ñÔòÎª²Ù×÷·û
			double num1 = 0.0, num2 = 0.0;
			if (PopDouble(Is, &num1) == FAILED) {
				printf("<Error! The stack is empty>\n");
				return FAILED;
			}
			if (PopDouble(Is, &num2) == FAILED) {
				printError(postfixExpression, index);
				printf("<Error! No  operand after Operator \'%c\'>\n", postfixExpression[index]);
				return FAILED;
			}
			switch (postfixExpression[index]) {
			case ('+'): if (PushDouble(Is, num2 + num1) == FAILED) { printf("<Error! The stack is full>\n"); return FAILED; }break;
			case ('-'): if (PushDouble(Is, num2 - num1) == FAILED) { printf("<Error! The stack is full>\n"); return FAILED; }break;
			case ('*'): if (PushDouble(Is, num2 * num1) == FAILED) { printf("<Error! The stack is full>\n"); return FAILED; }break;
			case ('/'): if (num1 != 0) {
							if (PushDouble(Is, (double)num2 / num1) == FAILED) {
								printf("<Error! The stack is full>\n");
								return FAILED;
							}
							break;
						}
						else {
							printError(postfixExpression, index);
							printf("<Error! Some operand is divided by zero in the expression>\n");
							return FAILED;
						}
			default:break;
			}
		}
		//If the current character is '@' or '$', get one operand from the stack to calculate, then put the result back to stack. 
		else if (postfixExpression[index] == '@' || postfixExpression[index] == '$') {
			double num1 = 0.0;
			if (PopDouble(Is, &num1) == FAILED) {
				printf("<Error! The stack is empty>\n");
				return FAILED;
			}
			switch (postfixExpression[index]) {
			case ('@'): if (PushDouble(Is, num1) == FAILED) { printf("<Error! The stack is full>\n"); return FAILED; }break;
			case ('$'): if (PushDouble(Is, 0 - num1) == FAILED) { printf("<Error! The stack is full>\n"); return FAILED; }break;
			default:break;
			}
		}
		index++;
	}
	//Clear postfix expression.
	for (int i = 0; i < 100; i++)
		postfixExpression[i] = '\0';
	*value = 0.0;
	//Get the result from the stack, then give it to value. 
	if (PopDouble(Is, value) == FAILED) {
		printf("<Error! The stack is empty>\n");
		return FAILED;
	}
	DestroyStackDouble(Is);
	return SUCCEEDED;
}

/*Calculate the priority value of operators.
  The argument of the function is a char, and return the priority value of the input char. 
*/
int Priority(char c) {
	switch (c) {
	case '(': return 0;
	case '+': return 1;
	case '-': return 1;
	case '*': return 2;
	case '/': return 2;
	case '@': return 3;
	case '$': return 3;
	default:  return 0;
	}
}

/*Mark the index of error.
  The arguments of the function are the address of the infix expression and the location of error. 
  Print a sign '^' below the error.
*/
void printError(char* infixExpression, int index) {
	printf("\"");
	printf("%s", infixExpression);
	printf("\"\n");
	int i = 0;
	printf(" ");
	if (infixExpression[index] == '\b')
		index = index - 2;
	while (i < index) {
		if (infixExpression[i] == '\t'){
			printf("\t");		}
		else printf(" ");
		i++;
	}
	printf("^\n");
}
