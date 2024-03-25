typedef struct memory_list *memory_listptr ;

typedef struct line_store_list *line_listptr ;

int read(char **words,memory_listptr *mem_list , line_listptr line_list , char *line);

int write(char **words, memory_listptr *mem_list, line_listptr line_list , char *line);

int computations(char **words , memory_listptr *mem_list, line_listptr line_list , char *line);

int while_function(char **words, memory_listptr *mem_list , fpos_t line_position , int nesting , FILE *fp, int *arg_nums, int size_arguments , line_listptr line_list , int check_v, char *line) ;

int if_function(char **words, memory_listptr *mem_list, fpos_t line_position , int nesting , FILE *fp,int *arg_nums, int size_arguments , line_listptr line_list , int check_v, char *line) ;

int random_function(char **words , memory_listptr *mem_list, line_listptr line_list , char *line);

int argument_function(char **words, memory_listptr *mem_list,int *arg_nums, int size_arguments, line_listptr line_list , char *line);
