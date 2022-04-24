#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "main.h"
#include "function.h"


int getFirstDimension(char varName[], char varlist[50][1000], int currIndex); // get the first dimension of factor in the expression.
int getSecondDimension(char varName[], char varlist[50][1000], int currIndex); // get the second dimension of factor in the expression.
// A structure to represent a stack.
// To handle postfix notation a stack implementation is done.
struct Stack {
    int top;
    unsigned capacity;
    char** array;
};

// function to create a stack of given capacity. It initializes size of.
// stack as 0.
struct Stack* createStack(unsigned capacity)
{
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (char**)malloc(stack->capacity * sizeof(char*));
    return stack;
}

// Stack is full when top is equal to the last index.
int isFull(struct Stack* stack)
{
    return stack->top == stack->capacity - 1;
}

// Stack is empty when top is equal to -1
int isEmpty(struct Stack* stack)
{
    return stack->top == -1;
}

// Function to add an item to stack.  It increases top by 1.
void push(struct Stack* stack, char* item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
    
}



// Function to return the top from stack without removing it.
char* peek(struct Stack* stack)
{
    if (isEmpty(stack))
        return "bosStack";
    return stack->array[stack->top];
}

// Function to remove an item from stack.  It decreases top by 1.
char* pop(struct Stack* stack)
{
    if (isEmpty(stack))
        return "bosStack";
    
    
    return stack->array[stack->top--];
}

