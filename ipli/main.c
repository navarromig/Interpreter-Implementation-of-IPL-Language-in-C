#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "modes.h"
#include "lists.h"
#include "words.h"
#include "check.h"

int main(int argc , char *argv[])
{
    FILE *fp ; 
    fpos_t position ;
    line_listptr line_list = NULL  ;
    char line[1000] , help[1000] ;
    int i , check_v = 0 , length , error ;
    if (strcmp(argv[1],"-v")==0){  
        strcpy(help,argv[2]) ;     
        check_v = 1 ;
        fp= fopen(argv[2],"r") ;
        if (fp==NULL){
            printf("Error: File can not be opened\n") ;
            return 1 ;
        }
    }
    else {
        strcpy(help,argv[1]) ;
        fp= fopen(argv[1],"r") ;
        if (fp==NULL){
            printf("Error: File can not be opened\n") ;
            return 1 ;
        }
    }
    length= strlen(help) ;   // help contains the name of the .ipl file
    if (help[length-4]!='.' || help[length-3]!='i'  || help[length-2]!='p' || help[length-1]!='l' ){ 
        printf("Invalid input extension\n") ;
        return 1 ;
    } 
    i = 1;
    while (fgets(line,sizeof(line),fp)!=NULL){   // Every line of the file and it's number will be stored in the line_list
        insert_2(&line_list,line,i) ;
        i++;
    }
    rewind(fp);
    memory_listptr mem_list = NULL ; //list for variables
    char  **words ; 
    int tabcounter , size_arguments, number, *arg_nums , ignore_line = 0 ;
    long curtime ;
    curtime= time(NULL) ;
    srand((unsigned int)curtime) ;
    size_arguments = 0;
    arg_nums = NULL;
    // the "if" bellow is used when command line arguments are used 
    if ( (argc > 2 && check_v==0) || (argc > 3 && check_v==1)){    //  When check_v = 1 the user has used the "-v" argument
        if (check_v==0){
            size_arguments = argc - 2 ;
        }
        else {
            size_arguments = argc - 3 ;
        }
        arg_nums = malloc(sizeof(int) * size_arguments);   // The array arg_nums[size_arguments] will store the arguments the user has used 
        for (i=0; i<size_arguments; i++){
            if (check_v==0){
                if (number_check(argv[i+2]) == 1){ //Checking if the given string is a positive number and returns 1. Otherwise, it returns 0
                    printf("%d. Expected a number\n",return_line(line_list,line));
                    free(arg_nums) ;
                    return 1;
                }
                number = atoi(argv[i+2]);
            }
            else {
                if (number_check(argv[i+3]) == 1){
                    printf("%d. Expected a number\n",return_line(line_list,line));
                    free(arg_nums) ;
                    return 1;
                }
                number = atoi(argv[i+3]);
            }
            arg_nums[i] = number;
        }
    }
    // Now that everything else has been taken care of, the interpeter starts to check every line
    while (fgets(line,sizeof(line),fp)!=NULL){
        words = words_function(line);  // every line is broken down to key words and every word is stored as a string in the array words
        if (words == NULL){
            printf("Memory error!\n") ;
            if ( (argc > 2 && check_v==0) || (argc > 3 && check_v==1)){
                free(arg_nums) ;
            }
            free_words_function(words);
            return 1 ;
        }
        if (strcmp(words[0],"1empty")==0){  // the line has nothing useful for the interpreter
            ignore_line= 1 ;
        }
        tabcounter = 0 ;
        while (line[tabcounter] == '\t' && ignore_line==0  ){
            tabcounter++ ;
        }
        if (tabcounter != 0){
            printf("%d. Expected another level\n",return_line(line_list,line));
            if ( (argc > 2 && check_v==0) || (argc > 3 && check_v==1)){
                free(arg_nums) ;
            }
            free_words_function(words);
            return 1;
        }
        error = 0 ;
        if (strcmp(words[0],"read")==0){ 
            if (check_v==1){
                print_line(line_list,return_line(line_list,line)) ;
            }
            error = read(words,&mem_list,line_list,line) ;
        }                                                                       
        else if (strcmp(words[0],"write")==0){
            if (check_v==1){
                print_line(line_list,return_line(line_list,line)) ;
            }
            error = write(words,&mem_list,line_list,line) ;
            printf(" ") ;
        }
        else if (strcmp(words[0],"writeln")==0){
            if (check_v==1){
                print_line(line_list,return_line(line_list,line)) ;
            }
            error = write(words,&mem_list,line_list,line) ;
            printf("\n") ;
        }                                                                  
        else if (strcmp(words[1],"=")==0){
            if (check_v==1){
                print_line(line_list,return_line(line_list,line)) ;
            }
            error = computations(words,&mem_list,line_list,line) ;
        }
        else if (strcmp(words[0],"while")==0){
            if (check_v==1){
                print_line(line_list,return_line(line_list,line)) ;
            }
            fgetpos(fp,&position) ;
            error = while_function(words,&mem_list,position,1,fp, arg_nums, size_arguments,line_list,check_v,line) ;
        }
        else if (strcmp(words[0],"if")==0){
            if (check_v==1){
                print_line(line_list,return_line(line_list,line)) ;
            }
            fgetpos(fp,&position) ;
            error = if_function(words,&mem_list,position,1,fp, arg_nums, size_arguments,line_list,check_v,line) ;
        }
        else if (strcmp(words[0],"random")==0){
            if (check_v==1){
                print_line(line_list,return_line(line_list,line)) ;
            }
            error = random_function(words,&mem_list,line_list,line) ;
        }
        else if (strcmp(words[0],"1empty")==0){
            // empty line
        }
        else if (strcmp(words[0],"argument") == 0){
            if (check_v==1){
                print_line(line_list,return_line(line_list,line)) ;
            }
            error = argument_function(words,&mem_list, arg_nums, size_arguments,line_list,line) ;
        }
        else {
            printf("%d. Undefined command\n",return_line(line_list,line)) ;
            error = 1 ;
        }
        if (error){
            if ( (argc > 2 && check_v==0) || (argc > 3 && check_v==1)){
                free(arg_nums) ;
            }
            free_words_function(words); 
            return 1 ;
        }
        free_words_function(words);
    }
    fclose(fp) ;
    if ( (argc > 2 && check_v==0) || (argc > 3 && check_v==1)){
        free(arg_nums) ;
    }
    free_list1(mem_list) ;
    free_list2(line_list) ;
    return 0 ;
}