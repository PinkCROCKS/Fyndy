#include "mail.h"

//Address
int create_address(Address * address, char* city, char* street, int number_house, char* building,
                   unsigned int flat, char* index){
    int result;
    if ((result = create_str(&(address->city), city))){
        return result;
    }
    if ((result = create_str(&(address->street), street))){
        return result;
    }
    if ((result = create_str(&(address->building), building))){
        return result;
    }
    if ((result = create_str(&(address->index), index))){
        return result;
    }
    address->flat = flat;
    address->house_number = number_house;
    return 0;
}

int create_address_ptr(Address** address, char* city, char* street, int number_house, char* building,
                        unsigned int flat, char* index){
    if (number_house < 0){
        return input_error;
    }
    Address * temp = (Address *) malloc(sizeof(Address));
    *address = temp;
    if (temp == NULL){
        return memory_error;
    }
    if (create_address(temp, city, street, number_house, building, flat, index)){
        free(temp);
        return memory_error;
    }
    return 0;
}

void destroy_address(Address * address){
    clear_str(&(address->city));
    clear_str(&(address->street));
    clear_str(&(address->building));
    clear_str(&(address->index));
    //free(address);
}

void print_address(FILE *stream, Address* address){
    fprintf(stream, "Address: ");
    print_string(stream, &(address->city), "");
    fprintf(stream, " ");
    print_string(stream, &(address->street), "");
    fprintf(stream, " %d ", address->house_number);
    print_string(stream, &(address->building), "");
    fprintf(stream, " %d ", address->flat);
    print_string(stream, &(address->index), "");
}

//Time

int day_in_mounth(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
        return 29;
    }
    return days[month - 1];
}

int is_correct_time(Time *time) {
    if (time->year < 1000 || time->year > 9999 || time->month < 1 || time->month > 12 || time->day < 0 ||
        time->day > day_in_mounth(time->month, time->year))
        return 0;
    if (time->hour < 0 || time->hour >= 24 || time->minute < 0 || time->minute >= 60 || time->sec < 0 ||
        time->sec >= 60)
        return 0;

    return 1;
}

int bigger_time(Time *t1, Time *t2) {
    if (t1->year > t2->year || (t1->year == t2->year && t1->month > t2->month) ||
        (t1->year == t2->year && t1->month == t2->month && t1->day > t2->day) ||
        (t1->year == t2->year && t1->month == t2->month && t1->day == t2->day && t1->hour > t2->hour) ||
        (t1->year == t2->year && t1->month == t2->month && t1->day == t2->day && t1->hour == t2->hour &&
         t1->minute > t2->minute) ||
        (t1->year == t2->year && t1->month == t2->month && t1->day == t2->day && t1->hour == t2->hour &&
         t1->minute == t2->minute && t1->sec > t2->sec))
        return 0;
    return 1;
}

//Mail
int is_digit(char c) {
    return ((c <= '9') && (c >= '0'));
}

int is_digit_str(const char *str){
    for (int i = 0; str[i] != '\0'; i++){
        if (!(is_digit(str[i]))){
            return 0;
        }
    }
    return 1;
}

int incorrect_index(char* index){
    return (size_of_str(index) != 6) || (!is_digit_str(index));
}

int incorrect_weight(double weight){
    return weight < 0;
}

int incorrect_post_id(char* index){
    return (size_of_str(index) != 14) || !is_digit_str(index);
}

int incorrect_time(Time* time){
    return !(is_correct_time(time));
}

int smaller_time(Time* first, Time* second){
    return !bigger_time(first, second);
}

