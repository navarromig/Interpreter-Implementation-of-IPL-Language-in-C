typedef struct memory_list *memory_listptr ;

struct memory_list{       
    char varname[100] ;
    int value ;
    memory_listptr next1 ;
} ;

typedef struct line_store_list *line_listptr ;

struct line_store_list{     
    char line[1000] ;
    int line_num ;
    line_listptr next2 ;
};

void insert(memory_listptr *mem_list , char *name , int data);

int isList(memory_listptr mem_list,char *var);

int returnValue(memory_listptr mem_list, char *var);

int value(char *word, memory_listptr *mem_list, line_listptr line_list, char *line, int *value);

void insert_2(line_listptr *line_list, char *line, int i);

int return_line(line_listptr line_list , char *line) ;

void print_line(line_listptr line_list , int line) ;

void free_list1(memory_listptr mem_list) ;

void free_list2(line_listptr line_list) ;