// function that handles all the operations in postfix notation.
void  handleStack(char content[30][100],char arr[1200],int index,int currentLine,char rightPart[1200],char craftedArray[3][1000]) {
    struct Stack* stack = createStack(30); // create stack.
    int count = 0;
    char  variables[50][1000]; // stores all the factors in expression.
    // initialize variable array.
    for(int i = 0;i< 50 ;i++){
        for(int j = 0;j< 1000 ;j++){
            variables[i][j] = '\0';
        }
    }
    
    char  arrayOfArray[50][1000]; // stores all elements after necessary calculations are made. (For example ss(1,2,*).)
    // initialize array of array array.
    for(int i = 0;i< 50 ;i++){
        for(int j = 0;j< 1000 ;j++){
            arrayOfArray[i][j] = '\0';
        }
    }
    int arrayIndex = 0; // stores length of the array of array array.
    int countdown = 0; // stores length of the variables array.
    int fElFDim = 0; // stores first element's first dimension.
    int fElSDim = 0; // stores first element's second dimension.
    int sElFDim = 0; // stores second element's first dimension.
    int sElSDim = 0; // stores second element's second dimension.
    while(count < index){
        if(strcmp(content[count],"*") == 0 || strcmp(content[count],"+")== 0 || strcmp(content[count],"-")== 0){
            char firstElem[1000] ={0};
            
            
            char secondElem[1000]={0};
            
            strcpy(secondElem,pop(stack)); // pop the top element when operator is encountered (second operand).
            strcpy(firstElem,pop(stack));  // pop the top element when operator is encountered (first operand).
            printf("First elem:%s\n",firstElem);
            printf("Second elem:%s\n",secondElem);
            
            
            
            
            // checks if there is a error in expression.
            if(strcmp(firstElem,"bosStack") == 0 || strcmp(secondElem,"bosStack") == 0){
                error(currentLine);
            }

            
            // try to match the type of the first element and do the necessary calculations based on that.
            if (isExpression(firstElem, rightPart, currentLine)) {
                
                char craftedExpr[3][1000];
                parseAndCallExpr(firstElem, rightPart, currentLine,craftedExpr);
                strcpy(firstElem,craftedExpr[0]);
                 fElFDim = atoi(craftedExpr[1]);
                 fElSDim = atoi(craftedExpr[2]);
                
            } else if (startsWith("choose", firstElem)) {
                
                char craftedExpr[3][1000];
                parseAndCallChoose(firstElem, rightPart, currentLine,craftedExpr);
                strcpy(firstElem,craftedExpr[0]);
                 fElFDim = atoi(craftedExpr[1]);
                 fElSDim = atoi(craftedExpr[2]);


            } else if (startsWith("sqrt", firstElem)) {
                char craftedExpr[3][1000];
                parseAndCallSqrt(firstElem, rightPart, currentLine,craftedExpr);
                strcpy(firstElem,craftedExpr[0]);
                 fElFDim = atoi(craftedExpr[1]);
                 fElSDim = atoi(craftedExpr[2]);


            } else if (startsWith("tr", firstElem)) {
                char craftedExpr[3][1000];
                parseAndCallTranspose(firstElem, rightPart, currentLine,craftedExpr);
                strcpy(firstElem,craftedExpr[0]);
                 fElFDim = atoi(craftedExpr[1]);
                 fElSDim = atoi(craftedExpr[2]);

            } else if (isNumber(firstElem) == 1) {
                
                fElSDim = 0;
                fElSDim = 0;
            } else if (firstDimension(firstElem, varList, currentIndex) >= 0 &&
                       secondDimension(firstElem, varList, currentIndex) >= 0) {
                fElFDim =firstDimension(firstElem, varList, currentIndex);
                fElSDim =secondDimension(firstElem, varList, currentIndex);


            } else if (getFirstDimension(firstElem, variables, countdown) >= 0 &&
                       getSecondDimension(firstElem, variables, countdown) >= 0) {
                
                fElFDim =getFirstDimension(firstElem, variables, countdown);
                fElSDim =getSecondDimension(firstElem, variables, countdown);

            

            } else if(strstr(firstElem,"]") && strstr(firstElem,"[")){
                char craftedExpr[3][1000];
                parseAndCallArray(firstElem, rightPart, currentLine,craftedExpr);
                strcpy(firstElem,craftedExpr[0]);
                 fElFDim = atoi(craftedExpr[1]);
                 fElSDim = atoi(craftedExpr[2]);
                
                
                
            }else {
                error(currentLine); // if first element is something that is not known then give an error.

            }
            
            // try to match the type of the second element and do the necessary calculations based on that.
            if (isExpression(secondElem, rightPart, currentLine)) {
                char craftedExpr[3][1000];
                parseAndCallExpr(secondElem, rightPart, currentLine,craftedExpr);
                strcpy(secondElem,craftedExpr[0]);
                 sElFDim = atoi(craftedExpr[1]);
                 sElSDim = atoi(craftedExpr[2]);
            } else if (startsWith("choose", secondElem)) {
                char craftedExpr[3][1000];
                parseAndCallChoose(secondElem, rightPart, currentLine,craftedExpr);
                strcpy(secondElem,craftedExpr[0]);
                 sElFDim = atoi(craftedExpr[1]);
                 sElSDim = atoi(craftedExpr[2]);


            } else if (startsWith("sqrt", secondElem)) {
                char craftedExpr[3][1000];
                parseAndCallSqrt(secondElem, rightPart, currentLine,craftedExpr);
                strcpy(secondElem,craftedExpr[0]);
                 sElFDim = atoi(craftedExpr[1]);
                 sElSDim = atoi(craftedExpr[2]);



            } else if (startsWith("tr", secondElem)) {
                char craftedExpr[3][1000];
                parseAndCallTranspose(secondElem, rightPart, currentLine,craftedExpr);
                strcpy(secondElem,craftedExpr[0]);
                 sElFDim = atoi(craftedExpr[1]);
                 sElSDim = atoi(craftedExpr[2]);

            } else if (isNumber(secondElem) == 1) {
                sElFDim = 0;
                sElSDim = 0;
            } else if (firstDimension(secondElem, varList, currentIndex) >= 0 &&
                       secondDimension(secondElem, varList, currentIndex) >= 0) {
                sElFDim =firstDimension(secondElem, varList, currentIndex);
                sElSDim =secondDimension(secondElem, varList, currentIndex);


            } else if (getFirstDimension(secondElem, variables, countdown) >= 0 &&
                       getSecondDimension(secondElem, variables, countdown) >= 0) {
                
                sElFDim =getFirstDimension(secondElem, variables, countdown);
                sElSDim =getSecondDimension(secondElem, variables, countdown);

            

            } else if(strstr(secondElem,"]") && strstr(secondElem,"[")){
                
                char craftedExpr[3][1000];
                parseAndCallArray(secondElem, rightPart, currentLine,craftedExpr);
                strcpy(secondElem,craftedExpr[0]);
                 sElFDim = atoi(craftedExpr[1]);
                 sElSDim = atoi(craftedExpr[2]);
                
                
            }else {
                
                error(currentLine); // if first element is something that is not known then give an error.

            }
            
            // Do the necessary calculations based on the operation if the both elements are scalar.
            // Operation can be +,*,-.
            if(fElFDim == 0 && fElSDim == 0 && sElFDim == 0 && sElSDim == 0){
        
                strcpy(arrayOfArray[arrayIndex],"ss(");
                              strcat(arrayOfArray[arrayIndex],secondElem);
                              strcat(arrayOfArray[arrayIndex],",");
                              strcat(arrayOfArray[arrayIndex],firstElem);
                              strcat(arrayOfArray[arrayIndex],",'");
                              
                              
                              if(strcmp(content[count],"*") == 0){
                                  strcat(arrayOfArray[arrayIndex],"*')");
                              }else if(strcmp(content[count],"+") == 0){
                                  strcat(arrayOfArray[arrayIndex],"+')");
                              }else{
                                  strcat(arrayOfArray[arrayIndex],"-')");
                              }
                              char modifiedArray[1000];
                              strcpy(modifiedArray,arrayOfArray[arrayIndex]);
                              strcat(modifiedArray," ");
                              strcat(modifiedArray,"0 ");
                              strcat(modifiedArray,"0");
                              strcpy(variables[countdown],modifiedArray);
                              countdown++;
                              push(stack,arrayOfArray[arrayIndex]);
                               arrayIndex++;
               
                
                
                
                // Do the necessary calculations based on the operation if first element is scalar and the other one is matrix or vector.
                // Operation can only be *.
            }else if(fElFDim == 0 && fElSDim == 0 && sElFDim >= 0 && sElSDim >= 0){
                if(strcmp(content[count],"*") != 0){
                    error(currentLine);
                }
                char num1[20];
                sprintf(num1, "%d", sElFDim);
                char num2[20];
                sprintf(num2, "%d", sElSDim);
                strcpy(arrayOfArray[arrayIndex],"sm(");
                strcat(arrayOfArray[arrayIndex],firstElem);
                strcat(arrayOfArray[arrayIndex],",");
                strcat(arrayOfArray[arrayIndex],num1);
                strcat(arrayOfArray[arrayIndex],",");
                strcat(arrayOfArray[arrayIndex],num2);
                strcat(arrayOfArray[arrayIndex],",");
                strcat(arrayOfArray[arrayIndex],secondElem);
                strcat(arrayOfArray[arrayIndex],")");
                char modifiedArray[1000];
                strcpy(modifiedArray,arrayOfArray[arrayIndex]);
                strcat(modifiedArray," ");
                strcat(modifiedArray,num1);
                strcat(modifiedArray," ");
                strcat(modifiedArray,num2);
                strcpy(variables[countdown],modifiedArray);
                countdown++;
                push(stack,arrayOfArray[arrayIndex]);
                arrayIndex++;
                // Do the necessary calculations based on the operation if second element is scalar and other one is matrix or vector.
                // Operation can only be *.
            }else if(fElFDim >= 0 && fElSDim >=0 && sElFDim == 0 && sElSDim == 0){
                if(strcmp(content[count],"*") != 0){
                    error(currentLine);
                }
                char num1[20];
                sprintf(num1, "%d", fElFDim);
                char num2[20];
                sprintf(num2, "%d", fElSDim);
                strcpy(arrayOfArray[arrayIndex],"sm(");
                strcat(arrayOfArray[arrayIndex],secondElem);
                strcat(arrayOfArray[arrayIndex],",");
                strcat(arrayOfArray[arrayIndex],num1);
                strcat(arrayOfArray[arrayIndex],",");
                strcat(arrayOfArray[arrayIndex],num2);
                strcat(arrayOfArray[arrayIndex],",");
                strcat(arrayOfArray[arrayIndex],firstElem);
                strcat(arrayOfArray[arrayIndex],")");
                char modifiedArray[1000];
                strcpy(modifiedArray,arrayOfArray[arrayIndex]);
                strcat(modifiedArray," ");
                strcat(modifiedArray,num1);
                strcat(modifiedArray," ");
                strcat(modifiedArray,num2);
                strcpy(variables[countdown],modifiedArray);
                countdown++;
                push(stack,arrayOfArray[arrayIndex]);
                arrayIndex++;
                // Do the necessary calculations based on the operation if the both elements are matrix or vector.
                // Operation can only be *,+,-.
            }else if(fElFDim >= 0 && fElSDim >=0 && sElFDim >= 0 && sElSDim >= 0){
                if(strcmp(content[count],"+") == 0 || strcmp(content[count],"-") == 0){
                    if(fElFDim == sElFDim && fElSDim == sElSDim){
                        char num1[20];
                        sprintf(num1, "%d", fElFDim);
                        char num2[20];
                        sprintf(num2, "%d", fElSDim);
                        strcpy(arrayOfArray[arrayIndex],"mm(");
                        strcat(arrayOfArray[arrayIndex],num1);
                        strcat(arrayOfArray[arrayIndex],",");
                        strcat(arrayOfArray[arrayIndex],num2);
                        strcat(arrayOfArray[arrayIndex],",");
                        strcat(arrayOfArray[arrayIndex],secondElem);
                        strcat(arrayOfArray[arrayIndex],",");
                        strcat(arrayOfArray[arrayIndex],num1);
                        strcat(arrayOfArray[arrayIndex],",");
                        strcat(arrayOfArray[arrayIndex],num2);
                        strcat(arrayOfArray[arrayIndex],",");
                        strcat(arrayOfArray[arrayIndex],firstElem);
                        strcat(arrayOfArray[arrayIndex],",'");
                        if(strcmp(content[count],"+") == 0){
                            strcat(arrayOfArray[arrayIndex],"+')");
                        }else{
                            strcat(arrayOfArray[arrayIndex],"-')");
                        }
                        
                        char modifiedArray[1000];
                        strcpy(modifiedArray,arrayOfArray[arrayIndex]);
                        strcat(modifiedArray," ");
                        strcat(modifiedArray,num1);
                        strcat(modifiedArray," ");
                        strcat(modifiedArray,num2);
                        strcpy(variables[countdown],modifiedArray);
                        countdown++;
                        push(stack,arrayOfArray[arrayIndex]);
                        arrayIndex++;
                    }else{
                        error(currentLine);
                    }
                }else{
                    if(fElSDim == sElFDim){
                        char num1[20];
                        sprintf(num1, "%d", fElFDim);
                        char num2[20];
                        sprintf(num2, "%d", sElSDim);
                        char num3[20];
                        sprintf(num3, "%d", fElSDim);
                        strcpy(arrayOfArray[arrayIndex],"mm(");
                        strcat(arrayOfArray[arrayIndex],num1);
                        strcat(arrayOfArray[arrayIndex],",");
                        strcat(arrayOfArray[arrayIndex],num3);
                        strcat(arrayOfArray[arrayIndex],",");
                        strcat(arrayOfArray[arrayIndex],firstElem);
                        strcat(arrayOfArray[arrayIndex],",");
                        strcat(arrayOfArray[arrayIndex],num3);
                        strcat(arrayOfArray[arrayIndex],",");
                        strcat(arrayOfArray[arrayIndex],num2);
                        strcat(arrayOfArray[arrayIndex],",");
                        strcat(arrayOfArray[arrayIndex],secondElem);
                        strcat(arrayOfArray[arrayIndex],",'");
                        strcat(arrayOfArray[arrayIndex],"*')");
                        char modifiedArray[1000];
                        strcpy(modifiedArray,arrayOfArray[arrayIndex]);
                        strcat(modifiedArray," ");
                        strcat(modifiedArray,num1);
                        strcat(modifiedArray," ");
                        strcat(modifiedArray,num2);
                        strcpy(variables[countdown],modifiedArray);
                        countdown++;
                        push(stack,arrayOfArray[arrayIndex]);
                        arrayIndex++;
                    }else{
                        error(currentLine);
                    }
                }
            }


            

        }else{
            push(stack,content[count]); // if current element is not an operator then push the element to stack.
        }
        count++;
    }
    pop(stack);
    char num1[1000];
    sprintf(num1, "%d", getFirstDimension(arrayOfArray[arrayIndex-1], variables, countdown));
    char num2[1000];
    sprintf(num2, "%d", getSecondDimension(arrayOfArray[arrayIndex-1], variables, countdown));
    strcpy(craftedArray[0],arrayOfArray[arrayIndex-1]);
    
    strcpy(craftedArray[1],num1);
    strcpy(craftedArray[2],num2);
    
}





// Return the first dimension of varName from the varlist array.
// Returns -1 if element is not found.
int getFirstDimension(char varName[], char varlist[50][1000], int currIndex) {
    char copiedVar[1000];

    for (int var = 0; var < currIndex; var++) {
        strcpy(copiedVar, varlist[var]);
        char *identName = strtok(copiedVar, " ");
        char *firstDimension = strtok(NULL, " ");
        char *secDimension = strtok(NULL, " ");
        if ((strcmp(varName, identName)) == 0) {
            int newVal = (atoi(firstDimension));
            return (newVal);
            break;
        }
    }
    return -1;
}

// Return the second dimension of varName from the varlist array.
// Returns -1 if element is not found.
int getSecondDimension(char varName[], char varlist[50][1000], int currIndex) {
    char copiedVar[1000];

    for (int var = 0; var < currIndex; var++) {
        strcpy(copiedVar, varlist[var]);
        char *identName = strtok(copiedVar, " ");
        char *firstDimension = strtok(NULL, " ");
        char *secDimension = strtok(NULL, " ");
        if ((strcmp(varName, identName)) == 0) {
            int newVal = (atoi(secDimension));
            return (newVal);
        }
    }
    return -1;
}
