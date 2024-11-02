#include "mail.h"

const char* MSG[] = {"Enter the command:\n",
                     "Email format:\n(city street house building apartment post_index parcel_weight mail_id "
                     "mail_create_time(%d:%d:%d %d:%d:%d) mail_receipt_time(%d:%d:%d %d:%d:%d))\n",
                     "You entered the wrong command\n",
                     "Unrecognized command\n",
                     "Incorrect data\n",
                     "Please, write mail_id: "};

const char* COMMANDS[] = {"/add\n",
                          "/help\n",
                          "/print_all\n",
                          "/delete\n",
                          "/find\n",
                          "/print_delivered\n",
                          "/print_not_delivered\n",
                          "/exit\n"};

const char * incorrect_arguments[] = {"City must be <500\n", "Index must be 6 Digits. Example: 123456\n",
                                      "weight must be a number > 0\n", "Incorrect ID. ID must be 14 Digits, "
                                       "for example: 12121212121212\n", "Incorrect Time. Write time by Format: DD:MM:YYYY HH:MM:SS\n", "Incorrect Time. Write time by Format: DD:MM:YYYY HH:MM:SS\n"};

int main() {
    int capacity_mails = 10;
    int count_mails = 0;
    Post post;
    int errorMsg = create_post(&post, capacity_mails, "Moscow", "1-line", 12, "k1", 43, "123456");
    if (errorMsg) {
        destroy_post(&post, count_mails);
        return find_error(errorMsg);
    }
    print_post(stdout, &post, count_mails);
    char command[1000];
    command[0] = '\0';
    printf("\nIf you need help: %s", COMMANDS[1]);
    while (1) {
        if(!my_strcmp(command, "\n")) printf("%s> ", MSG[0]);
        char* er = fgets(command, sizeof(command), stdin);
        if (!er) break;
//		Добавление письма
        if (!my_strcmp(command, COMMANDS[0]) || !my_strcmp(command, "1\n")) {
            //printf("%s", MSG[1]);
            Mail mail;
            printf("Write Address in Format: city street house building apartment\n");
            errorMsg = scan_mail(&mail, incorrect_arguments);
            if (errorMsg == input_error) {
                printf("%s", MSG[4]);
                continue;
            } else if (errorMsg) {
                destroy_post(&post, count_mails);
                return find_error(errorMsg);
            }
            //print_mail(stdout, &mail);
            errorMsg = put_mail_to_post(&post, &count_mails, &capacity_mails, &mail);
            if (errorMsg == input_error) {
                printf("%s", MSG[4]);
                continue;
            } else if (errorMsg) {
                destroy_post(&post, count_mails);
                return find_error(errorMsg);
            }
//			Печать всех команд
        } else if (!my_strcmp(command, COMMANDS[1]) || !my_strcmp(command, "2\n")) {  // КККККККККККККККККККККККККККККККККККККККК
            printf("You should write one of these commands:\n");
            for (int i = 0; i < 7; ++i) {
                printf("%d. %s", i + 1, COMMANDS[i]);
            }
//			Печать всех писем
        } else if (!my_strcmp(command, COMMANDS[2]) || !my_strcmp(command, "3\n")) {
            print_post(stdout, &post, count_mails);
//			Удаление письма
        } else if (!my_strcmp(command, COMMANDS[3]) || !my_strcmp(command, "4\n")) {
            printf("%s", MSG[5]);
            char id[100];
            scanf("%s", id);
            String mail_id;
            errorMsg = create_str(&mail_id, id);
            if (errorMsg) {
                destroy_post(&post, count_mails);
                return find_error(errorMsg);
            }
            errorMsg = delete_mail(&post, &mail_id, &count_mails);
            clear_str(&mail_id);
            if (errorMsg == input_error) {
                clear_buffer();
                printf("%s", MSG[4]);
            } else if (errorMsg) {
                destroy_post(&post, count_mails);
                return find_error(errorMsg);
            }
            printf("Mail with ID %s DELETED\n", id);
//			Поиск письма
        } else if (!my_strcmp(command, COMMANDS[4]) || !my_strcmp(command, "5\n")) {
            printf("%s", MSG[5]);
            char id[100];
            scanf("%s", id);
            Mail mail;
            String mail_id;
            errorMsg = create_str(&mail_id, id);
            if (errorMsg) {
                destroy_post(&post, count_mails);
                return find_error(errorMsg);
            }
            int index;
            errorMsg = find_mail(&post, &mail, &mail_id, count_mails, &index);
            clear_str(&mail_id);
            if (errorMsg == input_error) {
                printf("%s", MSG[4]);
            } else if (errorMsg) {
                destroy_post(&post, count_mails);
                return find_error(errorMsg);
            } else {
                clear_buffer();
                print_mail(stdout, &mail);
            }
//			Поиск доставленных писем
        } else if (!my_strcmp(command, COMMANDS[5]) || !my_strcmp(command, "6\n")) {
            errorMsg = find_delivered_mails(&post, count_mails, stdout);
            if (errorMsg) return find_error(errorMsg);
//			Поиск не доставленных писем
        } else if (!my_strcmp(command, COMMANDS[6]) || !my_strcmp(command, "7\n")) {
            errorMsg = find_not_delivered_mails(&post, count_mails, stdout);
            if (errorMsg) return find_error(errorMsg);
//			Неверная команда
        } else if (!my_strcmp(command, COMMANDS[7])){
            break;
        } else if (my_strcmp(er, "\n")) {
            printf("%s", MSG[3]);
        }
    }
    //free(&(post.address_of_post));
    destroy_post(&post, count_mails);
    return 0;
}
