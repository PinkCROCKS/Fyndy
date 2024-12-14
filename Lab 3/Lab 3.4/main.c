#include "dialog.h"

int main() {
    int capacity_mails = 10;
    int count_mails = 0;
    Post post;
    int errorMsg = create_post(&post, capacity_mails, "Moscow", "1-line", 12, "k1", 43, "123456");
    printf("%d", errorMsg);
    if (errorMsg) {
        return find_error(errorMsg);
    }
    print_post(stdout, &post, count_mails);
    if (dialog(&post, &capacity_mails, &count_mails)){
        destroy_address(post.address_of_post);
        free(post.address_of_post);
        destroy_post(&post, count_mails);
        return find_error(memory_error);
    }
    destroy_address(post.address_of_post);
    free(post.address_of_post);
    destroy_post(&post, count_mails);
    return 0;
}
