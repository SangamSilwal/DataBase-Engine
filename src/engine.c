#include "engine.h"

int read_lines_from_csv(FILE *fp)
{
    char buffer[BUFFER_SIZE];
    int count = 0;
    size_t bytesRead;
    while((bytesRead = fread(buffer,1,BUFFER_SIZE,fp))>0){
        for(size_t i=0;i<bytesRead;i++){
            if(buffer[i] == '\n'){
                count ++;
            }
        }
    }
    return count+1;
}

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

void select_specific_from_table(char *tablename,int *id){
    char filename[FILE_NAME_SIZE],buffer[BUFFER_SIZE],buffer1[SCHEMA_SIZE];
    sprintf(filename,"%s.csv",tablename);

    FILE *file = fopen(filename,"r");
    if(file == NULL)
    {
        printf("\t#======THE TABLE DOESNOT EXISTS======#");
        return;
    }

    while(fgets(buffer,sizeof(buffer),file)){
        strcpy(buffer1,buffer);
        char *token = strtok(buffer1,",");
        if(token && atoi(token) == *id)
        {
            printf("\t#======ROW FOUND: %s ======#\n",buffer);
            fclose(file);
            return;
        }
    }
    printf("\t#======FILE NOT FOUND======#\n");
    fclose(file);
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