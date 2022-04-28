
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEN 512
#define NUM_OF_MATS 6
#define SIZE 4

enum { COMMA_NOT_EXPACTED ,COMMA_EXPACTED};


typedef struct matrix {
    double** m;
} matrix;


typedef struct matrix * mat_ptr;


mat_ptr mats[NUM_OF_MATS];




/*****************************************************
        *      matrix calculator - methods      *
*****************************************************/

void read_mat(char *line , int* ptr_curr){
    
    int curr = *ptr_curr, i = 0, j, k = 0, mat_index;
    char num[MAX_LEN];
    double arr[SIZE*SIZE];
    
    mat_index = get_mat(line, &curr, COMMA_NOT_EXPACTED);
    
    if(mat_index >= 0){
        
        while(i < 16)
        {
            if(check_comma(line, &curr)){
                jump_comma(line, &curr);
                
                if(!check_comma(line, &curr)){
                    get_number(line, &num ,&curr);
                    arr[i] = atof(num);
                    i += 1;
                }
                else{
                    printf("Too Many Commas."); /***Error Log**/
                    return;
                }
                
            }
            else 
                break;
        }

        for(i = 0; i < SIZE; i++)
            for ( j = 0 ; j < SIZE; j++)
                mats[mat_index]->m[i][j] = arr[k++];
                
        *ptr_curr = curr;
    }
    else{
        printf("123");
        return;
    }
}


void print_mat(char* line, int* ptr_curr){
    int curr = *ptr_curr, mat_index, i, j;
    char name[MAX_LEN];
    
    mat_index = get_mat(line, &curr, COMMA_NOT_EXPACTED);
    if(mat_index >= 0){
        
        printf("\n");
        for(i=0; i < SIZE; i=i+1){
		    for(j=0; j < SIZE; j= j+1){
			    printf("%20.4f", *(*((mats[mat_index]->m)+i)+j));
		    }
		    printf("\n");
	    }
        *ptr_curr = curr;
    }
    
}


void add_mat(char* line , int* ptr_curr){
    int curr = *ptr_curr, i, j, mat1, mat2, mat3;
    char name[MAX_LEN];
    
    mat1 = get_mat(line, &curr, COMMA_EXPACTED);
    mat2 = get_mat(line, &curr, COMMA_EXPACTED);
    mat3 = get_mat(line, &curr, COMMA_NOT_EXPACTED);
    
    
    if(mat1 >= 0 && mat2 >=0 && mat3 >= 0){
        for(i = 0; i < SIZE; i++) {
            for(j = 0; j < SIZE; j++) {
                mats[mat3]->m[i][j] = (mats[mat1]->m[i][j] + mats[mat2]->m[i][j]);
            }
        }
        *ptr_curr = curr;
    }
}

void sub_mat(char* line , int* ptr_curr){
    int curr = *ptr_curr, i, j, mat1, mat2, mat3;
    char name[MAX_LEN];
    
    mat1 = get_mat(line, &curr, COMMA_EXPACTED);
    mat2 = get_mat(line, &curr, COMMA_EXPACTED);
    mat3 = get_mat(line, &curr, COMMA_NOT_EXPACTED);
    
    if(mat1 >= 0 && mat2 >=0 && mat3 >= 0){
        
        for(i = 0; i < SIZE; i++) {
            for(j = 0; j < SIZE; j++) {
                mats[mat3]->m[i][j] = (mats[mat1]->m[i][j] - mats[mat2]->m[i][j]);
            }
        }
    
        *ptr_curr = curr;
    }
}

void mul_mat(char* line , int* ptr_curr){
    int curr = *ptr_curr, i, j, k, mat1, mat2, mat3;
    char name[MAX_LEN];

    mat1 = get_mat(line, &curr, COMMA_EXPACTED);
    mat2 = get_mat(line, &curr, COMMA_EXPACTED);
    mat3 = get_mat(line, &curr, COMMA_NOT_EXPACTED);
    
    if(mat1 >= 0 && mat2 >=0 && mat3 >= 0){
        for(i = 0; i < SIZE; ++i){
            for(j = 0; j < SIZE; ++j){
                for(k = 0; k < SIZE; ++k) {
                    mats[mat3]->m[i][j] += mats[mat1]->m[i][k] * mats[mat2]->m[k][j];
                }
            }
        }
        *ptr_curr = curr;
    }
}


void scalar_mat(char* line, int *ptr_curr){
    int curr = *ptr_curr, i , j, mat1, mat2;
    double scalar = 1;
    char float_word[MAX_LEN];

    mat1 = get_mat(line, &curr, COMMA_EXPACTED);
    get_number(line, float_word,&curr);
    
    if(!check_comma(line, &curr)){
        printf("Missing Commas.");
        return;
    }
    jump_comma(line, &curr);
    
    scalar = atof(float_word);
    mat2 = get_mat(line, &curr, COMMA_NOT_EXPACTED);
    
    if(mat1 >= 0 && mat2 >=0){
    
        for(i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++){
                mats[mat2]->m[i][j] = scalar * mats[mat1]->m[i][j];
            }
        }
        
        *ptr_curr = curr;
    }
}

void trans_mat(char* line , int* ptr_curr){
    int curr = *ptr_curr, i, j, mat1, mat2;
    
    mat1 = get_mat(line, &curr, COMMA_EXPACTED);
    mat2 = get_mat(line, &curr, COMMA_NOT_EXPACTED);
    
    if(mat1 >= 0 && mat2 >=0){
        
        for(i = 0; i <SIZE; i++) {
            for (j = 0; j < SIZE; j++){
                mats[mat2]->m[i][j] = mats[mat1]->m[j][i];
            }
        }
        
        *ptr_curr = curr;
    }
}

void stop(char* line , int* ptr_curr){
    int i;
    
	for(i = 0; i < NUM_OF_MATS; i++){
		free_mat(mats[i]);
	}
	
	printf("\n---Bye---\n");
	exit(0);
}



/*****************************************************
          *      start-func      *
*****************************************************/
mat_ptr create_mat(){
    int i , j;
    mat_ptr new_mat;
    new_mat = (matrix*)malloc(sizeof(matrix));
    
    if(new_mat){
        new_mat->m = (double**) calloc (SIZE, sizeof(double*));
        
        if(new_mat->m){
            for(i = 0; i < SIZE ; i++) {
                *((new_mat->m)+i) = (double*) calloc (SIZE, sizeof(double));
                
                if(!(*((new_mat->m)+i))){
                    for (j = 0; j < i; j++){
                        free(*((new_mat->m)+j));
                    }
                    return NULL;
                }
            }
        } else {
            free(new_mat);
            return NULL;
        }
    }
    return new_mat;
}

void generate_mats(){
    int i, j;
    for(i = 0; i < NUM_OF_MATS; i++){
            mats[i] = create_mat();
            if(!mats[i]){
                printf("memory allocation failed. exit program.\n");
                for(j = 0; j < i; j++){
                    free_mat(mats[j]);
                }
                exit(0);
            }
    }
}

void free_mat(mat_ptr ptr){
	int i;
	for(i = 0; i < SIZE; i++){
		free(*((ptr->m)+i));
	}
	free(ptr);
}
