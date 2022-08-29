#include <stdlib.h>
#include <stdio.h>
#include "lists.h"
#include "error.h"
#include "parser.h"
#include "files.h"


/* the 32base letters. */
char numbers[32]={'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};


/*extract specific bits from integer. */
unsigned int extract(unsigned short value, int begin, int end)
{
    unsigned int mask = (1 << (end - begin)) - 1;
    return (value >> begin) & mask;
}


/*
* check if its WORD or BITS and convert it.
* return the index of the correct char.
*/
int convert_bits(LineData* p, int loc){
	int res = 0;
	int tmp = 0;
	
	if(p->type == BITS){
		tmp = p->mc.bits.ARE; 			/* only 0, 1 */
		tmp += p->mc.bits.source << 2; 	/* only 2, 3 */
		tmp += p->mc.bits.target << 4; 	/* only 4, 5 */
		tmp += p->mc.bits.opcode << 6; 	/* only 6 -9 */
		res = (loc == 0 ? extract(tmp, 0,6) : extract(tmp, 5,11));
	}
	
	else {
		int a = p->mc.word & 10; /* all the 10 bits */
		if(loc == 0){
			res = extract(a, 0,6);
		}
		else
			res = extract(a, 5,11);
	}
	return res;
	
}


/* 
* handle ext/ent output file. 
* check if need to create file before creation.
*/
void print_ext_file(label_list* list, char* end_of_file, int type){
	
	label_node* p;
	FILE* fp = open_file(parser_data.nameOfFile, end_of_file, WirtingFile);
	fprintf(fp, "\tlabel\taddress\n\n");
	
	for (p = list->head; p != NULL; p = p->next) {
		if(p->labelType == EXTERN && type == EXTERN){
			fprintf(fp, "\t%s\t%c%c\n", p->label, numbers[extract(p->address, 4,10)], numbers[extract(p->address, 0,5)]);
		}
		else if(p->labelType == ENTRY  && type == ENTRY){
			fprintf(fp, "\t%s\t%c%c\n", p->label, numbers[extract(p->address, 4,10)], numbers[extract(p->address, 0,5)]);
		}
	}
	fclose(fp);
}


/*
* handle the main output file. 
* run over data-list and convert it to base32 and print it to the file.
*/
void print_data_to_ob_file(LineData_list* list, char* end_of_file)
{
    LineData* p;
	FILE* fp = open_file(parser_data.nameOfFile, end_of_file, WirtingFile);
	unsigned int first, sec;
	
	fprintf(fp, "\tBase32 address Base32 code\n\t%d\t%d\n\n", parser_data.IC, parser_data.DC);
	 
	for (p = list->head; p != NULL; p = p->next) {
		first = convert_bits(p,0);
		sec = convert_bits(p,1);
		fprintf(fp, "\t%c%c\t%c%c\n", numbers[extract(p->address, 5,10)], numbers[extract(p->address, 0,5)], numbers[sec], numbers[first]);
	}
	
	fclose(fp);
}



