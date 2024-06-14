#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// James Phillips
// 6/13/24
// Program to take an equation, output if the parenthesis are valid,
// and if they are, output the postfix version of it.

#define SIZE 10
#define EMPTY -1

typedef struct stack {
    char items[100];
    int top;
}stack;

void remove_spaces(char* input);
int isOperator(char c);
int isParenthesis(char c);
int getPriority(char c);
void append(char* s, char c);
char respective(char c);
int checkBalance(stack* s, char exp[]);
char *infixToPostfix(char infix[]);
void initialize(struct stack* stackPtr);
void push(struct stack* stackPtr, char c);
char pop(struct stack* stackPtr);

int main() {
    stack s;
    initialize(&s);

    char input[100];
    printf("Enter Expression: ");
    scanf("%[^\n]%*c", input); // Prevent spaces from ending scanf
    printf("Your input expression: %s", input);
    remove_spaces(input); // Makes postfix easier
    
    if(checkBalance(&s, input)){ // If parenthesis are valid, output postfix
        printf("\nVALID");
        printf("\nThe postfix is: %s", infixToPostfix(input));
    }
    else
        printf("\nINVALID for %c!!!", s.items[s.top]);

    
    return 0;
}

// For each character in the string, copy it to the temp string if it's not a space
// Then copy the result string back to the input. It's a pointer so alters the original.
void remove_spaces(char* input){
    int traverse = 0;
    char temp[100] = "";
    while(input[traverse] != '\0'){
        if(input[traverse] != ' ')
            append(temp, input[traverse]);
        traverse++;
    }
    strcpy(input, temp);

    return;
}

void initialize(struct stack* stackPtr) {
     stackPtr->top = -1;
}

void push(struct stack* stackPtr, char c) {
    stackPtr->items[++stackPtr->top] = c;
    return;
}

char pop(struct stack* stackPtr) {
    return stackPtr->items[stackPtr->top--];
}

int isOperator(char c){
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^')
        return 1;
    return 0;
}

int isParenthesis(char c){
    if(c == '(' || c == '[' || c == '{' || c == '}' || c == ']' || c == ')')
        return 1;
    return 0;
}

// Returns integer to show priority. Ease of access function.
int getPriority(char c){
    if(c == '^')
        return 2;
    else if(c == '*' || c == '/' || c == '%')
        return 1;
    else if(c == '+' || c == '-')
        return 0;
}

// For appending to postfix. Make character a string and then strcat
void append(char* s, char c){
    char* temp = malloc(sizeof(char));
    *temp = c;
    strcat(s, temp);
    free(temp);
}

// Used to find respective opening parenthesis to closing parenthesis
char respective(char c){
    if(c == ']')
        return '[';
    else if(c == '}')
        return '{';
    else if(c == ')')
        return '(';
}

// Check valid parenthesis
int checkBalance(stack* s, char exp[]){
    printf("\n\nChecking balance...");

    int traverse = 0;
    char temp;

    // Loop through input until end of string. If it isn't a form of parenthesis,
    // just traverse forward.
    while(exp[traverse] != '\0'){
        // If it's an opening parenthesis, add to stack.
        if(exp[traverse] == '(' || exp[traverse] == '[' || exp[traverse] == '{'){
            push(s, exp[traverse]);
        }
        // Check to see if it is a closing parenthesis and if the top of the stack
        // isn't the respective parenthesis, return invalid (0)
        else if(exp[traverse] == ')'){
            if(pop(s) != '('){
                push(s, exp[traverse]);
                return 0;
            }
        }
        else if(exp[traverse] == ']'){
            if(pop(s) != '['){
                push(s, exp[traverse]);
                return 0;
            }
        }
        else if(exp[traverse] == '}'){
            if(pop(s) != '{'){
                push(s, exp[traverse]);
                return 0;
            }
        }
        traverse++;
    }
    return 1;
}

// Convert to postfix
char *infixToPostfix(char infix[]){
    int traverse = 0;
    char *postfix = malloc(sizeof(char) * 100);
    char temp;
    stack s;
    initialize(&s);

    // Loops through whole input string
    while(infix[traverse] != '\0'){
        // If char is a parenthesis, if it's open push to stack. If it's close
        // pop stack and append to postfix until you reach the open and then just
        // remove the open.
        if(isParenthesis(infix[traverse])){
            if(infix[traverse] == '(' || infix[traverse] == '[' || infix[traverse] == '{'){
                push(&s, infix[traverse]);
            }
            else{
                while(s.items[s.top] != respective(infix[traverse])){
                    temp = pop(&s);
                    append(postfix, temp);
                }
                temp = pop(&s);
            }
        }
        // If it's an operator, if stack is empty or parenthesis is at the top then push to stack, if
        // not, then if priority is higher then push. If priority is less than or equal, pop and append
        // to postfix until higher priority, opening parenthesis, or end of stack. Then push char.
        else if(isOperator(infix[traverse])){
            if(s.top < 0 || isParenthesis(s.items[s.top])){
                push(&s, infix[traverse]);
            }
            else{
                if(getPriority(infix[traverse]) > getPriority(s.items[s.top])){
                    push(&s, infix[traverse]);
                }
                else{
                    while(s.top > -1 && !isParenthesis(s.items[s.top]) && (getPriority(infix[traverse]) <= getPriority(s.items[s.top]))){
                        temp = pop(&s);
                        append(postfix, temp);
                    }
                    push(&s, infix[traverse]);
                }
            }
        }
        // If it's an operand (anything other than operator or parenthesis) just append to postfix.
        else{
            append(postfix, infix[traverse]);
        }
        traverse++;
    }
    // Append the remaining stack to postfix.
    if(s.top > -1){
        while(s.top > -1 && !isParenthesis(s.items[s.top])){
            temp = pop(&s);
            append(postfix, temp);
        }
    }
    return postfix;
}