#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

typedef struct Stack {
    int top;
    unsigned capacity;
    double* array;
} Stack;

Stack* createStack(unsigned capacity);
int isFull(Stack* stack);
int isEmpty(Stack* stack);
void push(Stack* stack, double item);
double pop(Stack* stack);
int precedence(char op);
void convertToRPN(char* infix, char* postfix);
double evaluateRPN(char* tokens);

int main() {
    int choice;
    char infix[MAX_SIZE];
    char postfix[MAX_SIZE];
    int expressionEntered = 0;
    system("chcp 1251");
    /*system("color f0");*/
   
    do {
        printf("\nВыберите действие:\n");
        printf("1. Ввести выражение\n");
        printf("2. Посчитать выражение\n");
        printf("3. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            printf("Введите выражение: ");
            fgets(infix, sizeof(infix), stdin);
            expressionEntered = 1;
            break;
        case 2:
            if (expressionEntered) {
                convertToRPN(infix, postfix);
                printf("Обратная польская запись: %s\n", postfix);

                double result = evaluateRPN(postfix);
                printf("Результат вычисления: %lf\n", result);
            }
            else {
                printf("Сначала введите выражение.\n");
            }
            break;
        case 3:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор. Пожалуйста, выберите снова.\n");
            break;
        }
    } while (choice != 3);

    return 0;
}

Stack* createStack(unsigned capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (double*)malloc(stack->capacity * sizeof(double));
    return stack;
}

int isFull(Stack* stack) {
    return stack->top == stack->capacity - 1;
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, double item) {
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}

double pop(Stack* stack) {
    if (isEmpty(stack))
        return -1; 
    return stack->array[stack->top--];
}

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/' || op == '^')
        return 2;
    return 0;
}

void convertToRPN(char* infix, char* postfix) {
    Stack* stack = createStack(strlen(infix) + 1);
    int i, j = 0;

    for (i = 0; infix[i]; ++i) {
        char token = infix[i];

        if (isdigit(token) || token == '.') {
            while (isdigit(infix[i]) || infix[i] == '.') {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
            i--;
        }
        else if (token == '(') {
            push(stack, token);
        }
        else if (token == ')') {
            while (!isEmpty(stack) && stack->array[stack->top] != '(') {
                postfix[j++] = pop(stack);
                postfix[j++] = ' ';
            }
            pop(stack);
        }
        else {
            while (!isEmpty(stack) && precedence(token) <= precedence(stack->array[stack->top])) {
                postfix[j++] = pop(stack);
                postfix[j++] = ' ';
            }
            push(stack, token);
        }
    }

    while (!isEmpty(stack)) {
        postfix[j++] = pop(stack);
        postfix[j++] = ' ';
    }

    postfix[j] = '\0';
    free(stack->array);
    free(stack);
}

double evaluateRPN(char* tokens) {
    Stack* stack = createStack(strlen(tokens) + 1);
    int i;
    double operand;

    for (i = 0; tokens[i]; ++i) {
        char token = tokens[i];

        if (isdigit(token) || token == '.') {
            double num = atof(&tokens[i]);
            while (isdigit(tokens[i]) || tokens[i] == '.') {
                i++;
            }
            i--;
            push(stack, num);
        }

        else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^') {
            double operand2 = pop(stack);
            double operand1 = pop(stack);

            switch (token) {
            case '+':
                operand = operand1 + operand2;
                break;
            case '-':
                operand = operand1 - operand2;
                break;
            case '*':
                operand = operand1 * operand2;
                break;
            case '/':
                operand = operand1 / operand2;
                break;
            case '^':
                operand = pow(operand1, operand2);
                break;
            }
            push(stack, operand);
        }
    }

    double result = pop(stack);
    free(stack->array);
    free(stack);
    return result;
} 