int create_mail(Mail* mail, char* city, char* street, int number_house, char* building, unsigned int flat,
                char* index, double weight, char* post_id, Time create_time, Time deliver_time){
    int result = 0;
    if ((result = create_address(&(mail->adress), city, street, number_house, building, flat, index)) != 0){
        return result;
    }
    if (incorrect_index(index) || incorrect_weight(weight) || incorrect_post_id(post_id) ||
        incorrect_time(&create_time) || incorrect_time(&deliver_time) || smaller_time(&create_time, &deliver_time)) {
        destroy_address(&(mail->adress));
        return input_error;
    }
    mail->weight = weight;
    if (weight < 1e-16){
        destroy_address(&(mail->adress));
        return input_error;
    }
    char temp[21];
    temp[0] = '\0';
    sprintf(temp, "%02d:%02d:%04d %02d:%02d:%02d", create_time.day, create_time.month, create_time.year,
            create_time.hour, create_time.minute, create_time.sec);
    result = create_str(&(mail->time_of_create), temp);
    if (result){
        destroy_address(&(mail->adress));
        return result;
    }
    temp[0] = '\0';
    sprintf(temp, "%02d:%02d:%04d %02d:%02d:%02d", deliver_time.day, deliver_time.month, deliver_time.year,
            deliver_time.hour, deliver_time.minute, deliver_time.sec);
    result = create_str(&(mail->time_of_deliver), temp);
    if (result){
        destroy_address(&(mail->adress));
        clear_str(&(mail->time_of_create));
        return  result;
    }
    result = create_str(&(mail->post_id), post_id);
    if (result){
        destroy_address(&(mail->adress));
        clear_str(&(mail->time_of_create));
        clear_str(&(mail->time_of_deliver));
        return  result;
    }
    return 0;
}

void destroy_mail(Mail * mail){
    destroy_address(&(mail->adress));
    clear_str(&(mail->time_of_create));
    clear_str(&(mail->time_of_deliver));
    clear_str(&(mail->post_id));
}

int mail_less(Mail mail1, Mail mail2){
    return string_to_int(&(mail1.adress.index)) < string_to_int(&(mail2.adress.index)) ||
     (string_to_int(&(mail1.adress.index)) == string_to_int(&(mail2.adress.index)) &&
      string_to_int(&(mail1.post_id)) < string_to_int(&(mail2.post_id)));
}

void print_mail(FILE* stream, Mail* mail){
    printf("============== Mail:");
    print_string(stream, &(mail->post_id), "");
    printf(" ==============\nAddress: ");
    print_address(stream, &(mail->adress));
    printf("\nTime of create: ");
    print_string(stream, &(mail->time_of_create), "");
    printf("\nTime of deliver: ");
    print_string(stream, &(mail->time_of_deliver), "");
    printf("\n===================================\n");
}

//Post
int create_post(Post * post, int amount_of_mails, char *city, char *street, int house, char *building,
                int apartment, char *index){
    int result = create_address_ptr(&(post->address_of_post), city, street, house, building, apartment, index);
    if (result){
        return result;
    }
    Mail * temp = (Mail*) malloc(amount_of_mails * sizeof(Mail));
    if (temp == NULL){
        destroy_address(&(post->address_of_post));
        return memory_error;
    }
    post->mails = temp;
    return 0;
}

void print_post(FILE * stream, Post* post, int amount_mails){
    fprintf(stream, "Post Office\nAddress: ");
    print_address(stream, post->address_of_post);
    fprintf(stream, "\nAmount of mails: %d\n", amount_mails);
    for (int i = 0; i < amount_mails; ++i) {
        print_mail(stream, &(post->mails[i]));
    }
}

void destroy_post(Post * post, const int amount_mails){
    for (int i = 0; i < amount_mails;++i){
        destroy_mail(&(post->mails[i]));
    }
    free(post->mails);
}

int put_mail_to_post(Post * post, int* amount_mails, int * size_post, Mail* mail){
    for (int i = 0; i < *amount_mails; i++){
        if (equal_str(&(mail->post_id), &(post->mails[i].post_id))){
            destroy_mail(mail);
            return input_error;
        }
    }
    if (*amount_mails == *size_post){
        Mail *temp = (Mail *) realloc(post->mails, *size_post * 2 * sizeof(Mail));
        if (temp == NULL){
            destroy_post(post, *amount_mails);
            return memory_error;
        }
        *size_post *= 2;
        post->mails = temp;
    }
    int i = 0;
    while (i < *amount_mails && mail_less(post->mails[i], *mail)){
        i++;
    }
    for (int j = *amount_mails; j > i; j--){
        post->mails[j] = post->mails[j - 1];
    }
    post->mails[i] = *mail;
    *amount_mails += 1;
    return 0;
}

