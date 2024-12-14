#include <stdio.h>
#include <stdlib.h>
#include "10.h"

int main(int argc, char* argv[]) {
    if (validate_input(argc, argv)){
        return find_error(input_error);
    }

    FILE* input = fopen(argv[1], "r");
    if (!input) {
        return find_error(open_error);
    }

    FILE* output = fopen(argv[2], "w");
    if (!output) {
        fclose(input);
        return find_error(open_error);
    }

    char *line = NULL;
    size_t line_size = 0;
    int count = 1;
    while (getline(&line, &line_size, input) != -1) {
        const char* expr = line;
        if (check_expression(expr)){
            fprintf(stdout, "Incorrect expression in %d Tree\n", count++);
            continue;
        }
        Node* root = parse_expression(&expr);
        if (!root){
            continue;
        }
        fprintf(output,"Tree number %d:\n", count++);
        print_tree(root, output, 0);
        fprintf(output, "\n");
        free_tree(root);
    }

    free(line);
    fclose(input);
    fclose(output);
    return 0;
}