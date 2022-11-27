#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//#include "debugmalloc.h"

#define TABLE 57
#define MAX_LENGTH_WEBPAGE 256
#define MAX_LENGTH_UP 32

typedef struct data{
    char webpage[MAX_LENGTH_WEBPAGE];
    char username[MAX_LENGTH_UP];
    char password[MAX_LENGTH_UP];
    struct data *next;
}data;

FILE *fp;

void file_open(){
    fp= fopen("hash_table.txt", "r+");
    if(fp==NULL){
        perror("File not opened");
        return;
    }
}

void file_close(){
    fclose(fp);
}

data** table_init(){
    data **hash_table= (data**)malloc(sizeof(data*)*TABLE);
    for (int i = 0; i < TABLE; ++i) {
        hash_table[i]=(data*) malloc(sizeof(data));
        hash_table[i]->next = NULL;
        hash_table[i]->webpage[0] = '/0';
        hash_table[i]->password[0] = '/0';
        hash_table[i]->username[0] = '/0';
    }
    //data **hash_table= calloc(TABLE, sizeof(data*));
    return hash_table;
}



unsigned int hash_function(char *webpage){
    int length = strnlen(webpage, MAX_LENGTH_WEBPAGE);
    long unsigned int hash_value = 1;
    for (int i = 0; i < length; ++i) {
        hash_value *= webpage[i];
        hash_value *= 1000000007;
        hash_value = (hash_value + webpage[i]) % TABLE;
    }
    return hash_value;
}

void new_data(data** hash_table, char *webpage, char *username, char *password){
    data *newdata= (data*) malloc(sizeof(data*));
    strcpy(newdata->webpage, webpage);
    strcpy(newdata->username, username);
    strcpy(newdata->password, password);
    newdata->next=NULL;
    data *current = hash_table[hash_function(webpage)];
    while(current->next!=NULL){
        current=current->next;
    }
    current->next=newdata;
}


void print_table(data **hash_table){
    data *head = malloc(sizeof(data*));
    for (int i = 0; i < TABLE; ++i) {
        head =hash_table[i];
        if(hash_table[i]!=NULL){
            printf("%s %s %s", head->webpage, head->username, head->password);
            while (head->next!=NULL){
                head=head->next;
                printf("\t%s %s %s", head->webpage, head->username, head->password);
            }
        }
        printf("\n");
    }
    free(head);
}

void write_file(data **hash_table){
    data *head = malloc(sizeof(data*));
    for (int i = 0; i < TABLE; ++i) {
        head =hash_table[i];
        if(hash_table[i]->webpage!=NULL){
            fprintf(fp, "%s %s %s\n", head->webpage, head->username, head->password);
            while (head->next!=NULL){
                head=head->next;
                fprintf(fp, "%s %s %s\n", head->webpage, head->username, head->password);
            }
        }
    }
    free(head);
}

void search(char *webpage, data **hash_table){
    if(webpage==NULL) {
        printf("Wrong search\n");
        return;
    }
    unsigned int index= hash_function(webpage);
    data *head =hash_table[index];
    while(head!=NULL){
        if(strncmp(head->webpage, webpage, MAX_LENGTH_WEBPAGE)==0) {
            printf("%s %s %s\n", head->webpage, head->username, head->password);
            return;
        }
        head=head->next;
    }
    printf("Not in the table\n");
}

void delete(char *webpage, data** hash_table){
    unsigned int index = hash_function(webpage);
    data* temporary_before;
    data* temporary;
    temporary_before=NULL;
    temporary=hash_table[index];
    while(temporary!=NULL && strncmp(webpage, temporary->webpage, MAX_LENGTH_WEBPAGE)==0) {
        temporary_before = temporary;
        temporary=temporary->next;
    }
    if(temporary==NULL){
        return;
    } else if(temporary_before==NULL){
        hash_table[index]=temporary->next;
    }else{
        temporary_before->next=temporary->next;
    }
}

void free_list(data** hash_table){
    for (int i = 0; i < TABLE; ++i) {
        data* tmp;
        while (hash_table[i]!=NULL){
            tmp=hash_table[i];
            hash_table[i]=hash_table[i]->next;
            free(tmp);
        }
    }
}
int main(){
    file_open();

    data** hash_table = table_init();
    char *webpage1="1qwer";
    char *username1="2qwer";
    char *password1="3qwer";
    new_data(hash_table, webpage1, username1, password1);
    print_table(hash_table);
    search(webpage1, hash_table);
    print_table(hash_table);
    write_file(hash_table);
    delete(webpage1, hash_table);
    print_table(hash_table);
    free_list(hash_table);
    file_close();
}
//TODO a keresés ne printf et returnoljon mert másra is kell
