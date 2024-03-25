#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lists.h"

int variable_name_check(char *name , line_listptr line_list , char *line )
{
    int length , i ;
    length= strlen(name) ;
    if (strcmp(name,"read")==0 || strcmp(name,"write")==0   || strcmp(name,"writeln")==0 || strcmp(name,"if")==0 || strcmp(name,"while")==0  ||  strcmp(name,"argument")==0 || strcmp(name,"random")==0 || strcmp(name,"else")==0 || strcmp(name,"size")==0){
        printf("%d. Error! : \"%s\" cannot be used as a variable name\n",return_line(line_list,line),name) ;
        return 1 ;
    } 
    if ( isalpha(name[0]) == 0 ){
        printf("%d. Variables must always start with a letter! : \"%s\"\n",return_line(line_list,line),name) ;
        return 1 ;
    }
    for (i=1 ; i<length ; i++){
        if (  isalnum(name[i]) == 0 && (name[i] != '_' )) {
            printf("%d. Illegal variable name! : \"%s\"\n",return_line(line_list,line),name) ;
            return 1 ;
        }
    }
    return 0 ;
}

int number_check(char *argument){  // Checking if the given string is a positive number and returns 1. Otherwise, it returns 0
    int length , i ;
    length= strlen(argument) ;
    for (i=0 ; i<length; i++){
        if (isdigit(argument[i]) == 0){
            return 1;
        }  
    }
    return 0;
}