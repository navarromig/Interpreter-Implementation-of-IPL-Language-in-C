#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

char ** words_function(char *line) //receives a line, breaks it down into key words and stores them in an array
{
    char line_words[1000] , line_operator[1000] , *p , *words2[100], **words ;
    int i , j , size , words2_counter ;
    words = malloc(sizeof(char *) * 100);
    for (j=0 ; j<100 ; j++){
        words[j] = "1empty" ; //array for operators
        words2[j]= "1empty" ; //array for words except comments and operators 
    }
    strcpy(line_words,line); //to be able to edit the line
    p= strtok(line_words," \n\t=-*+<>!%/") ;
    j=0;
    while (p != NULL){
        if (p[0] == '#' ){
            break;
        }
        words2[j]= p ;
        j++ ;
        p= strtok(NULL," \n\t=-*+<>!%/") ;
    }
    strcpy(line_operator,line);
    words2_counter = 0;
    j = 0;
    i = 0; 
    while (line_operator[i]!='#' && i<strlen(line_operator)-1){ //checking the characters of the line for operators 
    // I have written "i<strlen(line_operator)-1", because every line ends with two characters : '\n' and '\0' 
        if (line_operator[i]=='=' || line_operator[i]=='+' || line_operator[i]=='-' || line_operator[i]=='/' || line_operator[i]=='*' || line_operator[i]=='%' || line_operator[i]=='>' || line_operator[i]=='<' || line_operator[i]=='!'){
            if (i==0){ //it's not possible to begin with an operator  ex. "=  a  "
                printf("Error!\n") ;
                return NULL ;
            }
            if (line_operator[i]!='=' && line_operator[i]!='<' && line_operator[i]!='>' && line_operator[i]!='!'){ //in this case there is 1 character operator
                if (line_operator[i]=='+'){
                    words[j] = "+" ;  
                }
                else if (line_operator[i]=='-'){
                    words[j]= "-" ;
                }
                else if (line_operator[i]=='/'){
                    words[j]= "/" ;
                }
                else if (line_operator[i]=='*'){
                    words[j]= "*" ;
                }
                else {
                    words[j]= "%" ;
                }
            }
            else{ //checking for the next character if it is also an operator
                if (line_operator[i] == '='){
                    if (line_operator[i+1] == '='){
                       words[j]= "==" ;
                        i++; //icreased by 1 because of the double character operator
                    }
                    else{
                        words[j] = "=";
                    } 
                }
                else if (line_operator[i] == '!' && line_operator[i+1] == '=') {
                   words[j]= "!=" ; 
                   i++;

                }
                else if (line_operator[i] == '<')
                {
                    if (line_operator[i+1] == '='){
                        words[j] ="<=" ;
                        i++;
                    }
                    else{
                        words[j]= "<" ;
                    }   
                }
                else if (line_operator[i] == '>')
                {
                    if (line_operator[i+1] == '='){
                        words[j]= ">=" ;
                        i++;
                    }
                    else{
                        words[j] = ">";
                    }
                }
            }
            i++ ;
            j++ ;
        }
        else if(line_operator[i] != '\t' && line_operator[i] != ' ') {
            p = words2[words2_counter];
            words2_counter++;
            size = strlen(p);
            words[j] = malloc(sizeof(char) * (size+1));  // size+1 because of the '\0' at the end of every string. 
            strcpy(words[j],p);
            j++;
            i += size ; // the i counter must be increased by the size
        }
        else{
            i++ ;
        }
    }
    return words ;
}


void free_words_function(char **words){ 
    int j , i ;
    for (j=0; j<100; j++){
        if ( (strcmp(words[j],"1empty") != 0) && (strcmp(words[j],"=") != 0) && (strcmp(words[j],"==") != 0) && (strcmp(words[j],"<=") != 0) && (strcmp(words[j],">=") != 0) && (strcmp(words[j],"!=") != 0 )
            && (strcmp(words[j],"<") != 0) && (strcmp(words[j],">") != 0) && (strcmp(words[j],"+") != 0) && (strcmp(words[j],"-") != 0) && (strcmp(words[j],"/") != 0) && (strcmp(words[j],"*") != 0) && (strcmp(words[j],"%") != 0))
        {// in these cases we have used malloc to store the words, so it must be freed
           free(words[j]);
        }
    }
    free(words);
    return;
}


