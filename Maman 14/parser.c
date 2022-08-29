#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "error.h"
#include "parser.h"
#include "files.h"
#include "lists.h"

struct {
	char* name;
	int numOfOperands;
	unsigned int opcode:4;
	
} cmds[] = {
	{"mov", 2, 0},
	{"cmp", 2, 1},
	{"add", 2, 2},
	{"sub", 2, 3},
	{"not", 1, 4},
	{"clr", 1, 5},
	{"lea", 2, 6},
	{"inc", 1, 7},
	{"dec", 1, 8},
	{"jmp", 1, 9},
	{"bne", 1, 10},
	{"get", 1, 11},
	{"prn", 1, 12},
	{"jsr", 1, 13},
	{"rts", 0, 14},
	{"hlt", 0, 15},

};

char registers[8][2] = {"r0","r1","r2","r3","r4","r5","r6","r7"};




/* move pointer to the beggining of the next word (not blanc char) */
void forward(char* line ,int *ptr_curr) {
	int curr = *ptr_curr;
    while( isspace(line[curr]) && line[curr] != '\n')
		   curr++;
	*ptr_curr = curr;
}

/* return 1 if theres a comma in the next blanc chars. */
int check_comma(char* line, int *ptr_curr) {
    int curr = *ptr_curr, comma = 0;;
    forward(line, &curr);
    
    if(line[curr] == ',')
	    comma = 1;
    return comma;
}

/* move pointer over comma (usaliy came after 'check_comma' function */
void jump_comma(char* line, int *ptr_curr) {
    	int curr = *ptr_curr;
    	forward(line, &curr);
	if(line[curr] == ',')
		curr++;
	
	forward(line, &curr);
	*ptr_curr = curr;
}

/* extract the whole next word from the line. */
void get_next_word(char* line, char* word, int* ptr_curr) {
	int curr = *ptr_curr, j;	
    	forward(line, &curr);

	for(j = 0; !isspace(line[curr]) && line[curr] != '\n' && line[curr] != ','; curr++)
		word[j++] = line[curr];
	word[j] = '\0'; /* set end to null */

	forward(line, &curr);
	*ptr_curr = curr; 
}

/************************************************************************************/

/* sec pas of asm algorithm, fill the data list with correct addresses of the labels and handle ext and ent. */
void sec_pas(char* line, int l_cnt)
{
    int curr = 0, tmp;
	char* first_word = (char*)malloc(sizeof(char)* LINE_LEN);  /* hold the first word in line */
	char* sec_word = (char*)malloc(sizeof(char)* LINE_LEN);  /* hold the second word in line */
	if(!first_word || !sec_word)
		fatal_error(ErrorMemoryAlloc);
	
	if (line[curr] != '\n' && line[curr] != ";") { /* not blanc line */
	    get_next_word(line, first_word, &curr);
	    if(first_word[0] == '.') /* if line start with dot ('.') */
	    {
	        if(strcmp(first_word, ".extern") == 0) {
	            printf("");
	        }
	        else if(strcmp(first_word, ".entry") == 0) {
	            printf("");
	        }
	    }
	    else if (first_word[strlen(first_word) - 1] == ':') /*line start with label */ 
	    {
	        get_next_word(line, sec_word, &curr);
	        /*call to func that handle labels in line and add it to data list.*/
	        if (sec_word[0] != '.')
	        	find_labels(line, sec_word, &curr);
	    }
	    else
        	find_labels(line, first_word, &curr); /*search address of label*/
	}
	free(sec_word);
	free(first_word);
	
}

