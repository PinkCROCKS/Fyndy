#include "dialog.h"

const char* MSG[] = {"Enter the command:",
                     "Email format:\n(city street house building apartment post_index parcel_weight mail_id "
                     "mail_create_time(%d:%d:%d %d:%d:%d) mail_receipt_time(%d:%d:%d %d:%d:%d))\n",
                     "You entered the wrong command. Try Again. For help write 1:",
                     "Unrecognized command\n",
                     "Incorrect data\n",
                     "Please, write mail_id: "};

const char* COMMANDS[] = {"1 - help\n",
                          "2 - add\n",
                          "3 - print_all\n",
                          "4 - delete\n",
                          "5 - find\n",
                          "6 - print_delivered\n",
                          "7 - print_not_delivered\n",
                          "8 - exit\n"};

const char * incorrect_arguments[] = {"Incorrect Mail Address Try again\n", "Index must be 6 Digits. Example: 123456\n",
                                      "weight must be a number > 0\n", "Incorrect ID. ID must be 14 Digits, "
                                                                       "for example: 12121212121212\n", "Incorrect Time. Write time by Format: DD:MM:YYYY HH:MM:SS\n", "Incorrect Time. Write time by Format: DD:MM:YYYY HH:MM:SS\n"};
void print_commands(){
    for (int i = 0; i < 8; i++){
        printf("%s", COMMANDS[i]);
    }
    printf("%s", MSG[0]);
}

int dialog(Post* post, int * capacity_mails, int* count_mails){
    int errorMsg;
    print_commands();
    int exit = 1;
    char *command = NULL;
    size_t len;
    while (1){
        if (getline(&command, &len, stdin) == -1){
            free(command);
            break;
        }
        if (is_str_equal(command, "1\n")){
            print_commands();
        } else if (is_str_equal(command, "2\n")){
            Mail mail;
            int k = scan_mail(&mail, (char **) incorrect_arguments);
            if (!k){
                put_mail_to_post(post, count_mails, capacity_mails, &mail);
                printf("Your Mail added to post\n%s", MSG[0]);
            } else {
                printf("MISTAKE with delivery date\n%s", MSG[0]);
            }
        } else if (is_str_equal(command, "3\n")){
            print_post(stdout, post, *count_mails);
            printf("%s", MSG[0]);
        } else if (is_str_equal(command, "4\n")){
            char * mail_id = NULL;
            long int x;
            detect_cord(&mail_id, &x, "Write ID. 14 digits", "Incorrect Format. Write 14 digits:", is_correct_ID);
            String str;
            create_str(&str, mail_id);
            int k = delete_mail(post, &str, count_mails);
            if (k){
                printf("No such mail in Post\n");
            }
            else {
                printf("Your mail successfully deleted\n");
            }
            free(mail_id);
            clear_str(&str);
            printf("%s", MSG[0]);
        } else if (is_str_equal(command, "5\n")){
            Mail mail;
            char * mail_id = NULL;
            long int x;
            detect_cord(&mail_id, &x, "Write ID. 14 digits", "Incorrect Format. Write 14 digits:", is_correct_ID);
            String str;
            create_str(&str, mail_id);
            int i = 0;
            int result = find_mail(post, &mail, &str, *count_mails, &i);
            if (result){
                printf("No such mail in Post\n");
            } else {
                print_mail(stdout, &mail);
            }
            free(mail_id);
            clear_str(&str);
            printf("%s", MSG[0]);
        } else if (is_str_equal(command, "6\n")){
            if (find_delivered_mails(post, *count_mails, stdout)){
                free(command);
                return memory_error;
            }
            printf("%s", MSG[0]);
        } else if (is_str_equal(command, "7\n")){
            if (find_not_delivered_mails(post, *count_mails, stdout)){
                free(command);
                return memory_error;
            }
            printf("%s", MSG[0]);
        } else if (is_str_equal(command, "\n")){
            print_commands();
        } else if (is_str_equal(command, "8\n")){
            free(command);
            break;
        } else{
            printf("%s", MSG[2]);
        }
    }
    return 0;
}
