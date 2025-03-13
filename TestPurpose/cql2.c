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
#define MAX_LINE 1024
#define MAX_COLUMN 100


void create_table(char *,char *);
void insert_into_table(char *,char *);
void select_all_from_table(char *);
void select_specific_from_table(char *,int *);
void process_query(char *);
int read_lines_from_csv(FILE *);
void merge_CSV_file(char *,char *);
void delete_from_csv(const char *,const char *,const char *);
void read_from_csv(const char *,const char *,double );
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
    int range;
    // if((sscanf(query,"CREATE TABLE %s (%[^)])",table_name,schema))==2)
    // {
    //     create_table(table_name,schema);
    // }
    // else if((sscanf(query,"INSERT INTO TABLE %s (%[^)])",table_name,data))==2)
    // {
    //     insert_into_table(table_name,data);
    // }
   
    // else if (sscanf(query, "SELECT FROM %s WHERE %[^=]=%s", table_name, schema, data) == 3) {
    //     int id = atoi(data);  
    //     select_specific_from_table(table_name, &id);
    // }
    // else if(sscanf(query,"%s %s",fileName1,fileName2) == 2)
    // {
    //     merge_CSV_file(fileName1,fileName2);
    // }
    // else if(sscanf(query,"%s %s %s",table_name,schema,data) == 3)
    // {
    //     delete_from_csv(table_name,schema,data);
    // }
    if(sscanf(query,"READ %s %s %d",table_name,schema,&range) == 3)
    {
        printf("hey");
        read_from_csv(table_name,schema,range);
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


void read_from_csv(const char *filename, const char *attribute, double range) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("\t#======ERROR: FILE DOES NOT EXIST======#\n");
        return;
    }

    char line[MAX_LINE];
    char row[MAX_COLUMN][MAX_LINE];
    int col_count = 0, column_index = -1;

    // Read the first line to find the column index
    if (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",\n");
        col_count = 0;

        while (token) {
            if (strcmp(token, attribute) == 0) {
                column_index = col_count;
            }
            strcpy(row[col_count++], token);
            token = strtok(NULL, ",\n");
        }

        if (column_index == -1) {
            printf("\t#======ERROR: ATTRIBUTE %s NOT FOUND IN TABLE======#\n", attribute);
            fclose(file);
            return;
        }
    }

    // Now, read the remaining lines to filter based on the condition
    printf("\n#======ROWS WHERE %s > %.2f======#\n", attribute, range);
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",\n");
        int col_idx = 0;

        while (token) {
            strcpy(row[col_idx++], token);
            token = strtok(NULL, ",\n");
        }

        // Convert the value of the specified column to a number and compare
        double value = atof(row[column_index]); // assuming the column has numerical values
        if (value > range) {
            for (int i = 0; i < col_count; i++) {
                printf("|%s ", row[i]);
            }
            printf("\n");
        }
    }

    fclose(file);
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
void delete_from_csv(const char *table_name, const char *column_name, const char *value) {
    char filename[MAX_LINE];
    snprintf(filename, sizeof(filename), "%s.csv", table_name); // CSV file instead of TXT

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Table '%s' not found.\n", table_name);
        return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    char line[MAX_LINE];
    int column_index = -1;
    int first_line = 1;

    while (fgets(line, sizeof(line), file)) {
        char row[MAX_COLUMN][MAX_LINE];  // Store columns in an array
        int col_count = 0;

        // Copy line to avoid strtok modifying the original
        char temp_line[MAX_LINE];
        strcpy(temp_line, line);

        // Tokenize the row
        char *token = strtok(temp_line, ",\n");
        while (token) {
            strcpy(row[col_count++], token);
            token = strtok(NULL, ",\n");
        }

        // Determine column index from header
        if (first_line) {
            first_line = 0;
            for (int i = 0; i < col_count; i++) {
                if (strcmp(row[i], column_name) == 0) {
                    column_index = i;
                    break;
                }
            }
            if (column_index == -1) {
                printf("Error: Column '%s' not found.\n", column_name);
                fclose(file);
                fclose(temp);
                remove("temp.csv");
                return;
            }
            // Write header to temp file
            fprintf(temp, "%s", line);
            continue;
        }

        // If the row matches the condition, skip writing it (i.e., delete it)
        if (strcmp(row[column_index], value) == 0) {
            continue;
        }

        // Write the row back in proper CSV format
        for (int i = 0; i < col_count; i++) {
            fprintf(temp, "%s", row[i]);
            if (i < col_count - 1) {
                fprintf(temp, ",");
            }
        }
        fprintf(temp, "\n");  // Add newline after the row
    }

    fclose(file);
    fclose(temp);

    // Replace original file with temp file
    remove(filename);
    rename("temp.csv", filename);

    printf("Deleted rows where %s = %s from table %s.\n", column_name, value, table_name);
}


