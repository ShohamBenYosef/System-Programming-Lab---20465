#include <stdlib.h>
#include <stdio.h>
#include "lists.h"
#include "error.h"
#include "parser.h"
#include "files.h"


char numbers[32]={'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};

int getNum(LineData* p){
	int res = 0;
	if(p->type == BITS){
		res+=p->mc.bits.ARE;
		res+=(p->mc.bits.target)*4;/*2^2*/
		res+=(p->mc.bits.source)*16; /*2^4*/
		res+=(p->mc.bits.opcode)*64; /*2^6*/
	}
	else{
		res = p->mc.word;
	}
	printf("%d**\n", res);
	return res;
}



int split_to_five(int* number, int k, int p){
	int res = 0;
	printf("num=%d\n",*number);
	res =((*number)>>5);
	return res;
}

unsigned int extract(unsigned short value, int begin, int end)
{
    unsigned int mask = (1 << (end - begin)) - 1;
    return (value >> begin) & mask;
}


/**/
void print_ext_file(label_list* list, char* end_of_file, int type){
	
	label_node* p;
	FILE* fp = open_file(parser_data.nameOfFile, end_of_file, WirtingFile);
	fprintf(fp, "\tlabel\taddress\n\n");
	
	for (p = list->head; p != NULL; p = p->next) {
		if(p->labelType == EXTERN && type == EXTERN){
			fprintf(fp, "\t%s\t%c%c\n", p->label, numbers[extract(p->address, 10,4)], numbers[extract(p->address, 0,5)]);
		}
		else if(p->labelType == ENTRY  && type == ENTRY){
			fprintf(fp, "\t%s\t%c%c\n", p->label, numbers[extract(p->address, 10,4)], numbers[extract(p->address, 0,5)]);
		}
	}
	fclose(fp);
}

int convert_bits(LineData* p, int loc){
	int res = 0, tmp = 0;
	if(p->type == BITS){
		tmp = p->mc.bits.ARE;
		tmp += p->mc.bits.source << 2;
		tmp += p->mc.bits.target << 4;
		tmp += p->mc.bits.opcode << 6;
		res = (loc == 0 ? extract(tmp, 0,6) : extract(tmp, 5,11));
	}
	
	else {
		res = (loc == 0 ? extract(p->mc.word, 0,6) : extract(p->mc.word, 5,11));
	}
	return res;
	
}

void print_data_to_file(LineData_list* list, char* end_of_file)
{
    LineData* p;
	FILE* fp = open_file(parser_data.nameOfFile, end_of_file, WirtingFile);
	int first, sec;
	
	fprintf(fp, "\t%d\%d\n\n", parser_data.IC-100, parser_data.DC);
	
	for (p = list->head; p != NULL; p = p->next) {
		first = convert_bits(p,0);
		sec = first = convert_bits(p,1);
		fprintf(fp, "\t%c%c\t%c%c\n", numbers[extract(p->address, 10,4)], numbers[extract(p->address, 0,5)], numbers[sec], numbers[first]);
	}
	fclose(fp);
}


int convert(int num){
	int first_digit, second_digit;
   		if(num<32){

      		first_digit=numbers[0];
       		second_digit=numbers[num];
       	}
    	else if(num>=32){
        	first_digit=numbers[num/32];

     		second_digit=numbers[num%32];
    	}
}

int binToDec(int binary_num){
	int decimal_num = 0, base = 1, rem;
	printf("12\n");
	while ( binary_num > 0)  
    {  
        rem = binary_num % 10; /* divide the binary number by 10 and store the remainder in rem variable. */  
        decimal_num = decimal_num + rem * base;  
	printf("13\n");
       	binary_num = binary_num / 10; /* divide the number with quotient*/  
        base = base * 2;  
    }  
	printf("11\n");
    return decimal_num;
}












/*
void print_data_to_file(LineData_list* list, char* end_of_file)
{
    LineData* p = list->head;
    FILE* fp = open_file(parser_data.nameOfFile, end_of_file,WirtingFile);
    char binary_word[11] = {0};
    
    int tmp;
    fprintf(fp,"\tIC, DC\t\n");
    while(p){
    	tmp = getNum(&p);
    	memset(binary_word, '\0', sizeof(binary_word));
    	
    	
    	
    	printf("%d", split_to_five(&(p->address),5,2));
    	printf("11\n");
    	
    	
  		printf("%c%c\t\t%c%c\n",numbers[split_to_five(p->address,5,2)], numbers[split_to_five(p->address,5,1)], numbers[split_to_five(tmp,5,2)], numbers[split_to_five(tmp,5,1)]);
        printf("11\n");
        p = p->next;
    };
    printf("12\n");
    fclose(fp);
}*/
