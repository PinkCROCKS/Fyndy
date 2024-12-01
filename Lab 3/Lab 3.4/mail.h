#ifndef LABA_3_4_MAIL_H
#define LABA_3_4_MAIL_H

#include "strings.h"
#include <time.h>

typedef enum Arguments{
    a_address,
    a_index,
    a_weight,
    a_id,
    a_create_time,
    a_deliver_time,
    a_quit
} Arguments;

typedef struct Address{
    String city;
    String street;
    int house_number;
    String building;
    int flat;
    String index;
} Address;

typedef struct Time{
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int sec;
} Time;

typedef struct Mail{
    Address adress;
    double weight;
    String post_id;
    String time_of_create;
    String time_of_deliver;
} Mail;

typedef struct Post{
    Address *address_of_post;
    Mail * mails;
} Post;

//Address
int create_address(Address *, char*, char*, int, char*, unsigned int, char*);
int create_address_ptr(Address **, char*, char*, int, char*, unsigned int, char*);
void destroy_address(Address *);
void print_address(FILE *, Address*);

//Time
int day_in_mounth(int, int);
int is_correct_time(Time*);
int bigger_time(Time *, Time *);

//Mail
int create_mail(Mail*, char*, char*, int, char*, unsigned int, char*, double, char*, Time, Time);
void detect_cord(char** coord, long int* len, char* first, char* incorrect_answer, int (*func)(char *));
int is_correct_ID(char * part);
void destroy_mail(Mail*);
void print_mail(FILE*, Mail*);
int put_mail_to_post(Post*, int*, int*, Mail*);
int find_mail(Post*, Mail*, String*, int, int*);
int delete_mail(Post*, String*, int*);
int find_delivered_mails(Post*, int, FILE*);
int find_not_delivered_mails(Post*, int, FILE*);
int scan_mail(Mail*, char**);

//Post
int create_post(Post*, int, char*, char *, int, char*, int, char*);
void print_post(FILE*, Post*, int);
void destroy_post(Post*, const int);
void clear_buffer();
#endif //LABA_3_4_MAIL_H
