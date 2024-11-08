#include "Liver.h"

//Data
int is_latin_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int is_correct_name(const char * name){
    if (!name) return 0;
    for (size_t i = 0; i < strlen(name); i++) {
        if (!is_latin_letter(name[i])) {
            return 0;
        }
    }
    return 1;
}

int day_in_mounth(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
        return 29;
    }
    return days[month - 1];
}

int is_correct_date(Date *date) {
    if (date->year < 1000 || date->year > 9999 || date->month < 1 || date->month > 12 || date->day < 0 ||
        date->day > day_in_mounth(date->month, date->year))
        return 0;
    return 1;
}

int bigger_date(Date *t1, Date *t2) {
    if (t1->year > t2->year || (t1->year == t2->year && t1->month > t2->month) ||
        (t1->year == t2->year && t1->month == t2->month && t1->day > t2->day))
        return 1;
    else if (t1->year == t2->year && t1->month == t2->month && t1->day == t2->day){
        return 0;
    }
    return -1;
}

void copy_date(Date* date1, Date* date2){
    date1->day = date2->day;
    date1->month = date2->month;
    date1->year = date2->year;
}
//Liver

int init_liver(Liver* liver, const char* data){
    int patronomic = 0;
    const char *delimiter = " ";
    char *copy = strdup(data);
    char *token = strtok(copy, delimiter);
    if ((token == NULL) || (!is_correct_name(token))) return input_error;
    create_str(&(liver->surname), token);
    token = strtok(NULL, delimiter);
    if ((token == NULL) || (!is_correct_name(token))){
        clear_str(&(liver->surname));
        return input_error;
    }
    create_str(&(liver->name), token);
    token = strtok(NULL, delimiter);
    if (token == NULL){
        clear_str(&(liver->surname));
        clear_str(&(liver->name));
        return input_error;
    }
    else if (!(is_correct_name(token))){
        int n = sscanf(token, "%d.%d.%d", &(liver->birth_date.day), &(liver->birth_date.month), &(liver->birth_date.year));
        if (n != 3){
            clear_str(&(liver->surname));
            clear_str(&(liver->name));
            return input_error;
        }
        create_str(&(liver->patronymic), "\0");
    }
    else {
        create_str(&(liver->patronymic), token);
        patronomic = 1;
    }
    token = strtok(NULL, delimiter);
    if (patronomic){
        int n = sscanf(token, "%d.%d.%d", &(liver->birth_date.day), &(liver->birth_date.month), &(liver->birth_date.year));
        if (n != 3){
            clear_str(&(liver->surname));
            clear_str(&(liver->name));
            clear_str(&(liver->patronymic));
            return input_error;
        }
        token = strtok(NULL, delimiter);
        if (token == NULL){
            clear_str(&(liver->surname));
            clear_str(&(liver->name));
            clear_str(&(liver->patronymic));
            return input_error;
        }
    }
    int n = sscanf(token, "%c", &(liver->male));
    if ((liver->male != 'M') && (liver->male != 'W')){
        clear_str(&(liver->surname));
        clear_str(&(liver->name));
        clear_str(&(liver->patronymic));
        return input_error;
    }
    token = strtok(NULL, delimiter);
    if (token == NULL){
        clear_str(&(liver->surname));
        clear_str(&(liver->name));
        clear_str(&(liver->patronymic));
        return input_error;
    }
    n = sscanf(token, "%lf", &(liver->salary));
    if (!n){
        clear_str(&(liver->surname));
        clear_str(&(liver->name));
        clear_str(&(liver->patronymic));
        return input_error;
    }
    if (!is_correct_date(&(liver->birth_date))){
        clear_str(&(liver->surname));
        clear_str(&(liver->name));
        clear_str(&(liver->patronymic));
        return date_error;
    }
    if (liver->salary < 0){
        clear_str(&(liver->surname));
        clear_str(&(liver->name));
        clear_str(&(liver->patronymic));
        return salary_error;
    }
    free(copy);
    return 0;
}
//1 eсли Liver1 старше liver2, -1 если младше и 0 если равны
int elder_livers(Liver* liver1, Liver* liver2){
    return bigger_date(&(liver2->birth_date), &(liver1->birth_date));
}

