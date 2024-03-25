#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lists.h"
#include "words.h"
#include "check.h"

int rec_main(memory_listptr, fpos_t, int, FILE *, int *, int, line_listptr, int);

int read(char **words , memory_listptr *mem_list, line_listptr line_list, char *line){
    int value;
    char help[1000],i ;
    if (strcmp(words[2],"1empty")!=0 || strcmp(words[1],"1empty")==0){
        printf("%d: Wrong syntax of \"read\" command\n",return_line(line_list,line));
        return 1 ;
    }
    strcpy(help,words[1]) ;
    if (variable_name_check(help,line_list,line)==1){
        return 1 ;
    }
    printf("%s= ",help) ;
    if ( scanf("%d",&value) != 1){
        printf("%d: Invalid input!\n",return_line(line_list,line));
        return 1;
    }
    insert(mem_list,help,value) ;
    return 0;
}

int write (char **words , memory_listptr *mem_list, line_listptr line_list, char *line){

    int value1 ;
    if (strcmp(words[1],"1empty")==0 || strcmp(words[2],"1empty")!=0){
        printf("%d. Invalid syntax of \"write\" command\n",return_line(line_list,line)) ;
        return 1 ;
    }  
    if (value(words[1],mem_list,line_list,line,&value1)==1){  // a function that checks if the word is a number or a variable and returns it's value
        return 1 ;
    }
    printf("%d",value1) ;
    return 0 ;
}

int computations(char **words , memory_listptr *mem_list, line_listptr line_list, char *line)
{
    int isnumber , length , value1 , value2 , i , j ;
    char help[100] , operator ;

    if (strcmp(words[5],"1empty")!=0  || strcmp(words[2],"1empty")==0){ // Error examples: "a = a + a g" or "a =     "
        printf("%d. Wrong syntax of expression\n",return_line(line_list,line)) ;
        return 1 ;
    }
    if (variable_name_check(words[0],line_list,line)==1){
        return 1 ;
    }

    if (value( words[2], mem_list,line_list,line, &value1) == 1 ){  
        return 1;
    }
    if (strcmp(words[3],"1empty")!=0){ // if the computation has an operator
        if (value( words[4], mem_list,line_list,line, &value2) == 1 ){
            return 1;
        }
        strcpy(help,words[3]) ;
        operator= help[0] ; // help[0]='operator' and help[1]='\0'  
        if (operator=='+' || operator=='-' || operator=='*' || operator=='/' || operator=='%'){
            if (isList(*mem_list,words[0])==0){
                insert(mem_list,words[0],0) ;
            }
            switch (operator){
                case ('+') :  insert(mem_list,words[0],value1+value2) ;
                break ;
                case ('-') :  insert(mem_list,words[0],value1-value2) ;
                break ;
                case ('*') : insert(mem_list,words[0],value1*value2) ;
                break ;
                case ('/') : if (value2==0){
                    printf("%d. Cannot divide by zero!\n",return_line(line_list,line)) ;
                    return 1 ;
                }
                insert(mem_list,words[0],value1/value2) ;
                break ;
                case ('%') : if (value2==0){
                    printf("%d. Cannot divide by zero!\n",return_line(line_list,line)) ;
                    return 1 ;
                }
                insert(mem_list,words[0],value1%value2) ;
                break ;
            }
        }
        else{
            printf("%d. Could not find operator!\n",return_line(line_list,line)) ;
            return 1 ;
        }
    }
    else{
        if (isList(*mem_list,words[0])==0){             
            insert(mem_list,words[0],0) ;
        }
        insert(mem_list,words[0],value1) ;
    }
    return 0 ;
}


int condition(char **words, memory_listptr *mem_list, int *result, line_listptr line_list, char *line){  //  This function is used in while_function and if_function below
    char help[100], logic[3] ; // logic's size is 3 because of the '\0' in the end of the string 
    int value1, isnumber,length, j, i, value2;
    i=0;
    while (strcmp(words[i],"1empty") != 0){
        i++;
    }
    if (i != 4){ 
        printf("%d. Wrong syntax of condition\n",return_line(line_list,line)) ;
        return 1;
    }
    if (value( words[1], mem_list,line_list,line, &value1) == 1 )
        return 1;
    if (value( words[3], mem_list,line_list,line, &value2) == 1 )
        return 1;
    strcpy(logic,words[2]);
    if (strcmp(logic,"==") == 0)
        *result = (value1 == value2);
    else if (strcmp(logic,"!=") == 0)
        *result = (value1 != value2);
    else if (strcmp(logic,"<=") == 0)
        *result = (value1 <= value2);
    else if (strcmp(logic,"<") == 0)
        *result = (value1 < value2);
    else if (strcmp(logic,">=") == 0)
        *result = (value1 >= value2);
    else if (strcmp(logic,">") == 0)
        *result = (value1 > value2);
    else{
        printf("%d. Logic operator not found\n",return_line(line_list,line));
        return 1;
    }
    return 0;
}