/* search address of label and set it in data node. */
find_labels(char* line, char* word, int * ptr)
{
	printf("in find labels\n");
    int curr = *ptr, i, val_of_label;
    char* next_word = (char*)malloc(sizeof(char)* LINE_LEN);
	if(!next_word)
		fatal_error(ErrorMemoryAlloc);

	/*check if its valid command */
	for(i = 0; i < 16; i++)
            if (strcmp(word ,cmds[i].name) == 0)
                break;
    if (i == 16)
        fatal_error(InvalidCommand);
    
    if(cmds[i].numOfOperands > 0){
        get_next_word(line, word, &curr);
        val_of_label = check_label(parser_data.Shead, word);
        
        if(val_of_label){
        	printf("if val\n");
            refill_data_node(parser_data.Dhead, val_of_label);
        }
        /* if command have more operands - dont check errors because its sec pas.*/
        if(cmds[i].numOfOperands > 1)
        	printf("*1*\n");
        	jump_comma(line, &curr);
            get_next_word(line, next_word, &curr);
            val_of_label = check_label(parser_data.Shead, next_word);
            if(val_of_label)
                refill_data_node(parser_data.Dhead, val_of_label);
    }
    
	free(next_word);
}
/*************************************************************************/

void parse_line( char* line)
{
	int label_flag = 0, curr = 0, tmp;
	char* first_word = (char*)malloc(sizeof(char)* LINE_LEN);  /* hold the first word in line */
	char* sec_word = (char*)malloc(sizeof(char)* LINE_LEN);  /* hold the second word in line */
	if(!first_word || !sec_word)
		fatal_error(ErrorMemoryAlloc);
	puts(line); /*Debag*/
	
	if (line[curr] != '\n' && line[curr] != ";") /* check not comment line */
	{
	    parser_data.line_num++;
		get_next_word(line, first_word, &curr); /* read the first word in the line */
		if (first_word[strlen(first_word)-1] == ':') /* check if lebel */
		{
		    label_flag = 1;
		    tmp = curr;
		    get_next_word(line, sec_word, &tmp); /*check if need tmp or curr.*/ 
		    /* label of data (DC)*/
			if (sec_word[0] == '.' && (!strcmp(sec_word, ".struct") || !strcmp(sec_word, ".data") || !strcmp(sec_word, ".string")))
			{ 
			    add_label_to_list(parser_data.Shead, first_word, parser_data.IC, DATA);

                parse_data(line, sec_word, &curr);
			    /*parser_data.IC+=parse_data.DC;*/             
			} 
			else{ /* label of INSTRUCTION (IC)*/
			    add_label_to_list(parser_data.Shead, first_word, parser_data.IC++, INSTRUCTION);
		        	handle_instruction(line, sec_word, &tmp);
			}
		}
		else if(first_word[0] == '.'){ /* extern or entry line */
    			if (strcmp(first_word, ".extern") == 0) 
    			{
    				printf("extern\n");
    				parse_extern(line, &curr);
    			}
    			else if (strcmp(first_word, ".entry") == 0) 
    			{
    				printf("entry\n");
    				/*parse_entry(line, &curr);*/	
    			}
		}
		else
		    handle_instruction(line, first_word, &curr);
		   
		/*parser_data.IC += parser_data.IC;  לא כאן - להעביר לסוף קובץ*/    
		label_flag = 0;
		free(first_word);
	} /* End of Empty or Comment Line */
}

/*************Extern**and**Entry***********************/
/* add extern label to label list */
parse_extern(char* line, int* ptr){
    int curr = *ptr;
    char* s = (char*)malloc(sizeof(char)*32);
    while (s){
        get_next_word(line, s, &curr);
        add_label_to_list(parser_data.Shead,s, ++parser_data.ext_flag, EXTERN);
        /* every label should saperate with comma */
        if(check_comma(line, &curr)){
            jump_comma(line, &curr);
        }
        else
            break;
    }
    *ptr = curr;
    free(s);
}

