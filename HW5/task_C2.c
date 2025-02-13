#include <stdio.h>
#define MAX_STACK_SIZE 255

int st[MAX_STACK_SIZE]; // массив - стек
int pst = 0;            // заполненность  стека
void push(int v)
{ // используется для вычислений
    st[pst++] = v;
}
int pop()
{
    if (pst <= 0)
    {
        fprintf(stderr, "Error. Stack underflow");
        return 1;
    }
    else if (pst > MAX_STACK_SIZE)
    {
        fprintf(stderr, "Error. Stack overflow");
        return 1;
    }
    return st[--pst];
}
int isEmpty()
{ // определяет пустой ли стек st
    return (pst <= 0);
}
void operate(char c)
{ // вычисляем два верхних значения на стеке st
    int arg1 = pop(), arg2 = pop();
    if (c == '+')
        push(arg1 + arg2);
    else if (c == '-')
        push(arg2 - arg1);
    else if (c == '*')
        push(arg1 * arg2);
    else if (c == '/')
        push(arg2 / arg1);
}
int isDigit(char c)
{ // проверяем является ли символ цифрой
    return ((c >= '0') && (c <= '9'));
}

int main(void)
{
    char c;

    while ((c = getchar()) != '.')
    {
        if (isDigit(c))
        {
            int digit = c - '0';
            while((c = getchar()) != ' ') {
                digit = digit * 10 + (c - '0');
            }
            push(digit);
        }
        else if (c == ' ')
        {
            continue;
        }
        else
        {
            operate(c);
        }
    }

    printf("%d\n", pop());
    return 0;
}
