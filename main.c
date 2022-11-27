#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"

#define TABLE 57
#define MAX_LENGTH_WEBPAGE 256
#define MAX_LENGTH_UP 32

typedef struct data {
    char webpage[MAX_LENGTH_WEBPAGE];
    char username[MAX_LENGTH_UP];
    char password[MAX_LENGTH_UP];
    struct data *next;
} data;

data **filebol_memoriaba();

data **table_init() {
    data **hash_table = (data **) malloc(sizeof(data *) * TABLE);
    for (int i = 0; i < TABLE; ++i) {
        hash_table[i] = (data *) malloc(sizeof(data));
        hash_table[i]->next = NULL;
        hash_table[i]->webpage[0] = '\0';
        hash_table[i]->password[0] = '\0';
        hash_table[i]->username[0] = '\0';
    }
    //data **hash_table= calloc(TABLE, sizeof(data*));
    return hash_table;
}


unsigned int hash_function(char *webpage) {
    int length = strnlen(webpage, MAX_LENGTH_WEBPAGE);
    long unsigned int hash_value = 1;
    for (int i = 0; i < length; ++i) {
        hash_value *= webpage[i];
        hash_value *= 1000000007;
        hash_value = (hash_value + webpage[i]) % TABLE;
    }
    return hash_value;
}

void new_data(data **hash_table, char *webpage, char *username, char *password) {
    data *newdata = (data*) malloc(sizeof(data));
    strcpy(newdata->webpage, webpage);
    strcpy(newdata->username, username);
    strcpy(newdata->password, password);
    newdata->next = NULL;
    data *current = hash_table[hash_function(webpage)];
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newdata;
}

void print_table(data **hash_table) {
    data *head;
    for (int i = 0; i < TABLE; ++i) {
        head = hash_table[i]->next;
        if (head != NULL) {
            printf(">>>   %s\t\t\t\t\t%s\t\t\t\t\t%s", head->webpage, head->username, head->password);
            printf(" | \n---   ");
            while (head->next != NULL) {
                head = head->next;
                printf("%s\t\t\t\t\t%s\t\t\t\t\t%s", head->webpage, head->username, head->password);
                printf(" | ");
                printf("\n---   ");
            }
        } else {
            printf(">>>   %s \t\t\t\t\t%s \t\t\t\t\t%s\t |", "null", "null", "null");
        }
        printf("\n");
    }
}

void write_file(data **hash_table) {
    FILE *fp = fopen("hash_table.txt", "w");
    for (int i = 0; i < TABLE; ++i) {
        data *head = hash_table[i]->next;
        while (head != NULL) {
            fprintf(fp, "%s %s %s\n", head->webpage, head->username, head->password);
            head = head->next;
        }
    }
    fclose(fp);
}

void search(char *webpage, data **hash_table) {
    if (webpage == NULL) {
        printf("Wrong search\n");
        return;
    }
    unsigned int index = hash_function(webpage);
    data *head = hash_table[index];
    while (head != NULL) {
        if (strncmp(head->webpage, webpage, MAX_LENGTH_WEBPAGE) == 0) {
            printf("\n \n --Search result: ");
            printf("%s %s %s\n", head->webpage, head->username, head->password);
            return;
        }
        head = head->next;
    }
    printf("Not in the table\n");
}

void delete(char *webpage, data **hash_table) {
    unsigned int index = hash_function(webpage);
    data *temporary_before;
    data *temporary;
    temporary_before = NULL;
    temporary = hash_table[index];
    while (temporary != NULL && strncmp(webpage, temporary->webpage, MAX_LENGTH_WEBPAGE) == 0) {
        temporary_before = temporary;
        temporary = temporary->next;
    }
    if (temporary == NULL) {
        return;
    } else if (temporary_before == NULL) {
        hash_table[index] = temporary->next;
    } else {
        temporary_before->next = temporary->next;
    }
}

void free_list(data **hash_table) {
    for (int i = 0; i < TABLE; ++i) {
        data *tmp;
        while (hash_table[i] != NULL) {
            tmp = hash_table[i];
            hash_table[i] = hash_table[i]->next;
            free(tmp);
        }
    }
}

int main() {

    data **hash_table = filebol_memoriaba();
    print_table(hash_table);
    write_file(hash_table);
    free_list(hash_table);
    free(hash_table);
    //search("1qwerty", hash_table);
}


data **filebol_memoriaba() {
    data **hash_table = table_init();
    char webpage[MAX_LENGTH_WEBPAGE];
    char username[MAX_LENGTH_UP];
    char password[MAX_LENGTH_UP];
    FILE* fp = fopen("hash_table.txt", "r");


    bool nem_eof = (fscanf(fp, "%s %s %s", webpage, username, password) == 3);
    while (nem_eof) {
        new_data(hash_table, webpage, username, password);
        nem_eof = (fscanf(fp, "%s %s %s", webpage, username, password) == 3);
    }
    fclose(fp);
    return hash_table;
}