int equal_livers(Liver* liver1, Liver* liver2){
    if (!(equal_str(&(liver2->surname), &(liver1->surname)))){
        return 0;
    }
    if (!(equal_str(&(liver2->name), &(liver1->name)))){
        return 0;
    }
    if (!(equal_str(&(liver2->patronymic), &(liver1->patronymic)))){
        return 0;
    }
    if (bigger_date(&(liver1->birth_date), &(liver2->birth_date))){
        return 0;
    }
    if (liver2->male != liver1->male){
        return 0;
    }
    if (fabs(liver1->salary - liver2->salary) > 10e-16){
        return 0;
    }
    return 1;
}

void destroy_liver(Liver* liver){
    if (liver == NULL){
        free(liver);
        return;
    }
    clear_str(&(liver->surname));
    clear_str(&(liver->name));
    clear_str(&(liver->patronymic));
    free(liver);
}

void print_liver(FILE *file, Liver* liver, int number){
    fprintf(file, "Liver %d:\n", number);
    print_string(file, &(liver->surname), "");
    fprintf(file, " ");
    print_string(file, &(liver->name), "");
    fprintf(file, " ");
    print_string(file, &(liver->patronymic), "");
    fprintf(file, "\n");
    fprintf(file, "%d.%d.%d %c\n%f", liver->birth_date.day,liver->birth_date.month, liver->birth_date.year, liver->male, liver->salary);
}

//Node
int create_node(Node** node, Liver* liver, Node* next){
    if (*node == NULL){
        Node *temp = (Node *) malloc(sizeof(Node));
        if (temp == NULL) return memory_error;
        *node = temp;
    }
    (*node)->data = liver;
    (*node)->next = next;
    return 0;
}

int destroy_node(Node **head, Node* node){
    if (node == *head){
        Node *temp = *head;
        *head = node->next;
        //destroy_liver(node->data);
        free(temp);
        return 0;
    }
    Node *temp = *head;
    while ((temp->next != node) && (temp->next != NULL)){
        temp = temp->next;
    }
    if (temp->next == NULL){
        return find_error;
    }
    Node * timed = temp->next;
    //destroy_liver(temp->next->data);
    temp->next = temp->next->next;
    free(timed);
    return 0;
}

void destroy_array(Node** node){
    while (*node != NULL){
        destroy_node(node, *node);
    }
}

int insert_after(Node* current, Liver* liver){
    Node* temp = (Node*) malloc(sizeof(Node));
    if (temp == NULL) return memory_error;
    temp->data = liver;
    temp->next = current->next;
    current->next = temp;
    return 0;
}

void print_array(FILE *file, Node *root){
    if (root == NULL){
        fprintf(file, "Array is Empty\n");
        return;
    }
    Node * temp = root;
    int k = 1;
    while (temp != NULL){
        print_liver(file, temp->data, k++);
        fprintf(file, "\n");
        temp = temp->next;
    }
}

int insert_liver(Node** head, Liver* liver){
    if (elder_livers(liver, (*head)->data) == 1){
        Node * temp = (Node *) malloc(sizeof(Node));
        if (temp == NULL) return memory_error;
        temp->next = (*head);
        temp->data = liver;
        *head = temp;
        return 0;
    }
    Node * next = (*head)->next;
    Node * temp = *head;
    while (next != NULL && elder_livers(next->data, liver) != -1){
        temp = next;
        next = next->next;
    }
    if (insert_after(temp, liver)){
        free(temp);
        return memory_error;
    }
    return 0;
}

void find_liver(Node** answer, Node* head, Liver* liver){
    Node *temp = head;
    while (temp != NULL){
        if (equal_livers(temp->data, liver)){
            *answer = temp;
            return;
        }
        temp = temp->next;
    }
    *answer = NULL;
}

int change_liver(Node** head, Liver * liver, Liver* liver2){
    Node * node = NULL;
    find_liver(&node, *head, liver);
    if (node == NULL){
        destroy_liver(liver);
        return find_error;
    }
    copy_str(&(node->data->surname) , &(liver2->surname));
    copy_str(&(node->data->name) , &(liver2->name));
    copy_str(&(node->data->patronymic) , &(liver2->patronymic));
    copy_date(&(node->data->birth_date), &(liver2->birth_date));
    node->data->male = liver2->male;
    node->data->salary = liver2->salary;
    return 0;
}

void change_by_undo(Node** head, Liver* liver, Liver* liver2){
    copy_str(&(liver->surname) , &(liver2->surname));
    copy_str(&(liver->name) , &(liver2->name));
    copy_str(&(liver->patronymic) , &(liver2->patronymic));
    copy_date(&(liver->birth_date), &(liver2->birth_date));
    liver->male = liver2->male;
    liver->salary = liver2->salary;
}