#include <stdio.h>
#include "Liver.h"
#include "dialog.h"

int main(int argc, char** argv) {
    Node * head = NULL;
    Stack * stack = createStack(100);
    read_file(argv[1], &head, stack);
    //change_liver(&head, "Q Q 12.11.2025 M 1.265", "U U 12.11.2025 M 1.265");
    //print_array(stdout, head);
    //printf("\n=============================\n");
    //print_array(stdout, head);
    if (dialog(&head, stack)){
        return 1;
    }
    deleteStack(stack);
    destroy_array(&head);
    return 0;
}