/***************Data**String**Or**Struct*********************/
void parse_data(char* line, char* sec, int *ptr)
{
	int curr = *ptr;
    char* str = (char*) malloc (sizeof(char)*32);
    if (str)
    {
        get_next_word(line, str, &curr);
        if (!strcmp(sec, ".struct")){
            handle_struct(line, str, &curr);
        }
        else if (!strcmp(sec, ".string")){
            handle_string(line, str, &curr);
        }
        else {
            handle_data(line, str, &curr);
        }
        free(str);
    }
    else
    	fatal_error(ErrorMemoryAlloc);
    	
}

void handle_struct(char* line, char* str,  int *ptr)
{
	int curr = *ptr;
	if(str){
	    while(1)
	    {
		if(str[0] == "\"")
		    handle_string(line, &str, &curr);
		else
		    handle_data(line, &str, &curr);

		if(check_comma(line, &curr)){
		    jump_comma(line, &curr);
			get_next_word(line, &str, &curr);
		    }
		else
			break;
	    }
	}
	else
		fatal_error(ErrorMemoryAlloc);
}

void handle_data(char* line, char* str,  int *ptr)
{
	int curr = *ptr, n;
	while (1) 
	{
		MachineCodeWord _mc;
		_mc.word = get_next_num(line, &curr);
		add_data_to_list(parser_data.Dhead, ++(parser_data.IC), _mc, 1, 0);
		parser_data.DC++;
		if(!check_comma(line, &curr))
			break;
		else
			jump_comma(line, &curr);
	}
	
}

/*extract next number from line. */
int get_next_num(char* line, int* ptr_curr){
    	int curr = *ptr_curr, j;
        char num[LINE_LEN];
    	for(j = 0; !isspace(line[curr]) && line[curr] != '\n'  && ( isalnum(line[curr]) || line[curr] == '+' || line[curr] == '-' ); curr++)
			{
			    num[j] = line[curr];
			    j++;
			}
		num[j] = '\0'; /* set null in the end */
    	forward(line, &curr);
    	*ptr_curr = curr;
        return atoi(num);
}
/* extract string from line and add every char to data list as WORD. */
void handle_string(char* line, char* str, int *ptr)
{
    int i = 0;
    while(str[i])
    {
    	/* run on every char */
        if(isalnum(str[i])){
            parser_data.DC++;
            MachineCodeWord _mc;
            _mc.word = str[i];
            add_data_to_list(parser_data.Dhead, ++(parser_data.IC), _mc, WORD, 0);
        }   
        i++;
    }
}

/******************************************************/


handle_instruction(char* line,char* command, int* ptr)
{
    int curr = *ptr;
    int command_num = parse_command(line, command ,&curr);
    MachineCodeWord m;
    /* func that build line in data list of what in operand, its diferend from parse operand! */
    if(cmds[command_num].numOfOperands == 1)
    {
        operand_handle(line, &curr, 1);
    }
    else if(cmds[command_num].numOfOperands == 2)
    {
        operand_handle(line, &curr, 1);
        if(check_comma(line, &curr)){
            jump_comma(line, &curr);
            operand_handle(line, &curr, 2);
        }
        /*else
            fatal_error("Missing Comma");*/
    }
    *ptr = curr;
}

operand_handle(char* line, int* ptr, int num_of_op)
{
    int curr = *ptr, i;
    MachineCodeWord mc;
    char* op = (char*)malloc(sizeof(char)*32);
    if (op)
    {
        get_next_word(line, op, &curr);
        parser_data.IC++;
        if(is_register(op)){
            for(i = 0; i < 8; i++){
                if(!strcmp(op, registers[i]))
                    break;
            }
            if (num_of_op == 2)
                mc.word = (i<<2); /* move bits 2 digits to the left and add 0's */
            else
                mc.word = (i<<5);
            add_data_to_list(parser_data.Dhead, parser_data.IC, mc, 0, 0);
        }
        else if(is_imidiate(op)){
            for (i =0; op[i+1]; i++)
                op[i] = op[i+1];
            op[i] = '\0';
            
            mc.word = (atoi(op) * 4); /* move bits 2 digits to the left and add 0's */
            add_data_to_list(parser_data.Dhead, parser_data.IC, mc, 1, 0);
        }
        else{
            add_data_to_list(parser_data.Dhead, parser_data.IC, mc, 1, 1);
        }
    } else
        fatal_error(ErrorMemoryAlloc);
    *ptr = curr;
}