fpos_t return_end_of_block(int nesting , FILE *fp){ //  This function is used in while_function and if_function below
    int tabcounter, end;
    char help[1000], line[1000], *p;
    fpos_t position_end;
    do{ 
        tabcounter = 0 ;
        end = 0;
        fgetpos(fp,&position_end); //getting the position of the last line of "block command" in while or if
        if (fgets(line,sizeof(line),fp) == NULL){
            end =1 ;              //end is 1 when end of file is reached
        }
        strcpy(help,line) ;
        p= strtok(help," \t") ;
        if (p[0]=='#' || p[0]=='\n'){    //if the line contains only a comment or if it is an empty line, then I ignore it and move on to the next line
            do {                        //if the line is empty after the use of the strtok function it contains only \n
                if (fgets(line,sizeof(line),fp)==NULL){
                    end= 1 ;
                }
                strcpy(help,line) ;
                p= strtok(help," \t") ; 
            }while (  (p[0] == '#' || p[0]=='\n' ) && end == 0 ) ;
        }
        while (line[tabcounter] == '\t'){
            tabcounter++;
        }
    }while (  nesting >= 1 && tabcounter >= nesting && end == 0 ); //the loop is continued until a command is found that it's not included in "command block" of while/if or if EOF is read
    return position_end ;
}


int while_function(char **words, memory_listptr *mem_list, fpos_t line_position , int nesting , FILE *fp, int *arg_nums, int size_arguments, line_listptr line_list, int check_v, char *line){  
    fpos_t position_end;
    int result ;
    if (condition(words,mem_list,&result,line_list,line) == 1){ //checking the condition and setting result as 1 or 0
        return 1;                                               //if result is 0 the condition is false, if 1 is true 
    }
    if (result == 0){
        position_end= return_end_of_block(nesting,fp) ;
        fsetpos(fp,&position_end);   //setting fp to the position of the last line inside the while. With fgets I will get the first line outside the while
    }
    while (result == 1){ //if result is 1 I execute all the command block until result is 0
        if (rec_main(*mem_list,line_position,nesting,fp,arg_nums,size_arguments,line_list,check_v)==1){
            return 1 ;
        }
        if (condition(words,mem_list,&result,line_list,line) == 1){
            return 1;
        }
        if (result == 0){
            fsetpos(fp,&line_position); //setting fp to the beginning of while "command block"
            position_end= return_end_of_block(nesting,fp) ;
            fsetpos(fp,&position_end);  
        }
    }
    return 0;  
}


