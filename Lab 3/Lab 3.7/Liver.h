#ifndef LABA_3_7_LIVER_H
#define LABA_3_7_LIVER_H

#include "strings.h"
#include <string.h>
#include <math.h>

typedef struct Date{
    int day;
    int month;
    int year;
} Date;

typedef struct Liver{
    String surname;
    String name;
    String patronymic;
    Date birth_date;
    char male;
    double salary;
} Liver;

typedef struct Node{
    Liver* data;
    struct Node * next;
} Node;

typedef struct Element{
    Liver * liver;
    Liver * father;
    int key;
} Element;

// Определение структуры Stack
typedef struct {
    int size;
    Element ** data;
    int capacity;
} Stack;

int init_liver(Liver*, const char*);
void print_liver(FILE*, Liver*, int);
void print_array(FILE *, Node *);
void destroy_liver(Liver*);
int bigger_date(Date *, Date *);
int elder_livers(Liver* , Liver* );
int insert_after(Node*, Liver*);
int insert_liver(Node**, Liver*);
int create_node(Node**, Liver*, Node*);
void destroy_array(Node**);
int equal_livers(Liver*, Liver*);
void find_liver(Node **, Node*, Liver*);
int change_liver(Node**, Liver *, Liver *);
int destroy_node(Node **, Node*);
int is_correct_name(const char *);
int is_correct_date(Date *);
void change_by_undo(Node**, Liver*, Liver*);

#endif //LABA_3_7_LIVER_H
