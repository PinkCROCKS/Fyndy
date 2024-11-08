#include "Liver.h"
#include "Stack.h"

Stack* createStack(int capacity) {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(1);
    }
    stack->size = 0;
    stack->capacity = capacity;
    stack->data = (Element **)malloc(capacity * sizeof(Element *));
    if (stack->data == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(1);
    }
    return stack;
}

// Функция для удаления стека
void deleteStack(Stack *stack) {
   for (int i = 0; i < stack->size;i++){
       if (stack->data[i]->key >= 0){
           destroy_liver(stack->data[i]->liver);
       }
       destroy_liver(stack->data[i]->father);
       free(stack->data[i]);
   }
   free(stack->data);
   free(stack);
}

// Функция для добавления элемента в стек
void push(Stack *stack, Liver *item, Liver* father, int key) { //1 - Добавление, 0 - Редактирование, -1 - Удаление
    if (stack->size == stack->capacity) {
        fprintf(stderr, "Стек переполнен\n");
        return;
    }
    Element * new = (Element *) malloc(sizeof(Element));
    if (new == NULL) {
        fprintf(stderr, "Ошибка Памяти\n");
        return;
    }
    if (key == 1){
        Liver * fat = (Liver *) malloc(sizeof(Liver));
        if (fat == NULL){
            fprintf(stderr, "Ошибка Памяти\n");
            return;
        }
        stack->data[stack->size] = new;
        stack->data[stack->size]->liver = item;
        stack->data[stack->size]->father = fat;
        stack->data[stack->size]->father = NULL;
        stack->data[stack->size]->key = key;
        free(fat);
    }
    else if (key == -1){
        Liver * bat = (Liver *) malloc(sizeof(Liver));
        if (bat == NULL){
            fprintf(stderr, "Ошибка Памяти\n");
            return;
        }
        stack->data[stack->size] = new;
        stack->data[stack->size]->liver = bat;
        stack->data[stack->size]->father = NULL;
        stack->data[stack->size]->father = father;
        stack->data[stack->size]->key = key;
        free(bat);
    }
    else{
        stack->data[stack->size] = new;
        stack->data[stack->size]->liver = item;
        stack->data[stack->size]->father = father;
        stack->data[stack->size]->key = key;
    }
    (stack->size)++;
}

void add(Stack *stack, Liver *item){
    push(stack, item, NULL, 1);
}

void delete(Stack *stack, Liver* liver) {
    push(stack, NULL, liver, -1);
}

void change(Stack* stack, Liver* old_liver, Liver* new_liver){
    push(stack, new_liver, old_liver, 0);
}

Element * pop(Stack *stack) {
    if (stack->size == 0) {
        fprintf(stderr, "Стек пуст\n");
        return NULL;
    }
    return stack->data[--stack->size];
}

// Функция для проверки, пуст ли стек
int isEmpty(Stack *stack) {
    return stack->size == 0;
}

// Функция для проверки, полон ли стек
int isFull(Stack *stack) {
    return stack->size == stack->capacity;
}

int undo(Node** head, Stack* stack){
    int n = stack->size;
    Node *node = NULL;
    for (int i = 0; i < (n / 2) + (n % 2); i++){
        Element * element = pop(stack);
        switch (element->key) {
            case 1:
                find_liver(&node, *head, element->liver);
                destroy_liver(element->liver);
                free(element);
                destroy_node(head, node);
                break;
            case -1:
                insert_liver(head, element->father);
                break;
            case 0:
                change_by_undo(head, element->liver, element->father);
                break;
        }
    }
}

