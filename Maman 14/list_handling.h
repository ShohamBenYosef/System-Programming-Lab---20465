#ifndef ITEM_LIST_HANDLING_H
#define ITEM_LIST_HANDLING_H

#define MAX_LEN_OF_LABEL 81
#define LINE_LEN 81



typedef struct Node {
	char label[MAX_LEN_OF_LABEL]; /* the name (in String) of the symbol */
	int ic; /* address of the symbol */
	int instructionOrData; /* 1 for instruction, 0 for data */
	int ext; /*flag to extern labels */
	char data_label[81]; /*hold the nums or letters.*/
	struct Node* next;
} label_node;


typedef struct List {
	label_node* head;
} label_list;




/*
* create symbol table and return the head of the list.
*/
label_list* create_list();

/*
* build new label node and fill it with data, 
*/
void add_label_to_list(label_list* list, char label[], int ic, int 
instructionOrData, int ext, char *);

/*
* print the node in format.
*/
void print_label(label_node* node);

/*
* print the symbol list. (calling to print_label method for each node)
*/
void print_label_table(label_list* list);

/*
* delete the table and free the memory.
*/
void deleteList(label_list* list);


#endif 
