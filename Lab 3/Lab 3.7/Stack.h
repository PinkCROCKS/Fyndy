#ifndef LABA_3_7_STACK_H
#define LABA_3_7_STACK_H
#include "Liver.h"

Stack* createStack(int);
void deleteStack(Stack *);
void push(Stack *, Liver *, Liver*, int);
Element * pop(Stack *);
int isEmpty(Stack *);
int isFull(Stack *s);
int undo(Node**, Stack*);
void add(Stack *, Liver *);
void delete(Stack *, Liver*);
void change(Stack*, Liver*, Liver*);

#endif //LABA_3_7_STACK_H
