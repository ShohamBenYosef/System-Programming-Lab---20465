#ifndef ITEM_LIST_HANDLING_H
#define ITEM_LIST_HANDLING_H


#define MAX_LEN_OF_LABEL 81
#define LINE_LEN 81

typedef enum typeOfLabel {DATA, INSTRUCTION , STRUCT, STRING , EXTERN , ENTRY }; /* type of assembly line*/

typedef enum typeOfData {BITS, WORD}; 

typedef struct LabelNode {
	char label[MAX_LEN_OF_LABEL]; 	/* the name (in String) of the symbol */
	unsigned int address; 		/* address of the symbol */
	int labelType;			/* 1 for instruction, 0 for data */
	char data_label[81]; 		/*hold the nums or letters.*/
	struct LabelNode* next;
} label_node;


typedef struct List {
	label_node* head;
} label_list;




typedef struct {
	unsigned int opcode :4;
	unsigned int target :2;
	unsigned int source :2;
	unsigned int ARE :2;
} MachineCodeBits;

typedef union {
	MachineCodeBits bits;	/* for instruction */
	unsigned int word :10;		/* otherwise */
} MachineCodeWord;


typedef struct DataNode {
	int address;
	MachineCodeWord mc;
	int type;
	int forSec;
	struct DataNode* next;
} LineData;

typedef struct DataList {
	LineData* head;
} LineData_list;


/*
* create symbol table and return the head of the list.
*/
label_list* create_list();
LineData_list* create_data_list();
/*
* build new label node and fill it with data, 
*/
void add_label_to_list(label_list* list, char* label, int _address, int _type);
void add_data_to_list(LineData_list* list, int _address, MachineCodeWord _machine, int _type, int _forSec);
/*
* print the node in format.
*/
void print_label(label_node* node);
void print_data_node(LineData* node);
/*
* print the symbol list. (calling to print_label method for each node)
*/
void print_label_table(label_list* list);
void print_debbug_data(LineData_list* list);
/*
* delete the table and free the memory.
*/
void deleteList(label_list* list);
void deleteListData(LineData_list* list);


void refill_data_node(LineData_list* list, int val);
int check_label(label_list* list, char* word);
#endif 
