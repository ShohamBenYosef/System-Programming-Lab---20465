#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "files.h"
#include "parser.h"
#include "error.h"
#include "pre_asm.h"


/* create single macro list */
macro_list* create_macro_list(){
	 macro_list* list = (macro_list*) malloc (sizeof(macro_list));
	 
	 if (list) {
	 	list->head = NULL;
	 	list->name = (char*) malloc (sizeof(char)*81);
	 	if(list->name)
	 		return list;
	 	free(list->name);
	 } else
	 	fatal_error(ErrorMemoryAlloc);
}
/* free single macro */
void free_macro(macro_list* m){
	free(m->name);
}
/* free all the macros in the list */
void free_macro_lists(){
	int i = 0;
	for ( i = 0; i < StartNumOfMacros; i++){
		free_macro(lists[i]);
	}
}


void generate_macro_lists()
{
	int i;
	for(i = 0; i < StartNumOfMacros; i++){
		lists[i] = create_macro_list();
	}
}


void add_to_macro_list(macro_list* list, macro_line* node) /*TODO conect 2 func of add_to_list from lists handling */
{
	if(list->head == NULL)
        	list->head = node;
        
        else 
        {
		macro_line* p = list->head;
		
		while(1){
			
			if(p->next == NULL){
				p->next = node;
				break;
			}
			
			p = p->next;
		};
	}

}

void print_macro(char* m_word){
	int i = 0;
	for (i=0; i < StartNumOfMacros; i++) {
		if (strcmp(m_word, lists[i]->name) == 0) {

			macro_line* p = lists[i]->head;
			while (p != NULL) {

				fputs(p->l, parser_data.file);
				p = p->next;
			}
		}
		else
			i += 1;
	}
	
}

int check_word(char* new_name){
	int i;
	for (i = 0; i < StartNumOfMacros; i++) {

		if (strcmp(new_name, lists[i]->name) == 0) {
			return 1;
		}
		i += 1;
	}
	return 0;
}


void get_macro_lines(macro_list* list , FILE* fp)
{
	char line[LINE_LEN];
	int curr;
	char* first_word = (char*)malloc(sizeof(char)* LINE_LEN);
	if (first_word) {

		while (fgets(line, LINE_LEN, fp)) {
			curr = 0;
			get_next_word(line, first_word, &curr);
			if (strcmp(first_word, "endmacro")) /* not endmacro */
			{
				/*printf("%s\n", line);*/
				macro_line* node = (macro_line*)malloc(sizeof(macro_line));
				if(node) {
					strcpy(node->l, line);
					add_to_macro_list(list, node);
				}
			}
			else {
				free(first_word);
				break;
			}
		}
		
	}
	else {
		free(first_word);
		fatal_error(ErrorMemoryAlloc);
	}
	
} 



void pre_assembler()
{
	int curr, m_index = -1, i;
	char line[LINE_LEN];
	char* first_word, *name;
	
	first_word = (char*)malloc(sizeof(char)* LINE_LEN);
	name = (char*)malloc(sizeof(char)* LINE_LEN);
	generate_macro_lists();
	
	/* check allocation */
	if (first_word && name) {
		
		/* open original file. */
		FILE* fp = open_file(parser_data.nameOfFile, MainFileEnding ,ReadFile);
		/* open new file to write in. */
		parser_data.file = open_file(parser_data.nameOfFile, AfterMacroEnding ,WirtingFile);
		
		if (fp && parser_data.file) {
			/* read line by line */
			while (fgets(line, LINE_LEN, fp)) {
				/*printf("1\n%s\n", line);*/
				curr = 0;
				get_next_word(line, first_word, &curr);
				/* check if the first name is "macro" */
				if(strcmp("macro", first_word) == 0)
				{
					get_next_word(line, name, &curr);
					strcpy(lists[++m_index]->name, name);
					get_macro_lines(lists[m_index], fp); /* add macro lines to list */
				}
				/* check if its macro name, if it is so print it to file. */
				else if (check_word(first_word)) {
					print_macro(first_word);/*print any line of the macro */
				}
				else
					fputs(line, parser_data.file);
					
			}
			/* Return to the beginning of the file */
			fseek(fp, 0, SEEK_SET);
		}
		else
			fatal_error(ErrorCantRead);
		fclose(fp);
	}
	free(first_word);
	free(name);
	fclose(parser_data.file);
}

