#ifndef TRANSLATE_H
#define TRANSLATE_H








int convert(int num);
int binToDec(unsigned int binary_num);
int getNum(LineData* p);
int split_to_five(int number, int k, int p);
/*****/
void print_data_to_file(LineData_list* list, char* end_of_file);


void print_ext_file(label_list* list, char* end_of_file, int type);












#endif 
