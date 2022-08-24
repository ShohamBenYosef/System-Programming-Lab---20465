#include <stdlib.h>
#include <stdio.h>
#include "lists.h"
#include "error.h"
#include "parser.h"

label_list* create_label_list() {
    label_list* list = (label_list*)malloc(sizeof(label_list));
    list->head = NULL;
    return list;
}

LineData_list* create_data_list() {
    LineData_list* list = (LineData_list*)malloc(sizeof(LineData_list));
    list->head = NULL;
    return list;
}

void add_data_to_list(LineData_list* list, int _address, MachineCodeWord _machine, int _type, int _forSec)
{
	LineData* node = (LineData*)malloc(sizeof(LineData));
	if (node != NULL)
	{
		node->address = _address;
		node->mc = _machine;
		node->type = _type;
		node->next = NULL;
		node->forSec = _forSec;

		if(list->head == NULL)
        		list->head = node;
        	else {
        		LineData* curr = list->head;
		
			while(1)
			{
				if(curr->next == NULL)
				{
					curr->next = node;
					break;
				}
				curr = curr->next;
			} /* end of while loop */
        	}
	}
	else
		fatal_error(ErrorMemoryAlloc);
}

void add_label_to_list(label_list* list, char* label, int _address, int _type)
{
    label_node* curr, *new_label = (label_node*)malloc(sizeof(label_node));
    if(!new_label)
		fatal_error(ErrorMemoryAlloc);
	
	strcpy(new_label->label, label);
	new_label->address = _address;
	new_label->labelType = _type;
	new_label->next = NULL;
	
	if(list->head == NULL)
        	list->head = new_label;

    	else {
    		curr = list->head;
    		
    		while(1){
    			
	    		if(!strcmp(new_label->label, curr->label)){
	    			puts(new_label->label);
	    				if(new_label->labelType == EXTERN)
	    					fatal_error("Label Already Exist ");
	    		}
	    			
	    		else if(curr->next == NULL){
	    			curr->next = new_label;
	    			break;
	    		}
	    		curr = curr->next;
		};
	}
}


void print_label(label_node* node)
{
    printf(" [ %s , %u , %d ] ", node->label, node->address, node->labelType);
}

void print_data_node(LineData* node)
{
	if(node->type == BITS)
    		printf(" { %d , %d ,%d ,%d ,%d } ", node->address, node->mc.bits.opcode, node->mc.bits.source, node->mc.bits.target, node->mc.bits.ARE);
    	else
    		printf(" { %d , %d } ", node->address, node->mc.word);
}

void print_debbug_data(LineData_list* list)
{
	LineData* p;
	for (p = list->head; p != NULL; p = p->next)
	{
		print_data_node(p);
		printf("\n");
	}
	printf("\n");
}


void print_label_table(label_list* list){
	
	label_node* p;
	for (p = list->head; p != NULL; p = p->next) {
		print_label(p);
		printf("\n");
	}
	printf("\n");
}


void deleteList(label_list* list)
{
	label_node* p = list->head;

	while (list->head != NULL) {
		list->head = list->head->next;
		printf("delete %s\n", p->label);
		free(p);
		p = list->head;
	}

}

void deleteListData(LineData_list* list)
{
	LineData* p = list->head;
	while (list->head != NULL) {
		list->head = list->head->next;
		free(p);
		p = list->head;
	}	
}
/****************************************************/

/*********************************************************/

/* fill the next label in data-list with his address. */
void refill_data_node(LineData_list* list, int val)
{
    LineData* p = list->head;
    
    while(p){
        if(p->forSec == 1){
            MachineCodeWord _mc;
            _mc.word = val;
            p->mc = _mc;
            p->forSec = 0;
        }
        p = p->next;
    };
}
/* search if word is label  - move to list.c */
check_label(label_list* list, char* word)
{
    label_node* p = list->head;
    /*
    TODO - add check '.' in word and handle it. 
    */
    while(p){
        if(!strcmp(word, p->label)){
            puts(word);
            return p->address;
        }
        p = p->next;
    };
    return 0;
}
/******************************************************/
