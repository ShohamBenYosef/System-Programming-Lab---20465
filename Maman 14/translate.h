#ifndef TRANSLATE_H
#define TRANSLATE_H


void print_data_to_ob_file(LineData_list* list, char* end_of_file);
void print_ext_file(label_list* list, char* end_of_file, int type);
int convert_bits(LineData* p, int loc);
unsigned int extract(unsigned short value, int begin, int end);
void con(char* dst, int n, int  num);


#endif 
