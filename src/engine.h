#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define QUERY_SIZE 100
#define TABLE_SIZE 30
#define SCHEMA_SIZE 100
#define DATA_SIZE 100
#define BUFFER_SIZE 8192
#define FILE_NAME_SIZE 100

void create_table(char *,char *);
void insert_into_table(char *,char *);
void select_all_from_table(char *);
void select_specific_from_table(char *,char *);
void process_query(char *);
void merge_CSV_file(char *,char *);

