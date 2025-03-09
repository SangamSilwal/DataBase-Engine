#include "engine.h"

struct buffer_string{
    char buffer_str[BUFFER_SIZE];
};


//#====== Function to create a Table ======#
void create_table(char *tablename,char *schema)
{
    char filename[FILE_NAME_SIZE];
    sprintf(filename,"%s.csv",tablename);
    FILE *file;
    file = fopen(filename,"w");
    if(file == NULL){
        printf("#======THE TABLE CANNOT BE CREATED======#");
        return;
    }
    fprintf(file,"%s\n",schema);
    fclose(file);
    printf("#======THE TABLE HAS BEEN CREATED SUCCESSFULLY======#\n");
}

//#====== Function to insert into the table ======#
void insert_into_table(char *tablename,char *data)
{
    char filename[FILE_NAME_SIZE];
    sprintf(filename,"%s.csv",tablename);
    FILE *file;
    file = fopen(filename,"a");
    if(file == NULL)
    {
        printf("#======THE TABLE NAME DOESNOT EXISTS======#");
        return;
    }
    fprintf(file,"%s\n",data);
    fclose(file);
    printf("#======SUCCESSFULLY INSERTED INTO THE TABLE======#");
}


//#====== Function to select all form the table ======#
void select_all_from_table(char *tablename)
{
    char filename[FILE_NAME_SIZE];
    char data[SCHEMA_SIZE];
    sprintf(filename,"%s.csv",tablename);
    FILE *file;
    file = fopen(filename,"r");
    if(file == NULL)
    {
        printf("\t#======THE TABLE DOESNOT EXISTS======#");
        return;
    }
    while(fgets(data,SCHEMA_SIZE,file))
    {
        data[strcspn(data,"\n")] = 0;
        printf("\t\t%s\n",data);
    }
}

//#====== Function to select specific attributes's value from the file ======#
void select_specific_from_table(char *tablename, char *condition) {
    char filename[FILE_NAME_SIZE], line[BUFFER_SIZE], temp_line[SCHEMA_SIZE];

    // ######   Opening the csv file for reading purpose  ######
    sprintf(filename, "%s.csv", tablename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("\t#====== THE TABLE DOES NOT EXIST ======#\n");
        return;
    }

    //#========= Dynamic memory allocation for storing conditions =========#
    struct buffer_string *bs[2];
    for (int i = 0; i < 2; i++) {
        bs[i] = (struct buffer_string *)malloc(sizeof(struct buffer_string));
    }

    //#========= Tokenizing the condition: "attribute=value" =========#
    char *token = strtok(condition, "=");
    int i = 0;
    while (token != NULL && i < 2) {
        strncpy(bs[i]->buffer_str, token, sizeof(bs[i]->buffer_str) - 1);
        bs[i]->buffer_str[sizeof(bs[i]->buffer_str) - 1] = '\0';
        token = strtok(NULL, "=");
        i++;
    }

    // #========= Checking the format of input here also =========#
    if (i < 2) {
        printf("\t#====== INVALID CONDITION FORMAT ======#\n");
        fclose(file);
        return;
    }

    // #========= Getting the attribute and value to search for =========#
    char *search_attribute = bs[0]->buffer_str;
    char *search_value = bs[1]->buffer_str;

    // #=========Reading the header =========#
    if (!fgets(line, sizeof(line), file)) {
        printf("\t#====== EMPTY FILE ======#\n");
        fclose(file);
        return;
    }

    //#========= Extract column names =========#
    strcpy(temp_line, line);
    char *column_token = strtok(temp_line, ",");
    int column_index = -1, index = 0;

    // #========= Find the column index of the attribute using case-insensitive comparison =========#
    while (column_token != NULL) {
        if (strcmp(column_token, search_attribute) == 0) {
            column_index = index;
            break;
        }
        column_token = strtok(NULL, ",");
        index++;
    }

    if (column_index == -1) {
        printf("\t#====== ATTRIBUTE NOT FOUND ======#\n");
        fclose(file);
        return;
    }
    printf("\t#======MATCHED RECORDS ARE:  =======#\n");

    //#========= reading the row and searching for the value =========#
    while (fgets(line, sizeof(line), file)) {
        strcpy(temp_line, line);
        char *data_token = strtok(temp_line, ",");
        int current_index = 0;

        while (data_token != NULL) {
            if (current_index == column_index && strcmp(data_token, search_value) == 0) {
               
                printf("\t%s\n", line); 
                break;
            }
            data_token = strtok(NULL, ",");
            current_index++;
        }
    }

    //# ========= Free dynamically allocated memory =========#
    for (int i = 0; i < 2; i++) {
        free(bs[i]);
    }

    fclose(file);
}

//# ========= Processing the queries in with the above function ========= #
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
    else if (sscanf(query, "SELECT FROM %s WHERE %s", table_name, schema )== 2) {
       
        select_specific_from_table(table_name, schema);
    }
    else 
    {
        printf("\t#======INVALID QUERY======#\n");
    }
}


int main()
{
    while(true)
    {
        printf("\tSQL QUERY: ");
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