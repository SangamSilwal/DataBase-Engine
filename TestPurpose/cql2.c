#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define QUERY_SIZE 100
#define TABLE_SIZE 30
#define SCHEMA_SIZE 100
#define DATA_SIZE 100
#define BUFFER_SIZE 8192


void create_table(char *,char *);
void insert_into_table(char *,char *);
void select_all_from_table(char *);
void select_specific_from_table(char *,int *);
void process_query(char *);
int read_lines_from_csv(FILE *);
void merge_CSV_file(char *,char *);

int main()
{
    while(true)
    {
        printf("Enter SQL QUERY: ");
        char query[QUERY_SIZE];
        fgets(query,100,stdin);
        query[strcspn(query,"\n")]=0;
        if(strcmp(query,"EXIT")==0)
        {
            return 0;
        }
        process_query(query);
    }
    return 0;
}


int read_lines_from_csv(FILE *fp)
{
    char buffer[BUFFER_SIZE];
    int count = 0;
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        for (size_t i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n') {
                count++;
            }
        }
    }
    return count+1;
}


void process_query(char *query)
{
    char table_name[TABLE_SIZE],schema[SCHEMA_SIZE],data[DATA_SIZE];int id;
    char fileName1[TABLE_SIZE],fileName2[TABLE_SIZE];
    if((sscanf(query,"CREATE TABLE %s (%[^)])",table_name,schema))==2)
    {
        create_table(table_name,schema);
    }
    else if((sscanf(query,"INSERT INTO TABLE %s (%[^)])",table_name,data))==2)
    {
        insert_into_table(table_name,data);
    }
    else if((sscanf(query,"SELECT * FROM %s",table_name))==1)
    {
        select_all_from_table(table_name);
    }
    else if (sscanf(query, "SELECT FROM %s WHERE %[^=]=%s", table_name, schema, data) == 3) {
        int id = atoi(data);  
        select_specific_from_table(table_name, &id);
    }
    else if(sscanf(query,"%s %s",fileName1,fileName2) == 2)
    {
        merge_CSV_file(fileName1,fileName2);
    }
    else 
    {
        printf("Invalid QUERY\n");
    }
}


void create_table(char *tablename,char *schema)

{
    char filename[100];
    char meta_file[100];
    sprintf(filename,"%s.csv",tablename);
    sprintf(meta_file,"%s.meta",tablename);

    FILE *file,*metafile;
    file = fopen(filename,"w");
    metafile = fopen(meta_file,"w");

    if(file == NULL || meta_file == NULL)
    {
        printf("Sorry Cannot create the table\n");
        return;
    }
    fprintf(file,"%s\n",schema);
    fclose(file);
    fprintf(metafile,"0\n");
    fclose(metafile);
    printf("Table has been created Succesfully\n");
}

void insert_into_table(char *tablename,char *data)
{
    char filename[100];
    char meta_file[100];
    sprintf(filename,"%s.csv",tablename);
    sprintf(meta_file,"%s.meta",tablename);
    FILE *file,*metafile,*mta;
    file = fopen(filename,"a");
    metafile = fopen(meta_file,"r");
    if(file == NULL || meta_file == NULL)
    {
        printf("\nThe TableName Doesnot exists\n");
        return;
    }
    fprintf(file,"%s\n",data);
    fclose(file);

    int i=read_lines_from_csv(metafile);
    fclose(metafile);

    mta = fopen(meta_file,"a");
    fprintf(mta,"%d\n",i+1);
    fclose(mta);

    printf("\nData has been added succesfully\n");
}


void select_all_from_table(char *tablename)
{
    char filename[100];
    char meta_file[100];
    char data[SCHEMA_SIZE];
    char id[3];
    sprintf(filename,"%s.csv",tablename);
    sprintf(meta_file,"%s.meta",tablename);
    FILE *file,*metafile;
    file = fopen(filename,"r");
    metafile = fopen(meta_file,"r");

    if(!file || !metafile)
    {
        printf("The table Doesnot EXISTS\n");
        return;
    }
    while(fgets(data,SCHEMA_SIZE,file) && fgets(id,3,metafile))
    {
        id[strcspn(id, "\n")] = 0;  
        data[strcspn(data, "\n")] = 0;
        printf("%s-\t%s\n",id,data);
    }
}


void select_specific_from_table(char *tablename,int *id ) {
    char filename[100], buffer[SCHEMA_SIZE],buffer1[SCHEMA_SIZE];
    sprintf(filename, "%s.csv", tablename);

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Table does not exist!\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        strcpy(buffer1, buffer);
        char *token = strtok(buffer1, ","); 
        if (token && atoi(token) == *id) { 
            printf("Row found: %s\n", buffer);
            fclose(file);
            return;
            }
    }
    
    printf("ID %d not found!\n", *id);
    fclose(file);
}


void merge_CSV_file(char *csv_file_1,char *csv_file_2)
{
    char filename1[100],filename2[100],schma1[SCHEMA_SIZE],schma2[SCHEMA_SIZE];
    sprintf(filename1,"%s.csv",csv_file_1);
    sprintf(filename2,"%s.csv",csv_file_2);

    FILE *fp_1,*fp_2;

    fp_1 = fopen(filename1,"r");
    fp_2 = fopen(filename2,"r");

    //Just storing the first of the file in the schma 1 and 2
    fgets(schma1,SCHEMA_SIZE,fp_1);
    fgets(schma2,SCHEMA_SIZE,fp_2);

    // Comparing if the schma of both lines matches or not
    if(strcmp(schma1,schma2) != 0)
    {
        printf("Can only Merge same type of file\nThe file you have entered contains different type of Data\n");
        return;
    }
    printf("Yes it matches\n");




}


