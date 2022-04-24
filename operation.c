#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// method for scalar scalar operations.
// make the necessary calculations based on the operation.
float ss(float number1, float number2,char operation){
 switch (operation)
{
    case '+':
      return number1+number2;
      
    case '*':
       return number1*number2;
      
    case '-':
	return number2-number1;
	
}
return 1;
}


// method for scalar matrix(or vector) operations.
// make the necessary calculations based on the operation (operation can only be *).
float ** sm(int number,int dim1,int dim2,float** matrix){
 float** arr = (float**)malloc(dim1 * sizeof(float*));
    for (int i = 0; i < dim1; i++){
        arr[i] = (float*)malloc(dim2 * sizeof(float));
 }
for (int i = 0; i < dim1; i++){
        for (int j = 0; j < dim2; j++){
	arr[i][j] =  matrix[i][j]*number;
}
}
return arr;
}

// method for matrix(or vector) matrix(or vector) operations.
// make the necessary calculations based on the operation.
float **  mm(int dim1,int dim2,float** matrix1,int dim3,int dim4,float** matrix2,char operation){
    float** arr = NULL;
 switch (operation)
{
    case '+':
        arr = (float**)malloc(dim1 * sizeof(float*));
    for (int i = 0; i < dim1; i++){
        arr[i] = (float*)malloc(dim2 * sizeof(float));
 } 
      for(int i = 0;i < dim1;i++){
	for(int j = 0;j < dim2;j++){
        arr[i][j] = matrix1[i][j] + matrix2[i][j];
        }
	}
        return arr;
    case '*':
        arr = (float**)malloc(dim1 * sizeof(float*));
    for (int i = 0; i < dim1; i++){
        arr[i] = (float*)malloc(dim4 * sizeof(float));
 } 
	 for (int i = 0; i < dim1; i++) {
            for (int j = 0; j < dim4; j++) {
                for (int k = 0; k < dim3; k++){
                     arr[i][j] += matrix1[i][k] * matrix2[k][j];
		}            
	}
        }
	return arr;
    case '-':
        arr = (float**)malloc(dim1 * sizeof(float*));
    for (int i = 0; i < dim1; i++){
        arr[i] = (float*)malloc(dim2 * sizeof(float));
 } 
      for(int i = 0;i < dim1;i++){
        for(int j = 0;j < dim2;j++){
        arr[i][j] = matrix2[i][j] -  matrix1[i][j];
        }
        }
        return arr;
    default:
      printf("Hata");
        return NULL;
}
    return arr;
}

// implementation of choose function.
float choose(float expr1,float expr2,float expr3,float expr4){
if(expr1 == 0){
return expr2;
}else if(expr1 > 0){
return expr3;
}else if(expr1 < 0){
return expr4;
}
return 0;
}
// implementation of tr function for scalars.
float trs(float scalar){
    return scalar;
}

// implementation of tr function for matrixes and vectors.
float** trm(int dim1,int dim2,float** matrix){
    float**  arr = (float**)malloc(dim2 * sizeof(float*));
for (int i = 0; i < dim2; i++){
    arr[i] = (float*)malloc(dim1 * sizeof(float));
}
    for(int i = 0; i< dim1;++i){
        for(int j = 0; j< dim2;++j){
        
            arr[j][i] = matrix[i][j];
        }
    }
    return arr;
}

