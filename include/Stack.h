#pragma once

typedef void *ClassType;

struct StackBullet
{
    ClassType data;
    struct StackBullet *next;
};
typedef struct StackBullet StackBullet;

struct Stack
{
    StackBullet *top;
    int size;
};
typedef struct Stack Stack;

Stack *createStack();

ClassType stackTop(Stack *s);
ClassType stackPop(Stack *s);
void stackPush(Stack *s, ClassType push);
void stackEmpty(Stack *s);
void stackFree(Stack *s);