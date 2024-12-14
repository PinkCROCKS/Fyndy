#include "10.h"
#include <stdlib.h>
#include <string.h>

int find_error(int key){
    switch (key) {
        case input_error:
            printf("Mistake with input");
            return 1;
        case amount_arguments_error:
            printf("Mistake with amount of arguments");
            return 1;
        case same_files_error:
            printf("All files must be different");
            return 1;
        case open_error:
            printf("No such file or Directory");
            return 1;
        default:
            printf("Unknown Mistake");
            return 1;
    }
}


int check_expression(const char *expr){
    int count = 0;
    while (*expr){
        switch (*expr){
            case '(':
                count++;
                break;
            case ')':
                count--;
                break;
        }
        expr++;
    }
    if (!count){
        return 0;
    }
    return input_error;
}

Node* create_node(char value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode){
        find_error(memory_error);
        return NULL;
    }

    newNode->value = value;
    newNode->child_count = 0;
    newNode->child_capacity = 2;
    newNode->children = (Node**)malloc(newNode->child_capacity * sizeof(Node*));
    if (!newNode->children){
        free(newNode);
        return NULL;
    }
    return newNode;
}

int add_child(Node* parent, Node* child) {
    if (parent->child_count == parent->child_capacity) {
        parent->child_capacity *= 2;
        Node** newChildren = (Node**)realloc(parent->children, parent->child_capacity * sizeof(Node*));
        if (newChildren == NULL) {
            return memory_error;
        }
        parent->children = newChildren;
    }
    parent->children[parent->child_count++] = child;
    return 0;
}

void free_tree(Node* root) {
    if (root == NULL) return;
    for (int i = 0; i < root->child_count; i++) {
        free_tree(root->children[i]);
    }
    free(root->children);
    free(root);
}

int skip_symbol(char c){
    return c == ' ' || c == '\t';
}

Node* parse_expression(const char** expr) {
    if (**expr == '\0' || **expr == ')') return NULL;

    Node* root = create_node(**expr);
    if (!root) return NULL;
    (*expr)++;
    while (skip_symbol(**expr)){
        (*expr)++;
    }

    if (**expr == '(') {
        (*expr)++;
        while (**expr != '\0' && **expr != ')') {
            Node* child = parse_expression(expr);
            if (child) {
                if (add_child(root, child)){
                    free_tree(root);
                    return NULL;
                }
            }else{
                free_tree(root);
                return NULL;
            }
            if (**expr == ',') (*expr)++;
        }
        (*expr)++;
    }
    return root;
}


void print_tree(Node* root, FILE* file, int depth) {
    if (root == NULL) return;

    for (int i = 0; i < depth; i++) {
        fprintf(file, "  ");
    }

    fprintf(file, "%c\n", root->value);

    for (int i = 0; i < root->child_count; i++) {
        print_tree(root->children[i], file, depth + 1);
    }
}

int validate_input(int argc, char **argv){
    char full_in[BUFSIZ], full_out[BUFSIZ];
    if (argc != 3) return input_error;

    if (!realpath(argv[1], full_in) || !realpath(argv[2], full_out)) return input_error;
    if (!strcmp(full_in, full_out)) return input_error;

    return 0;
}