int if_function( char **words, memory_listptr *mem_list, fpos_t position_if, int nesting, FILE *fp, int *arg_nums, int size_arguments, line_listptr line_list, int check_v, char *line){
    char line_1[1000],tabcounter, *p, *words2[100] , help[1000]  ;  // line_1 : lines that will be read inside the "if" or "else" command blocks
    fpos_t position_else, position_end;
    int j, result,end;
    if (condition(words,mem_list,&result,line_list,line) == 1){  //checking the condition and setting result as 1 or 0
        return 1;                                               //if result is 0 the condtion is false, if 1 is true 
    }
    end = 0;
    do{ 
        fgetpos(fp,&position_end); 
        if (fgets(line_1,sizeof(line_1),fp) == NULL){
           end =1;  //end is 1 when end of file is reached
        }
        strcpy(help,line_1) ;
        p= strtok(help," \t") ;
        if (p[0]=='#' || p[0]=='\n'){    //if the line contains only a comment or if it is an empty line, then I ignore it and move on to the next line
            do {   //if the line is empty after the use of the strtok function it contains only \n
                if (fgets(line_1,sizeof(line_1),fp)==NULL){
                    end= 1 ;
                }
                strcpy(help,line_1) ;
                p= strtok(help," \t") ; 
            }while (  (p[0] == '#' || p[0]=='\n' ) && end == 0 ) ;
        }
        for (j=0 ; j<100 ; j++){ //creating an temporary array to save the words of each line, to be able to find if there is an "else"
            words2[j]= "1empty" ;
        }
        tabcounter = 0;
        while (line_1[tabcounter] == '\t'){
            tabcounter++;
        }
        j= 0 ;
        p= strtok(line_1," \n\t") ;
        while (p != NULL){ 
            words2[j]= p ;
            j++ ;
            p= strtok(NULL," \n\t") ;
        }
    } while(  nesting >= 1  && tabcounter >= nesting  && (end ==0) ) ; //the loop is continued until a command is found that it's not included in "command block" of if statement or EOF is read
    if (strcmp(words2[0],"else")==0){  
        if (strcmp(words2[1],"1empty")!=0 ){
            printf("%d. Expected end of else statement\n",return_line(line_list,line_1));
            return 1;
        }
        fgetpos(fp,&position_else);
        position_end= return_end_of_block(nesting,fp) ;
        //now we know the begining/end of "if/else" command block  
        if (result == 1){ //I will execute the command block inside if
            if (rec_main(*mem_list,position_if,nesting,fp,arg_nums,size_arguments,line_list,check_v)==1)
                return 1 ;
        }
        else{  //I will execute the command block inside else
            if (rec_main(*mem_list,position_else,nesting,fp,arg_nums,size_arguments,line_list,check_v)==1)
                return 1 ;
        }
    }
     else { //there is no else...only if
        if (result == 1){
            if (rec_main(*mem_list,position_if,nesting,fp, arg_nums,size_arguments,line_list,check_v)==1)
                return 1 ;
        }
    }
    fsetpos(fp,&position_end); 
    return 0;
}

int random_function(char **words , memory_listptr *mem_list,line_listptr line_list, char *line) 
{
    int value ;
    value= rand() ;
    char help[1000] ;  
    if (strcmp(words[2],"1empty")!=0 || strcmp(words[1],"1empty")==0){  
    printf("%d. Wrong syntax of \"random\" command\n",return_line(line_list,line)) ;
    return 1 ;
    }
    strcpy(help,words[1]) ;
    if (variable_name_check(help,line_list,line)==1){
        return 1 ;
    } 
    insert(mem_list,help,value) ;
    return 0;
}

int argument_function(char **words, memory_listptr *mem_list,int *arg_nums, int size_arguments,line_listptr line_list, char *line){

    char help[1000];
    int number;

    if (strcmp(words[1],"1empty") == 0 || strcmp(words[2],"1empty")==0 || strcmp(words[3],"1empty") != 0){
        printf("%d. Wrong syntax of \"argument\" command\n",return_line(line_list,line)) ; 
        return 1;
    }

    if (strcmp(words[1],"size") == 0){ // "argument size <var>"
        strcpy(help,words[2]);
        if (variable_name_check(help,line_list,line) == 1){
            return 1;
        }
        insert(mem_list,help,size_arguments) ;
        return 0;

    }
    else{ // "argument <varint> <var>"
        if ( (number_check(words[1]) == 1) && (variable_name_check(words[1],line_list,line) == 1) ){
            printf("%d. Wrong syntax of \"argument\" command\n",return_line(line_list,line)) ; 
            return 1 ;
        }
        if (number_check(words[1]) == 0){
            number = atoi(words[1]);
        }
        else{
            if (isList(*mem_list, words[1]) == 0){
                number = 0; //the variable does not exist, it has the value 0, there is no argument 0. It's an error.
            }
            else{
                number = returnValue(*mem_list,words[1]);
            }
        }
        if (number > size_arguments || number <= 0){ // number must be within [1,size_arguments]
            printf("%d. Invalid argument\n",return_line(line_list,line)) ;
            return 1;
        }
        strcpy(help,words[2]);
        if (variable_name_check(help,line_list,line) == 1){
            return 1;
        }
        insert(mem_list,help,arg_nums[number-1]) ;
        return 0;

    }
}