int find_mail(Post* post, Mail* mail, String* post_id, int amount_mails, int* i){
    for (int k = 0; k < amount_mails; k++){
        if (equal_str(post_id, &(post->mails[k].post_id))){
            *mail = post->mails[k];
            *i = k;
            return 0;
        }
    }
    return input_error;
}

int delete_mail(Post* post, String* post_id, int* amount_mails){
    int i = 0;
    Mail mail;
    int result = find_mail(post, &mail, post_id, *amount_mails, &i);
    if (result){
        return result;
    }
    destroy_mail(&((post->mails)[i]));
    for (int j = i + 1; j < *amount_mails; j++){
        post->mails[j - 1] = post->mails[j];
    }
    (*amount_mails)--;
    return 0;
}

int compare_bigger(const void * first, const void * second){
    Mail * mail1 = (Mail *) first;
    Mail * mail2 = (Mail *) second;
    Time time1;
    Time time2;
    sscanf(mail1->time_of_create.content, "%02d:%02d:%04d %02d:%02d:%02d", &(time1.day), &(time1.month),
           &(time1.year), &(time1.hour), &(time1.minute), &(time1.sec));
    sscanf(mail2->time_of_create.content, "%02d:%02d:%04d %02d:%02d:%02d", &(time2.day), &(time2.month),
           &(time2.year), &(time2.hour), &(time2.minute), &(time2.sec));
    if (bigger_time(&time1, &time2)){
        return 1;
    }
    return -1;
}

int compare_smaller(const void * first, const void * second){
    return -compare_bigger(first, second);
}

int find_delivered_mails(Post* post, int amount_mails, FILE * file){
    time_t real_time = time(NULL);
    struct tm * now = localtime(&real_time);
    Time time0 = {now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec};
    Mail * delivered_mails = (Mail *) malloc(amount_mails * sizeof(Mail));
    int amount_delivered = 0;
    for (int i = 0; i < amount_mails; i++){
        Time time1;
        sscanf(post->mails[i].time_of_deliver.content,  "%02d:%02d:%04d %02d:%02d:%02d", &(time1.day), &(time1.month),
               &(time1.year), &(time1.hour), &(time1.minute), &(time1.sec));
        if (bigger_time(&time1, &time0)){
            delivered_mails[amount_delivered++] = post->mails[i];
        }
    }
    if (amount_delivered > 1){
        qsort(delivered_mails, amount_delivered, sizeof(Mail), compare_bigger);
    }
    for (int i = 0; i < amount_delivered; i++){
        print_mail(file, &delivered_mails[i]);
    }
    free(delivered_mails);
    return 0;
}

int find_not_delivered_mails(Post* post, int amount_mails, FILE * file){
    time_t real_time = time(NULL);
    struct tm * now = localtime(&real_time);
    Time time0 = {now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec};
    Mail * delivered_mails = (Mail *) malloc(amount_mails * sizeof(Mail));
    int amount_delivered = 0;
    for (int i = 0; i < amount_mails; i++){
        Time time1;
        sscanf(post->mails[i].time_of_deliver.content,  "%02d:%02d:%04d %02d:%02d:%02d", &(time1.day), &(time1.month),
               &(time1.year), &(time1.hour), &(time1.minute), &(time1.sec));
        if (!bigger_time(&time1, &time0)){
            delivered_mails[amount_delivered++] = post->mails[i];
        }
    }
    if (amount_delivered > 1){
        qsort(delivered_mails, amount_delivered, sizeof(Mail), compare_bigger);
    }
    for (int i = 0; i < amount_delivered; i++){
        print_mail(file, &delivered_mails[i]);
    }
    free(delivered_mails);
    return 0;
}

