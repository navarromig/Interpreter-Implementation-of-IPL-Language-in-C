#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "check.h"


typedef struct memory_list *memory_listptr ;

struct memory_list{       // this list will be used as a "memory" to store every variable the .ipl program will  use 
    char varname[100] ;
    int value ;
    memory_listptr next1 ;
} ;

memory_listptr mem_list= NULL ;


typedef struct line_store_list *line_listptr ;

struct line_store_list{     // this list will store every line of the .ipl program and it's number
    char line[1000] ;
    int line_num ;
    line_listptr next2 ;
};

line_listptr line_list= NULL ;

int return_line(line_listptr, char *) ;

void insert(memory_listptr *mem_list, char *var, int data){
    memory_listptr temp, newnode, prev;
    if ((*mem_list)==NULL){
        memory_listptr newnode ;
        newnode= malloc(sizeof(struct memory_list)) ;
        strcpy(newnode->varname,var) ;
        newnode->value= data ;
        newnode->next1= *mem_list ;
        *mem_list= newnode ;
        return;
    }
    int check = 1 ;
    temp = *mem_list ;
    while (temp != NULL){
        if (strcmp(var,temp->varname)==0){    //check if it exists
            temp->value = data;
            check = 0;
            break; 
        }
        prev = temp;
        temp = temp->next1 ;
    }
    if (check==1){   //doesnt exist..create newnode..
        newnode = malloc(sizeof(struct memory_list));           
        newnode->next1 = NULL;
        newnode->value = data;
        strcpy(newnode->varname, var);
        prev->next1 = newnode;
    }
    return;
}


int isList(memory_listptr mem_list,char *var){ // If it is on the list, then this function returns 1
    memory_listptr temp;
    temp = mem_list ;
    while (temp != NULL){
        if (strcmp(var,temp->varname)==0)
            return 1;
        temp = temp-> next1;
    }
    return 0;
}

int returnValue(memory_listptr mem_list, char *var){  // returns the value of the variable stored in the memory_list. This function is used only when isList returns 1.
    memory_listptr temp;
    temp = mem_list ;
    while (temp != NULL){
        if (strcmp(var,temp->varname)==0)
            return temp->value;
        temp = temp-> next1 ;
    }
    return 0 ;
}


int value(char *word, memory_listptr *mem_list, line_listptr line_list, char *line, int *value){  // Gets a string (variable or number) and stores its value in a variable (int *value)
    int isnumber, length, i, j;
    char help[100];
    strcpy(help,word);
    isnumber= 1 ;
    length= strlen(help) ;
    if (help[0]=='-'){ // ** if the word begins with "-" there is a possibily that it is a negative number. **
        j= 1 ;
    }
    else{
        j= 0 ;
    }
    for (i=j ; i<length ; i++){
        if ( !(help[i]>='0' && help[i]<='9')  ){
            isnumber= 0 ;
            break ;
        }
    }
    if (isnumber==1){   // It is a number
        *value= atoi(help) ;
        if (*value < 0){
            printf("%d. Error! negative number \"%d\"\n",return_line(line_list,line),atoi(help)) ;
            return 1 ;
        }
    }
    else{   // It is a variable name
        // ** It is not a negative number, but an illegal variable name (For example: "-smth") . **
        if (variable_name_check(help,line_list,line)==1){
            return 1 ;
        }
        if (isList(*mem_list,help)==0){
            insert(mem_list,help,0) ;
            *value = 0 ;
        }
        else{
            *value = returnValue(*mem_list,help) ;
        }
    }
    return 0;
}


void insert_2(line_listptr *line_list, char *line, int i){
    line_listptr temp, newnode, prev;
    if ((*line_list)==NULL){
        newnode= malloc(sizeof(struct line_store_list)) ;
        strcpy(newnode->line,line) ;
        newnode->line_num= i ;
        newnode->next2= *line_list ;
        *line_list= newnode ;
        return ;
    }
    temp = *line_list ;
    while (temp != NULL){
        prev = temp;
        temp = temp->next2;
    }
    newnode = malloc(sizeof(struct line_store_list));
    newnode->next2 = NULL;
    newnode->line_num = i;
    strcpy(newnode->line, line);
    prev->next2 = newnode;
    return;
}

int return_line(line_listptr line_list , char *line)  // Recieves a string (line), it searches for it in the list and returns the number of the line
{
    line_listptr temp ;
    temp= line_list ;
    while (temp!=NULL){
        if (strcmp(line,temp->line)==0){
            return temp->line_num ;
        }
        temp= temp->next2 ;
    }
    return 0 ;
}

void print_line(line_listptr line_list , int line)  // Recieves a line number, searches the list for the line corresponding to the line number and prints it, ignoring the comments (#)
{
    line_listptr temp ;
    int j;
    char help[1000] ;
    temp= line_list ;
    while (temp!=NULL){
        if (temp->line_num==line){
            printf("  %d: ",line);
            int i = 0;
            strcpy(help, temp->line);
            while (help[i] != '#' && help[i] != '\0'){ 
               printf("%c",help[i]);
               i++;
            }
            if (help[i] == '#'){ // if I find a comment, the whole process is stopped
                putchar('\n');
            }
            return ;
        }
        temp= temp->next2 ;
    }
    return ;
}

void free_list1(memory_listptr mem_list)
{
    memory_listptr temp ;
    while (mem_list!=NULL){
        temp= mem_list ;
        mem_list= mem_list->next1 ;
        free(temp) ;
    }
    return ;
}

void free_list2(line_listptr line_list)
{
    line_listptr temp ;
    while (line_list!=NULL){
        temp= line_list ;
        line_list= line_list->next2 ;
        free(temp) ;
    }
    return ;
}