int parse_command(char* line,char* command, int* ptr)
{
    int curr = *ptr, command_num = 0, tmp, shouldReturn = 0;
    MachineCodeWord _mc;
    if(command)
    {
        for(command_num = 0; command_num < 16; command_num++)
            if (strcmp(command ,cmds[command_num].name) == 0)
                break;

        if (command_num == 16)
            fatal_error(InvalidCommand);
            
        else
        {
            _mc.bits.opcode = command_num;
            tmp = curr;
            if(check_num_of_operands(line, command_num, &tmp))
            {
                tmp = curr;
                if(cmds[command_num].numOfOperands == 0){
                    if (cmds[command_num].numOfOperands < 2)
                        _mc.bits.source = 0;
                    else{
                        _mc.bits.source = parse_operand(line, command_num, &tmp, 1);
                    }
                    _mc.bits.target = parse_operand(line, command_num, &tmp, 2); /* only check if register or label or etc*/
                }else{
                    _mc.bits.target = 0;
                    _mc.bits.source = 0;
                }
                _mc.bits.ARE = 0;
            }
            parser_data.IC++;
            /*if (_mc.bits.target)*/
            	add_data_to_list(parser_data.Dhead, parser_data.IC, _mc, 0, shouldReturn);
            *ptr = curr;
            return command_num;
        }
    }
    else
        fatal_error(InvalidCommand);
}

/*TODO - check with yair. */
int parse_direct(){
    
}

int check_num_of_operands(char* line, int command_num, int* ptr)
{
    int count = 0, excepted = cmds[command_num].numOfOperands, curr = *ptr , i;
    char* s = line;
    for(i = 0 ; s[i] ; i++)
    {
    	if(s[i] == 44){
    	    count++;
    	}
 	}
 	if(i>0)
 	    count++;
 	return !(count-(cmds[command_num].numOfOperands));
}


parse_operand(char* line, int commandIndex, int* ptr, int opNumber)
{   
    int curr = *ptr;
    parser_data.IC++;
    if (cmds[commandIndex].numOfOperands < 2 && opNumber == 1)
        return 0;
    
    else {
        
        char* op = (char*) malloc (sizeof(char)*32);
        if (!op)
            fatal_error("Memory");
            
        get_next_word(line, op, &curr);
        
        
        if (is_imidiate(op))
            return 0;
        else if (is_direct(op))
            return 1;
        else if (is_register(op))
            return 3;
        else/* is_labelA was delete because i want to do it in the second round of the algorithm. */
            return 2;
        
    }
}



/********Directory**************/
int is_imidiate (char* op){
    if (op[0] == '#')
        return 1;
    return 0;
}

int is_direct(char* op){
    printf("\n");
}

int is_labelA(char* op){
    char* pPosition = strchr(op, '.');
    parser_data.IC++;
    printf("\n");
}

int is_register(char* op){
    return strlen(op) == 2 && op[0] == 'r' && op[1] >= '0' && op[1] <= '7';
}


/***********Start**Func**********/

/*set parser_data struct */
void setParserData(){
	parser_data.DC = 0;
    	parser_data.IC = 100;
    	parser_data.line_num = 0;
    	parser_data.file = NULL;
    	parser_data.err_count = 0;
    	parser_data.Shead = create_label_list();
    	parser_data.Dhead = create_data_list();
}
/* free parser_data at the end. */
void freeParserData(){
	parser_data.file = NULL;
	deleteList(parser_data.Shead); /*free label list*/
	deleteListData(parser_data.Dhead);/*free data list*/
	printf("123");
	free(parser_data.nameOfFile);
}

	

