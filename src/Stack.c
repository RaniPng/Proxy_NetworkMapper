#include <Stack.h>
#include <stdio.h>
#include <stdlib.h>

ClassType stackTop(Stack *s)
{
    return s->top ? s->top->data : NULL;
}

ClassType stackPop(Stack *s)
{
    if (s->size > 0)
    {
        ClassType ret = stackTop(s);
        if (!ret)
            return NULL;
        s->size--;
        StackBullet *ptr = s->top;
        s->top = s->top->next;
        free(ptr);
        return ret;
    }
    return NULL;
}

void stackPush(Stack *s, ClassType push)
{
    StackBullet *node = (StackBullet *)malloc(sizeof(StackBullet));
    if (node)
    {
        node->data = push;
        node->next = s->top;
        s->top = node;
        s->size++;
        return;
    }
    perror("No space");
}

void stackEmpty(Stack *s)
{
    while (s->top)
        stackPop(s); // pop automatic free the top
}

void stackFree(Stack *s)
{
    stackEmpty(s);
    free(s);
}

Stack *createStack()
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->top = NULL;
    s->size = 0;
    return s;
}
