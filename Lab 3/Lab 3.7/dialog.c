#include "dialog.h"

char* COMMANDS[] = {"Choose one of this COMMANDS:\n1: help", "2: print all Livers", "3: print livers to file", "4: find liver",
                    "5: Change info about Livers", "6: Add new Liver", "7: Delete_liver", "8: Undo", "9: exit"};

int read_file(char* input_name, Node** head, Stack* stack){
    FILE* file = fopen(input_name, "r");
    char* word = NULL;
    size_t len;
    getline(&word, &len, file);
    Liver * liver = (Liver *) malloc(sizeof(Liver));
    init_liver(liver, word);
    create_node(head, liver, NULL);
    free(word);
    add(stack, liver);
    while (!feof(file)){
        len = 0;
        getline(&word, &len, file);
        Liver * liver2 = (Liver *) malloc(sizeof(Liver));
        int result = init_liver(liver2, word);
        if (result){
            destroy_array(head);
            free(liver2);
            free(word);
            return find_errors(result);
        }
        result = insert_liver(head, liver2);
        if (result){
            destroy_array(head);
            free(word);
            return find_errors(result);
        }
        add(stack, liver2);
        free(word);
    }
    fclose(file);
}

void delete_enter(char * str){
    int size = size_of_str(str);
    str[size - 1] = '\0';
}

int detect_liver(Liver ** a_liver){
    long int len;
    int result = 0;
    char* surname = NULL;
    char* name = NULL;
    char* patronomic = NULL;
    char * birth = NULL;
    char male;
    double salary;
    printf("Write Surname:");
    getline(&surname, &len, stdin);
    delete_enter(surname);
    while ((!is_correct_name(surname))||(size_of_str(surname) < 1)){
        printf("Incorrect Format. Write Surname:");
        getline(&surname, &len, stdin);
        delete_enter(surname);
    }
    printf("Write Name:");
    getline(&name, &len, stdin);
    delete_enter(name);
    while (!is_correct_name(name)||(size_of_str(name) < 1)){
        printf("Incorrect name. Write Name:");
        getline(&name, &len, stdin);
        delete_enter(name);
    }
    printf("Write Patronomic:");
    getline(&patronomic, &len, stdin);
    delete_enter(patronomic);
    while (!is_correct_name(patronomic)&&(size_of_str(patronomic) > 0)) {
        printf("Incorrect Patronomic. Write Patronomic:");
        getline(&patronomic, &len, stdin);
        delete_enter(patronomic);
    }
    Date date;
    printf("Write Birth Date in Format DD:MM:YYYY:");
    while (1) {
        getline(&birth, &len, stdin);
        result = sscanf(birth, "%d.%d.%d", &(date.day), &(date.month), &(date.year));
        if (result == 3 && is_correct_date(&date)){
            break;
        }
        if (result != 3){
            printf("Incorrect Format. Write Birth Date in Format DD:MM:YYYY:");
            continue;
        }
        printf("Incorrect Date. Write Birth Date in Format DD:MM:YYYY:");
    }
    printf("Write Male (M/W):");
    while (1) {
        male = getc(stdin);
        if ((male == 'M')||(male == 'W')){
            break;
        }
        else if (male != '\n'){
            printf("Incorrect Format. Write Male (M/W):");
        }
    }
    printf("Write Salary > 0:");
    while (1){
        result = scanf("%lf", &salary);
        if ((result == 1)&&(salary > 0)){
            break;
        }
        if (result != 1){
            printf("Incorrect Format. Write Salary >0:");
            continue;
        }
        printf("Salary must be > 0. Write Salary Again:");
    }
    char answer[BUFSIZ];
    if (size_of_str(patronomic) > 0){
        sprintf(answer, "%s %s %s %d.%d.%d %c %lf", surname, name, patronomic, date.day,
                date.month, date.year, male, salary);
    }
    else {
        sprintf(answer, "%s %s %d.%d.%d %c %lf", surname, name, date.day,
                date.month, date.year, male, salary);
    }
    result = init_liver(*a_liver, answer);
    if (result){
        free(surname);
        free(name);
        free(patronomic);
        free(birth);
        return result;
    }
    free(surname);
    free(name);
    free(patronomic);
    free(birth);
    return 0;
}

void print_commands(){
    for (int i = 0; i < 9; i++){
        printf("%s\n", COMMANDS[i]);
    }
}

int dialog(Node** head, Stack* stack){
    int exit = 1;
    char *command = NULL;
    size_t len;
    print_commands();
    while (1){
        if (getline(&command, &len, stdin) == -1){
            free(command);
            break;
        }
        if (is_str_equal(command, "1\n")){
            print_commands();
        }
        else if (is_str_equal(command, "2\n")){
            print_array(stdout, *head);
            print_commands();
        }
        else if (is_str_equal(command, "3\n")){
            printf("Write path to the file:");
            char output[BUFSIZ];
            scanf("%s", output);
            FILE * file = fopen(output, "w");
            print_array(file, *head);
            fclose(file);
            print_commands();
        }
        else if (is_str_equal(command, "4\n")){
            Liver* liver = (Liver *) malloc(sizeof(Liver));
            if (liver == NULL){
                return memory_error;
            }
            detect_liver(&liver);
            Node * node = NULL;
            find_liver(&node, *head, liver);
            if (node == NULL){
                printf("No such Liver\n");
            }
            else{
                print_liver(stdout, node->data, 0);
            }
            printf("\n\n");
            destroy_liver(liver);
            print_commands();
        } else if (is_str_equal(command, "5\n")){
            Liver* liver = (Liver *) malloc(sizeof(Liver));
            if (liver == NULL){
                return memory_error;
            }
            Liver* liver2 = (Liver *) malloc(sizeof(Liver));
            if (liver2 == NULL){
                free(liver);
                return memory_error;
            }
            printf("Liver to change:\n");
            Node * node = NULL;
            detect_liver(&liver);
            find_liver(&node, *head, liver);
            if (node == NULL){
                printf("No such Liver");
                print_commands();
                destroy_liver(liver);
                destroy_liver(liver2);
                continue;
            }
            printf("New data:\n");
            char c = getc(stdin);
            detect_liver(&liver2);
            change_liver(head, liver, liver2);
            change(stack, liver, liver2);
            printf("Your Liver was sucessfully changed\n");
            print_commands();
        }
        else if (is_str_equal(command, "6\n")){
            Liver* liver = (Liver *) malloc(sizeof(Liver));
            if (liver == NULL){
                return memory_error;
            }
            detect_liver(&liver);
            insert_liver(head, liver);
            add(stack, liver);
            printf("Your Liver was sucessfully added\n");
        }
        else if (is_str_equal(command, "7\n")){
            Liver* liver = (Liver *) malloc(sizeof(Liver));
            if (liver == NULL){
                return memory_error;
            }
            detect_liver(&liver);
            Node * node = NULL;
            find_liver(&node, *head, liver);
            if (node == NULL){
                printf("No such Liver");
                destroy_liver(liver);
                print_commands();
                continue;
            }
            destroy_node(head, node);
            delete(stack, liver);
            print_commands();
        }
        else if (is_str_equal(command, "8\n")){
            if (stack->size > 0){
                undo(head, stack);
                printf("SUCESS\n");
                print_commands();
                continue;
            }
            printf("NOT SUCESS\nYou don`t have any actions");
            print_commands();
        }
        else if (is_str_equal(command, "9\n")){
            free(command);
            break;
        }
        else if (is_str_equal(command, "\n")){
            print_commands();
        }
        else{
            printf("Incorrect Command\n");
            print_commands();
        }
    }
    return 0;
}