int scan_mail(Mail * mail, char ** answers){
    char city[500];
    char street[500];
    int house;
    char building[500];
    int apartment;
    char index[100];
    double weight;
    char mail_id[50];
    Time create_time;
    Time delivered_time;
    Arguments k = a_address;
    int n = 0;
    int sucess = 1;
    while (k != a_quit && sucess){
        switch (k) {
            case a_address:
                n = scanf("%s %s %d %s %d", city, street, &house, building, &apartment);
                if (n != 5){
                    printf("%s", answers[a_address]);
                    sucess = 0;
                    break;
                }
                n = 0;
                k = a_index;
                printf("Now write index in format six digits. For Example: 123456\n");
                break;
            case a_index:
                n = scanf("%s", index);
                if ((n != 1)||(incorrect_index(index))){
                    printf("%s", answers[a_index]);
                    break;
                }
                n = 0;
                k = a_weight;
                printf("Now write weight. It must be > 0\n");
                break;
            case a_weight:
                n = scanf("%lf", &weight);
                if ((n != 1)||(incorrect_weight(weight))){
                    printf("%s", answers[a_weight]);
                    break;
                }
                n = 0;
                k = a_id;
                printf("Now write mail ID. It is 14 Digits, for example: 12121212121212\n");
                break;
            case a_id:
                n = scanf("%s", mail_id);
                if ((n != 1)||(incorrect_post_id(mail_id))){
                    printf("%s", answers[a_id]);
                    break;
                }
                n = 0;
                k = a_create_time;
                printf("Now write Create time by Format: DD:MM:YYYY HH:MM:SS\n");
                break;
            case a_create_time:
                n = 0;
                char input1[BUFSIZ];
                char input2[BUFSIZ];
                printf("Write Date: DD:MM:YYYY\n");
                n = scanf("%s", input1);
                int a = sscanf(input1, "%d:%d:%d", &(create_time.day), &(create_time.month), &(create_time.year));
                if (a != 3){
                    printf("Incorrect Format, Try again\n");
                    break;
                }
                printf("Write Time: HH:MM:SS\n");
                n = scanf("%s", input2);
                int b = sscanf(input2, "%d:%d:%d", &(create_time.hour), &(create_time.minute), &(create_time.sec));
                if (b != 3){
                    printf("Incorrect Format, Try again\n");
                    break;
                }
                if ((incorrect_time(&create_time))){
                    printf("Incorrect Time\n");
                    break;
                }
                n = 0;
                k = a_deliver_time;
                printf("Now write Deliver time by Format: DD:MM:YYYY HH:MM:SS\n");
                break;
            case a_deliver_time:
                n = 0;
                printf("Write Date: DD:MM:YYYY\n");
                n = scanf("%s", input1);
                a = sscanf(input1, "%d:%d:%d", &(delivered_time.day), &(delivered_time.month), &(delivered_time.year));
                if (a != 3){
                    printf("Incorrect Format, Try again\n");
                    break;
                }
                printf("Write Time: HH:MM:SS\n");
                n = scanf("%s", input2);
                b = sscanf(input2, "%d:%d:%d", &(delivered_time.hour), &(delivered_time.minute), &(delivered_time.sec));
                if (b != 3){
                    printf("Incorrect Format, Try again\n");
                    break;
                }
                if ((incorrect_time(&create_time))){
                    printf("Incorrect Time\n");
                    break;
                }
                n = 0;
                k = a_quit;
                printf("Now write Deliver time by Format: DD:MM:YYYY HH:MM:SS\n");
                break;
        }
    }
    if (sucess){
        return create_mail(mail, city, street, house, building, apartment, index, weight, mail_id, create_time, delivered_time);
    }
    return input_error;
}

void clear_buffer(){
    while ( getchar() != '\n' );
}