int rec_main(memory_listptr mem_list , fpos_t line_position , int nesting , FILE *fp, int *arg_nums, int size_arguments, line_listptr line_list, int check_v)
{
    fpos_t position;
    int tabcounter, end ;
    char line[1000] , *p, **words , help[1000] ;
    int i , j ;
    end = 0;
    fsetpos(fp,&line_position) ; //setting to fp the line after which I want to start the execution
    if (fgets(line,sizeof(line),fp)==NULL){
        printf("Empty block\n") ;
        return 1 ;
    }
    strcpy(help,line);
    p= strtok(help," \t") ;
    if (p[0]=='#' || p[0]=='\n'){
        do {
            if (fgets(line,sizeof(line),fp)==NULL){
                end= 1 ;
            }
            strcpy(help,line) ;
            p= strtok(help," \t") ; 
        }while ((p[0] == '#' || p[0]=='\n' ) && end == 0) ;
    }
    strcpy(help,line) ;
    tabcounter = 0;
    while (line[tabcounter] == '\t'){
        tabcounter++;
    }
    if (tabcounter != nesting){
        if (tabcounter <= nesting - 1){
            printf("Empty block\n");
            return 1;
        }
        printf("%d. Invalid depth\n",return_line(line_list,line));
        return 1;
    }
    do{
        words = words_function(line);
        if (words==NULL){
            return 1 ;
        }
        if (strcmp(words[0],"read")==0){  
            if (check_v==1)
            print_line(line_list,return_line(line_list,help)) ;
            if (read(words, &mem_list,line_list,line)==1){
                return 1 ;
            }
        }                                                                       
        else if (strcmp(words[0],"write")==0){
            if (check_v==1)
            print_line(line_list,return_line(line_list,help)) ;
            if (write(words,&mem_list,line_list,line)==1){
                return 1 ;
            }
            printf(" ") ;
        }
        else if (strcmp(words[0],"writeln")==0){
            if (check_v==1)
            print_line(line_list,return_line(line_list,help)) ;
            if (write(words,&mem_list,line_list,line)==1){
                return 1 ;
            }
            printf("\n") ;
        }                                                                  
        else if (strcmp(words[1],"=")==0){
            if (check_v==1)
                print_line(line_list,return_line(line_list,help)) ;
            if (computations(words,&mem_list,line_list,line)==1){
                return 1 ;
            }
        }
        else if (strcmp(words[0],"while")==0){
            if (check_v==1)
            print_line(line_list,return_line(line_list,help)) ;
            fgetpos(fp,&position) ;
            if (while_function(words,&mem_list,position,nesting+1,fp,arg_nums, size_arguments,line_list,check_v,line)==1){
                return 1 ;
            }
        }
       else if (strcmp(words[0],"if")==0){
           if (check_v==1){
            print_line(line_list,return_line(line_list,help)) ;
           }
            fgetpos(fp,&position) ;
            if (if_function(words,&mem_list,position,nesting+1,fp,arg_nums, size_arguments,line_list,check_v,line)==1){
                return 1 ;
            }
        }
        else if (strcmp(words[0],"random")==0){
            if (check_v==1)
            print_line(line_list,return_line(line_list,help)) ;
            if (random_function(words,&mem_list,line_list,line)==1){
                return 1 ;
            }
        }
        else if (strcmp(words[0],"1empty")==0){
            // empty line
        }
        else if (strcmp(words[0],"argument") == 0){
            if (check_v==1)
            print_line(line_list,return_line(line_list,help)) ;
            if (argument_function(words, &mem_list, arg_nums, size_arguments,line_list,line) == 1){
                return 1;
            }
        }
        else {
            printf("%d. Undefined comand\n",return_line(line_list,line)) ;
            return 1 ;
        }
        end = 0;
        if ( fgets(line, sizeof(line), fp) == NULL){
            end = 1;
        }
        strcpy(help,line) ;
        p= strtok(help," \t") ;
        if (p[0]=='#' || p[0]=='\n'){
            do {
                if (fgets(line,sizeof(line),fp)==NULL){
                    end= 1 ;
                }
                strcpy(help,line) ;
                p= strtok(help," \t") ; 
            }while ((p[0] == '#' || p[0]=='\n' ) && end == 0);
        }
        strcpy(help,line) ;
        tabcounter = 0 ;
        while (line[tabcounter] == '\t'){             
            tabcounter++;
        }
        if (tabcounter > nesting){ 
            printf("%d. Invalid depth\n",return_line(line_list,line));
            return 1 ;
        }
        free_words_function(words) ;
    } while ((tabcounter == nesting  ) && (end == 0));   
    return 0;
}