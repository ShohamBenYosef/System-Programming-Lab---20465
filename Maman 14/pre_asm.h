#ifndef PRE_ASM_H_
#define PRE_ASM_H_

#define StartNumOfMacros 4

/* macros node */
typedef struct Macro {
	char l[LINE_LEN];
	struct Macro* next;
} macro_line;


/* macros table */
typedef struct MacroSec {
	macro_line* head;
	char* name;
} macro_list;



/*typedef struct macro_list * list_ptr;*/

macro_list* lists[StartNumOfMacros];
void pre_assembler(); /* main func of file */ 
void get_macro_lines(macro_list* list , FILE* fp);
void generate_macro_lists();
macro_list* create_macro_list();
void add_to_macro_list(list, node);
int check_word(char* new_name);
void print_macro(char* m_word);


#endif
