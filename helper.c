#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "function.h"
#include "main.h"
#include "stack.h"

void arrangeArray(char *pointer);

void assignArray(char *array, int firstD, int secondD, int currentLine);

void error(int currentLine);

int firstDimension(char varName[], char varlist[300][50], int currIndex);

int secondDimension(char varName[], char varlist[300][50], int currIndex);

int choose(char expr1[], char expr2[], char expr3[], char expr4[], char rightPart[1200], int currentLine,
           char craftedArray[3][1000]);

int tr(char insideTr[], char rightPart[1200], int currentLine, char craftedArray[3][1000]);

int sqrtt(char insideSqrt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]);

int expr(char tokens[100][100], char *str, int *cur);

int term(char tokens[100][100], char *str, int *cur);

int moreterms(char tokens[100][100], char *str, int *cur);

int factor(char tokens[100][100], char *str, int *cur);

int morefactors(char tokens[100][100], char *str, int *cur);

void parseAndCallChoose(char chooseStmt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]);

void parseAndCallTranspose(char transposeStmt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]);

void parseAndCallSqrt(char sqrtStmt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]);

void parseAndCallExpr(char exprStmt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]);

int isExpression(char exprStmt[], char rightPart[1200], int currentLine);

void removeUnnecessaryPharanthesis(char copiedExpr[]);

int isNumber(char s[]);

void handleAssignment(char *leftPart, char rightPart[1200], int currentLine);

void handleIndexing(char content[]);

void handleVector(char content[]);

int array(char varName[], char insideArrayFirst[], char insideArraySecond[],char rightPart[1200], int currentLine, char craftedArray[3][1000]);

void parseAndCallArray(char arrayStmt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]);

void remove_white_spaces(char *str);

void handleAssignment(char *leftPart, char rightPart[1200], int currentLine) {  // takes the variable and expression with "=" sign and does the necessary actions.
    
    char copiedExpr[1200];
    arrangeArray(rightPart); // handle matrix dimensions.(A[1,2] -> A[1][2]).
    if (strcmp("!?forLoopCheck!?",leftPart)!=0) {
        trim(rightPart);
        trim(leftPart);
    }
    char copiedLeftForVector[80];
    strcpy(copiedLeftForVector,leftPart);
    if(strstr(copiedLeftForVector,"[") && strstr(copiedLeftForVector,"]")){ // handle left part if it contains square brackets.
        char* leftPartVar = strtok(copiedLeftForVector,"[");
        trim(leftPartVar);
        if(firstDimension(leftPartVar,varList,currentIndex) >= 1 && secondDimension(leftPartVar,varList,currentIndex) >= 1){
            char copiedAnotherLeft[250];
            char temporRightSide[1200];
            char leftCraftedArray[3][1000];
            strcpy(copiedAnotherLeft,leftPart);
            handleVector(copiedAnotherLeft);
            parseAndCallArray(copiedAnotherLeft,temporRightSide,currentLine,leftCraftedArray);
            if(strcmp(leftCraftedArray[1],"0") != 0 || strcmp(leftCraftedArray[2],"0") != 0){
                error(currentLine);
            }
            strcpy(leftPart,leftCraftedArray[0]);
        }
    }
    handleVector(rightPart); // add [1]to the end of the indexing if there is a vector.
    
    
    
    
    
    strcpy(copiedExpr, rightPart);
    removeUnnecessaryPharanthesis(copiedExpr);
    removeUnnecessaryPharanthesis(leftPart);

    

    char craftedArray[3][1000];
    // reset craftedArray.
    for(int i = 0;i< 3 ;i++){
        for(int j = 0;j< 1000 ;j++){
            craftedArray[i][j] = '\0';
        }
    }
    
    if (isExpression(copiedExpr, rightPart, currentLine)) { //checks the statement whether it has operators in the outermost shell.

        parseAndCallExpr(copiedExpr, rightPart, currentLine, craftedArray);
        

    } else if (startsWith("choose", copiedExpr)) {  // checks whether outermost shell is in choose function.
        
        parseAndCallChoose(copiedExpr, rightPart, currentLine, craftedArray);

    } else if (startsWith("sqrt", copiedExpr)) { // checks whether outermost shell is in sqrt function.
        parseAndCallSqrt(copiedExpr, rightPart, currentLine, craftedArray);

    } else if (startsWith("tr", copiedExpr)) { // checks whether outermost shell is in tr function.
        parseAndCallTranspose(copiedExpr, rightPart, currentLine, craftedArray);

    } else if (startsWith("{", copiedExpr) && strstr(copiedExpr, "}")) { // checks whether the statement is array declaration.
        
        
        
        if (firstDimension(leftPart, varList, currentIndex) == 0 &&
            secondDimension(leftPart, varList, currentIndex) == 0) {
            error(currentLine);
        }else if (firstDimension(leftPart, varList, currentIndex) >= 1 &&
                  secondDimension(leftPart, varList, currentIndex) >= 1) {
            
            assignArray(rightPart, firstDimension(leftPart, varList, currentIndex),
                        secondDimension(leftPart, varList, currentIndex), currentLine);
            return;
        } else if (strstr(leftPart, "[") && strstr(leftPart, "]")) {
            return;
        } else {
            if (strcmp("!?forLoopCheck!?", leftPart) != 0)
                error(currentLine);
        }
    } else if (isNumber(rightPart) == 1) { // checks whether the statement is a number.

        strcpy(craftedArray[0], rightPart);
        strcpy(craftedArray[1], "0");
        strcpy(craftedArray[2], "0");
        if (firstDimension(leftPart, varList, currentIndex) == 0 &&
            secondDimension(leftPart, varList, currentIndex) == 0) {
            return;
        } else if (strstr(leftPart, "[") && strstr(leftPart, "]")) {
            handleIndexing(leftPart);
            return;
        } else {
            if (strcmp("!?forLoopCheck!?", leftPart) != 0)
                error(currentLine);
        }
    } else if (strstr(rightPart,"]") && strstr(rightPart,"[")) { // checks whether the statement is an array operation.
        parseAndCallArray(copiedExpr,rightPart,currentLine,craftedArray);
    }else if (firstDimension(leftPart, varList, currentIndex) == firstDimension(rightPart, varList, currentIndex) &&
              secondDimension(leftPart, varList, currentIndex) == secondDimension(rightPart, varList, currentIndex) && firstDimension(leftPart, varList, currentIndex) >=0) {
        return;
    } else {

        if (strcmp("!?forLoopCheck!?", leftPart) != 0)
            error(currentLine);
        return;
    }


    if (strstr(leftPart, "[") && strstr(leftPart, "]")) {
        strcpy(rightPart, craftedArray[0]);
    }
    

    if (firstDimension(leftPart, varList, currentIndex) == atoi(craftedArray[1]) &&
        secondDimension(leftPart, varList, currentIndex) == atoi(craftedArray[2])) {
        strcpy(rightPart, craftedArray[0]);
    }
    else if (strcmp("!?forLoopCheck!?", leftPart) == 0) { // detects the variables inside for loop scope.
        
        if (atoi(craftedArray[1])!=0||atoi(craftedArray[2])!=0)
            error(currentLine);
        
        strcpy(rightPart, craftedArray[0]);
        
    }

    else {
        error(currentLine);
    }

    handleIndexing(leftPart); // add -1 to each index in leftPart (if there exists).
    handleIndexing(rightPart); // add -1 to each index in rightPart (if there exists).

}


void arrangeArray(char *pointer) { // takes the array and changes the commas with brackets for C language.
    int i = 0;
    char *replaced = "][";
    char open = '(';
    char close = ')';
    int pharanthesesCount = 0; // stores the number of parantheses.
    while (pointer[i] != '\0') {
        if (pointer[i] == '[') {
            int counter = 0;
            int isValid = 0;
            while (!(pointer[i + counter] == ']')) {
                if (pointer[i + counter] == ',' && pharanthesesCount == 0) {
                    isValid = 1;
                    break;
                }else if(pointer[i+counter] == open){
                    
                    pharanthesesCount++;
                }else if(pointer[i+counter] == close){
                    
                    pharanthesesCount--;
                }
                counter++;
            }
            
            if (pointer[i + counter] == '\0') {
                break;
            }
            
            // change the string according to given index.
            if (isValid == 1 && pharanthesesCount == 0) {
                char buf[100] = {};
                strncpy(buf, pointer, i + counter);
                int len = strlen(buf);
                strcpy(buf + len, replaced);
                len += strlen(replaced);
                strcpy(buf + len, pointer + i + counter + 1);
                strcpy(pointer, buf);
                
            }

        }
        i++;
    }
}

void error(int currentLine) {  // general error prompt for termination of the process and it detects the error line.
    char errorContent[90];
    char num[20];
    sprintf(num, "%d", currentLine);
    remove(outputFileName);
    strcpy(errorContent, "Error (Line ");
    strcat(errorContent, num);
    strcat(errorContent, ")");
    printf("%s\n", errorContent);
    exit(0);
}

int firstDimension(char varName[], char varlist[300][50], int currIndex) {  // finds the first dimension of the variables from varList array.
    char copiedVar[50];

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

int secondDimension(char varName[], char varlist[300][50], int currIndex) { // finds the second dimension of the variables from varList array.
    char copiedVar[50];

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

int isNumber(char s[]) {  // checks whether the input is a number or not.
    int dotCount = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '.') {
            dotCount++;
            continue;
        }
        if (dotCount > 1) {
            return 0;
        }
        if ((isdigit(s[i]) != 0) || s[i] == '.') {
            continue;
        } else {
            return 0;
        }
    }
    return 1;
}

// do the necessary adjusments for vector or matrix initialization or assignment.
void assignArray(char *array, int firstD, int secondD, int currentLine) {
    char *pointer = strtok(array, "{");
    char *insideArray = strtok(pointer, "}");
    char charArray[80][80];
    for(int i = 0;i< 80 ;i++){
        for(int j = 0;j< 80 ;j++){
            charArray[i][j] = '\0';
        }
    }
    trim(insideArray);
    char *token = strtok(insideArray, " ");
    strcpy(charArray[0], token);
    int index = 1;
    while (token != NULL) {
        token = strtok(NULL, " ");
        if (token != NULL) {
            strcpy(charArray[index], token);

            index++;
        }
    }
    if (index != firstD * secondD) {
        error(currentLine);
        return;
    }
    char combinedArray[100];

    strcpy(combinedArray, "{");
    index = 0;
    for (int i = 0; i < firstD; i++) {
        char innerArray[50];
        strcpy(innerArray, "{");
        for (int j = 0; j < secondD; j++) {
            strcat(innerArray, charArray[index]);
            if (j != secondD - 1) {
                strcat(innerArray, ",");
            } else {
                strcat(innerArray, "}");
            }

            index++;
        }
        if (strlen(combinedArray) != 1) {
            strcat(combinedArray, ",");
        }
        strcat(combinedArray, innerArray);
    }
    strcat(combinedArray, "}");
    strcpy(array, combinedArray);
}

int choose(char expr1[], char expr2[], char expr3[], char expr4[], char rightPart[1200], int currentLine,char craftedArray[3][1000]) {  // takes the choose function parameters and recursively controls the layers of the expressions.
    char copyExpr1[250];
    strcpy(copyExpr1, expr1);
    char copyExpr2[250];
    strcpy(copyExpr2, expr2);
    char copyExpr3[250];
    strcpy(copyExpr3, expr3);
    char copyExpr4[250];
    strcpy(copyExpr4, expr4);
    int isCompact1 = 0;
    int isCompact2 = 0;
    int isCompact3 = 0;
    int isCompact4 = 0;

    removeUnnecessaryPharanthesis(copyExpr1);
    removeUnnecessaryPharanthesis(copyExpr2);
    removeUnnecessaryPharanthesis(copyExpr3);
    removeUnnecessaryPharanthesis(copyExpr4);

    

    char craftedChoose[1000];
    char craftedExpr1[3][1000];
    // reset craftedExpr1.
    for(int i = 0;i< 3 ;i++){
        for(int j = 0;j< 1000 ;j++){
            craftedExpr1[i][j] = '\0';
        }
    }
    char craftedExpr2[3][1000];
    // reset craftedExpr2.
    for(int i = 0;i< 3 ;i++){
        for(int j = 0;j< 1000 ;j++){
            craftedExpr2[i][j] = '\0';
        }
    }
    char craftedExpr3[3][1000];
    // reset craftedExpr3.
    for(int i = 0;i< 3 ;i++){
        for(int j = 0;j< 1000 ;j++){
            craftedExpr3[i][j] = '\0';
        }
    }
    char craftedExpr4[3][1000];
    // reset craftedExpr4.
    for(int i = 0;i< 3 ;i++){
        for(int j = 0;j< 1000 ;j++){
            craftedExpr4[i][j] = '\0';
        }
    }
    

    if (isExpression(copyExpr1, rightPart, currentLine)) { //checks the statement whether it has operators in the outermost shell.
        isCompact1++;
        parseAndCallExpr(copyExpr1, rightPart, currentLine, craftedExpr1);
    } else if (startsWith("tr", copyExpr1)) { // checks whether the statement is in tr function.
        isCompact1++;
        parseAndCallTranspose(copyExpr1, rightPart, currentLine, craftedExpr1);
    } else if (startsWith("choose", copyExpr1)) {  // checks whether the statement is in choose function.
        isCompact1++;
        parseAndCallChoose(copyExpr1, rightPart, currentLine, craftedExpr1);
    } else if (startsWith("sqrt", copyExpr1)) { // checks whether the statement is in sqrt function.
        isCompact1++;
        parseAndCallSqrt(copyExpr1, rightPart, currentLine, craftedExpr1);
    } else {
        strcpy(craftedExpr1[0], copyExpr1);
    }


    if (isExpression(copyExpr2, rightPart, currentLine)) { //checks the statement whether it has operators in the outermost shell.
        isCompact2++;
        parseAndCallExpr(copyExpr2, rightPart, currentLine, craftedExpr2);
    } else if (startsWith("tr", copyExpr2)) { // checks whether the statement is in tr function.
        isCompact2++;
        parseAndCallTranspose(copyExpr2, rightPart, currentLine, craftedExpr2);
    } else if (startsWith("choose", copyExpr2)) { // checks whether the statement is in choose function.
        isCompact2++;
        parseAndCallChoose(copyExpr2, rightPart, currentLine, craftedExpr2);
    } else if (startsWith("sqrt", copyExpr2)) { // checks whether the statement is in sqrt function.
        isCompact2++;
        parseAndCallSqrt(copyExpr2, rightPart, currentLine, craftedExpr2);
    } else {
        strcpy(craftedExpr2[0], copyExpr2);
    }


    if (isExpression(copyExpr3, rightPart, currentLine)) { //checks the statement whether it has operators in the outermost shell.
        isCompact3++;
        parseAndCallExpr(copyExpr3, rightPart, currentLine, craftedExpr3);
    } else if (startsWith("tr", copyExpr3)) { // checks whether the statement is in tr function.
        isCompact3++;
        parseAndCallTranspose(copyExpr3, rightPart, currentLine, craftedExpr3);
    } else if (startsWith("choose", copyExpr3)) { // checks whether the statement is in choose function.
        isCompact3++;
        parseAndCallChoose(copyExpr3, rightPart, currentLine, craftedExpr3);
    } else if (startsWith("sqrt", copyExpr3)) { // checks whether the statement is in sqrt function.
        isCompact3++;
        parseAndCallSqrt(copyExpr3, rightPart, currentLine, craftedExpr3);
    } else {
        strcpy(craftedExpr3[0], copyExpr3);
    }


    if (isExpression(copyExpr4, rightPart, currentLine)) { //checks the statement whether it has operators in the outermost shell.
        isCompact4++;
        parseAndCallExpr(copyExpr4, rightPart, currentLine, craftedExpr4);
    } else if (startsWith("tr", copyExpr4)) { // checks whether the statement is in tr function.
        isCompact4++;
        parseAndCallTranspose(copyExpr4, rightPart, currentLine, craftedExpr4);
    } else if (startsWith("choose", copyExpr4)) { // checks whether the statement is in choose function.
        isCompact4++;
        
        parseAndCallChoose(copyExpr4, rightPart, currentLine, craftedExpr4);
    } else if (startsWith("sqrt", copyExpr4)) { // checks whether the statement is in sqrt function.
        isCompact4++;
        parseAndCallSqrt(copyExpr4, rightPart, currentLine, craftedExpr4);
    } else {
        strcpy(craftedExpr4[0], copyExpr4);
    }

    int numOfClose1 = 0;
    for (int i = 0; i< strlen(copyExpr1);i++){
        if (copyExpr1[i]==']')
            numOfClose1++;
    }
    if ((firstDimension(copyExpr1, varList, currentIndex) == 0) || isNumber(copyExpr1) == 1 || isCompact1 > 0) { // checks the expression whether it is number or not.
        
        strcpy(craftedExpr1[1], "0");
        strcpy(craftedExpr1[2], "0");
    } else if (strstr(copyExpr1,"]")&&strstr(copyExpr1,"[")) {
        parseAndCallArray(copyExpr1,rightPart,currentLine,craftedExpr1); // if the expression is an array statement calls the function.
    } else {
        error(currentLine);
    }


    if ((firstDimension(copyExpr2, varList, currentIndex) == 0) || isNumber(copyExpr2) == 1 || isCompact2 > 0) { // checks the expression whether it is number or not.
        
        strcpy(craftedExpr2[1], "0");
        strcpy(craftedExpr2[2], "0");
    }  else if (strstr(copyExpr2,"]")&&strstr(copyExpr2,"[")) {
        parseAndCallArray(copyExpr2,rightPart,currentLine,craftedExpr2);  // if the expression is an array statement calls the function.
    } else {
        error(currentLine);
    }

    if ((firstDimension(copyExpr3, varList, currentIndex) == 0) || isNumber(copyExpr3) == 1 || isCompact3 > 0) { // checks the expression whether it is number or not.
        
        strcpy(craftedExpr3[1], "0");
        strcpy(craftedExpr3[2], "0");
    } else if (strstr(copyExpr3,"]")&&strstr(copyExpr3,"[")) {
        parseAndCallArray(copyExpr3,rightPart,currentLine,craftedExpr3); // if the expression is an array statement calls the function.
    } else {
        error(currentLine);
    }

    if ((firstDimension(copyExpr4, varList, currentIndex) == 0) || isNumber(copyExpr4) == 1 || isCompact4 > 0) { // checks the expression whether it is number or not.
        
        strcpy(craftedExpr4[1], "0");
        strcpy(craftedExpr4[2], "0");
    } else if (strstr(copyExpr4,"]")&&strstr(copyExpr4,"[")) {
        parseAndCallArray(copyExpr4,rightPart,currentLine,craftedExpr4); // if the expression is an array statement calls the function.
    } else {
        
        error(currentLine);
    }


    if (!(strcmp(craftedExpr1[1], "0") == 0 && strcmp(craftedExpr1[2], "0") == 0)) { // if expressions are not a scalar, terminates and gives an error.
        error(currentLine);
    } else if (!(strcmp(craftedExpr2[1], "0") == 0 && strcmp(craftedExpr2[2], "0") == 0)) {
        error(currentLine);
    } else if (!(strcmp(craftedExpr3[1], "0") == 0 && strcmp(craftedExpr3[2], "0") == 0)) {
        error(currentLine);
    } else if (!(strcmp(craftedExpr4[1], "0") == 0 && strcmp(craftedExpr4[2], "0") == 0)) {
        error(currentLine);
    }


    strcpy(craftedChoose, "choose(");
    strcat(craftedChoose, craftedExpr1[0]);
    strcat(craftedChoose, ",");
    strcat(craftedChoose, craftedExpr2[0]);
    strcat(craftedChoose, ",");
    strcat(craftedChoose, craftedExpr3[0]);
    strcat(craftedChoose, ",");
    strcat(craftedChoose, craftedExpr4[0]);
    strcat(craftedChoose, ")");
    strcpy(craftedArray[0], craftedChoose);
    strcpy(craftedArray[1], "0");
    strcpy(craftedArray[2], "0");
    
    return 0;
}

void parseAndCallChoose(char chooseStmt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]) { // takes the choose statement and parses it to the expressions and calls the function <choose>.
    char copiedExpr[1000];
    strcpy(copiedExpr, chooseStmt);
    trim(copiedExpr);
    char *e;
    e = strchr(copiedExpr, '(');
    int indexOfCommas[5];
    int pharanthesisCount = 0; // stores number of parantheses.
    int commaCount = 0; // stores number of commas.
    char open = '(';
    char close = ')';
    char comma = ',';
    
    int indofFirstPharanthesis = (int) (e - copiedExpr);
    
    int indofSecondPharanthesis = strlen(copiedExpr) - 1;
    

    
    for (int i = 0; i < strlen(copiedExpr); i++) {
        if (open == copiedExpr[i]) {
            
            pharanthesisCount++;
            continue;
        } else if (close == copiedExpr[i]) {
            
            if (pharanthesisCount > 0) {
                pharanthesisCount--;
            } else {
                error(currentLine);
            }
            continue;
        }
        if ((copiedExpr[i] == comma) && pharanthesisCount == 1) {
            
            commaCount++;
            indexOfCommas[commaCount] = i;
        }
    }
    if (pharanthesisCount != 0) {
        error(currentLine);
    }
    char exc = '`';
    copiedExpr[indofFirstPharanthesis] = exc;
    copiedExpr[indofSecondPharanthesis] = exc;
    copiedExpr[indexOfCommas[1]] = exc;
    copiedExpr[indexOfCommas[2]] = exc;
    copiedExpr[indexOfCommas[3]] = exc;

    char *expr1;
    char *expr2;
    char *expr3;
    char *expr4;

    char *firstChoose = strtok(copiedExpr, "`");
    expr1 = strtok(NULL, "`");
    expr2 = strtok(NULL, "`");
    expr3 = strtok(NULL, "`");
    expr4 = strtok(NULL, "`");
    trim(expr1);
    trim(expr2);
    trim(expr3);
    trim(expr4);

    
    choose(expr1, expr2, expr3, expr4, rightPart, currentLine, craftedArray);
}

int tr(char insideTr[], char rightPart[1200], int currentLine, char craftedArray[3][1000]) { // takes the inside of the transpose function and controls it recursively.
    char copyInside[1000];
    strcpy(copyInside, insideTr);
    removeUnnecessaryPharanthesis(copyInside);

    char craftedTr[3][1000];
    for(int i = 0;i< 3 ;i++){
        for(int j = 0;j< 1000 ;j++){
            craftedTr[i][j] = '\0';
        }
    }
    char crafted[1000]; //x*2

    if (isExpression(copyInside, rightPart, currentLine)) { //checks the statement whether it has operators in the outermost shell
        parseAndCallExpr(copyInside, rightPart, currentLine, craftedTr);
    } else if (startsWith("choose", copyInside)) { // checks whether the statement is in choose function.
        parseAndCallChoose(copyInside, rightPart, currentLine, craftedTr);
    } else if (startsWith("sqrt", copyInside)) { // checks whether the statement is in sqrt function.
        parseAndCallSqrt(copyInside, rightPart, currentLine, craftedTr);
    } else if (startsWith("tr", copyInside)) { // checks whether the statement is in tr function.
        parseAndCallTranspose(copyInside, rightPart, currentLine, craftedTr);
    } else if (strstr(copyInside,"]")&&strstr(copyInside,"[")) { // if it is an array statement calls the relevant function.
        parseAndCallArray(copyInside,rightPart, currentLine, craftedTr);

    } else if ((firstDimension(copyInside, varList, currentIndex) == -1) && isNumber(copyInside) != 1) {
        
        error(currentLine);
    } else {
        strcpy(craftedTr[0], insideTr);
        if (isNumber(copyInside) == 1) {
            strcpy(craftedTr[1], "0");
            strcpy(craftedTr[2], "0");
        } else {
            char first[1000];
            char second[1000];

            sprintf(first, "%d", firstDimension(copyInside, varList, currentIndex));
            sprintf(second, "%d", secondDimension(copyInside, varList, currentIndex));
            strcpy(craftedTr[1], first);
            strcpy(craftedTr[2], second);
        }
    }
    if(strcmp(craftedTr[1],"0") == 0 && strcmp(craftedTr[2],"0") == 0){ // checkes whether return value is scalar or matrix (or vector).
        strcpy(crafted,"trs(");
        strcat(crafted,craftedTr[0]);
        strcat(crafted,")");
    }else{
        strcpy(crafted,"trm(");
        strcat(crafted,craftedTr[1]);
        strcat(crafted,",");
        strcat(crafted,craftedTr[2]);
        strcat(crafted,",");
        strcat(crafted,craftedTr[0]);
        strcat(crafted,")");
    }

    char temp[1000];
    strcpy(temp, craftedTr[1]);
    strcpy(craftedTr[1], craftedTr[2]);
    strcpy(craftedTr[2], temp);

    strcpy(craftedArray[0], crafted);
    strcpy(craftedArray[1], craftedTr[1]);
    strcpy(craftedArray[2], craftedTr[2]);
    return secondDimension(insideTr, varList, currentIndex);
}

void parseAndCallTranspose(char transposeStmt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]) { // takes the whole transpose statement and extracts the inside of it.
    
    char copiedExpr[1000];
    strcpy(copiedExpr, transposeStmt);
    trim(copiedExpr);

    char open = '(';
    char close = ')';
    int pharanthesisCount = 0; // stores the number of parantheses.

    

    for (int i = 0; i < strlen(copiedExpr); i++) {
        if (open == copiedExpr[i]) {
            pharanthesisCount++;
            continue;
        } else if (close == copiedExpr[i]) {
            
            if (pharanthesisCount > 0) {
                pharanthesisCount--;
            } else {
                
                error(currentLine);
            }
            continue;
        }
    }

    if (pharanthesisCount != 0) {
        
        error(currentLine);
    }

    char *e;
    e = strchr(copiedExpr, '(');

    int indofFirstPharanthesis = (int) (e - copiedExpr);
    int indofSecondPharanthesis = strlen(copiedExpr) - 1;

    
    


    char exc = '`';
    copiedExpr[indofFirstPharanthesis] = exc;
    copiedExpr[indofSecondPharanthesis] = exc;
    char *expr;
    char *leftTrPart = strtok(copiedExpr, "`");
    expr = strtok(NULL, "`");
    trim(expr);

    
    tr(expr, rightPart, currentLine, craftedArray);

}

int sqrtt(char insideSqrt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]) { // takes the inside of the sqrt function and controls it recursively.
    char copyInside[1000];
    strcpy(copyInside, insideSqrt);
    removeUnnecessaryPharanthesis(copyInside);
    char craftedSqrt[3][1000];
    for(int i = 0;i< 3 ;i++){
        for(int j = 0;j< 1000 ;j++){
            craftedSqrt[i][j] = '\0';
        }
    }
    char crafted[1000];


    if (isExpression(copyInside, rightPart, currentLine)) { //checks the statement whether it has operators in the outermost shell
        parseAndCallExpr(copyInside, rightPart, currentLine, craftedSqrt);
    } else if (startsWith("choose", copyInside)) { // checks whether the statement is in choose function.
        parseAndCallChoose(copyInside, rightPart, currentLine, craftedSqrt);
    } else if (startsWith("sqrt", copyInside)) { // checks whether the statement is in sqrt function.
        parseAndCallSqrt(copyInside, rightPart, currentLine, craftedSqrt);
    } else if (startsWith("tr", copyInside)) { // checks whether the statement is in tr function.
        parseAndCallTranspose(copyInside, rightPart, currentLine, craftedSqrt);
    }  else if (strstr(copyInside,"]")&&strstr(copyInside,"[")) { // if it is an array statement calls the relevant function.
        parseAndCallArray(copyInside,rightPart, currentLine, craftedSqrt);
    } else if (!((firstDimension(copyInside, varList, currentIndex) == 0) || isNumber(copyInside) == 1)) { // gives error in case of invalid expression.
        
        error(currentLine);
    }
    else {
        strcpy(craftedSqrt[0], insideSqrt);
        strcpy(craftedSqrt[1], "0");
        strcpy(craftedSqrt[2], "0");
    }

    if (!(strcmp(craftedSqrt[1], "0") == 0 && strcmp(craftedSqrt[2], "0") == 0)) {
        error(currentLine);
    }
    strcpy(crafted, "sqrt(");
    strcat(crafted, craftedSqrt[0]);
    strcat(crafted, ")");

    strcpy(craftedArray[0], crafted);
    strcpy(craftedArray[1], craftedSqrt[1]);
    strcpy(craftedArray[2], craftedSqrt[2]);
    return 0;
}

void parseAndCallSqrt(char sqrtStmt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]) { // takes the whole sqrt statement and extracts the inside of it.
    
    char copiedExpr[1000];
    strcpy(copiedExpr, sqrtStmt);
    trim(copiedExpr);

    char open = '(';
    char close = ')';
    int pharanthesisCount = 0;

    for (int i = 0; i < strlen(copiedExpr); i++) {
        if (open == copiedExpr[i]) {
            pharanthesisCount++;
            continue;
        } else if (close == copiedExpr[i]) {
            
            if (pharanthesisCount > 0) {
                pharanthesisCount--;
            } else {
                error(currentLine);
            }
            continue;
        }
    }
    if (pharanthesisCount != 0) {
        error(currentLine);
    }


    char *e;
    e = strchr(copiedExpr, '(');

    int indofFirstPharanthesis = (int) (e - copiedExpr);
    int indofSecondPharanthesis = strlen(copiedExpr) - 1;

    char exc = '`';
    copiedExpr[indofFirstPharanthesis] = exc;
    copiedExpr[indofSecondPharanthesis] = exc;
    char *expr;

    char *leftSqrtPart = strtok(copiedExpr, "`");
    expr = strtok(NULL, "`");
    trim(expr);

    sqrtt(expr, rightPart, currentLine, craftedArray);

}

// function that convert 2d array's elements to postfix notation.
int expr(char tokens[100][100], char *str, int *cur) {
    char str1[1000], str2[1000];

    str1[0] = str2[0] = '\0';
    if (!term(tokens, str1, cur)) {
        return (0);
    }
    if (!moreterms(tokens, str2, cur)) {
        return (0);
    }
    strcat(str1, str2);
    strcpy(str, str1);
    return (1);
}

int term(char tokens[100][100], char *str, int *cur) {
    char str1[1000], str2[1000];

    str1[0] = str2[0] = '\0';
    if (!factor(tokens, str1, cur)) {
        return (0);
    }
    if (!morefactors(tokens, str2, cur)) {
        return (0);
    }

    strcat(str1, str2);
    strcpy(str, str1);
    return (1);
}

int moreterms(char tokens[100][100], char *str, int *cur) {
    char str1[1000], str2[1000], str3[1000];

    str1[0] = str2[0] = str3[0] = '\0';

    if ((strcmp(tokens[*cur], "+") == 0) || (strcmp(tokens[*cur], "-") == 0)) {
        strcpy(str1, tokens[*cur]);
        strcat(str1, " ");
        *cur = *cur + 1;
        if (!term(tokens, str2, cur)) {
            return (0);
        }
        if (!moreterms(tokens, str3, cur)) {
            return (0);
        }
    }
    strcat(str2, str1);
    strcat(str2, str3);
    strcpy(str, str2);
    return (1);
}

int factor(char tokens[100][100], char *str, int *cur) {
    char str1[1000];
    char copiedExpr[1000];
    char inside[1000];
    str1[0] = '\0';
    trim(tokens[*cur]);
    if (!startsWith("(", tokens[*cur])) {
        strcpy(str, tokens[*cur]);
        strcat(str, " ");
        *cur = *cur + 1;
        return (1);
    } else {
        
        unsigned long length = strlen(tokens[*cur]);
        
        if (tokens[*cur][0] != '(' || tokens[*cur][length - 1] != ')') {
            return 0;
        }
        strcpy(copiedExpr, tokens[*cur]);
        int i = 0;
        for (i = 0; i < length - 2; i++) {
            inside[i] = copiedExpr[i + 1];
        }
        
        inside[i] = '\0';
        int *newNumber = (int *) malloc(1 * sizeof(int));
        *newNumber = 0;

        char newToken[100][100] = {};
        strcpy(newToken[0], inside);
        if (!expr(newToken, str1, newNumber)) {
            return (0);
        }
        
        free(newNumber);

        *cur = *cur + 1;
        strcpy(str, str1);
        return (1);
    }
    
    return (0);
}


int morefactors(char tokens[100][100], char *str, int *cur) {
    char str1[1000], str2[1000], str3[1000];

    str1[0] = str2[0] = str3[0] = '\0';

    if ((strcmp(tokens[*cur], "*") == 0) || (strcmp(tokens[*cur], "/") == 0)) {
        strcpy(str1, tokens[*cur]);
        strcat(str1, " ");
        *cur = *cur + 1;
        if (!factor(tokens, str2, cur)) {
            return (0);
        }
        if (!morefactors(tokens, str3, cur)) {
            return (0);
        }
    }
    strcat(str2, str3);
    strcat(str2, str1);
    strcpy(str, str2);
    return (1);
}


void parseAndCallExpr(char exprStmt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]) { // Takes the operative expression and parses it from +,*,- signs and calls the relevant function.
    char copiedExpr[1000];
    strcpy(copiedExpr, exprStmt);
    trim(copiedExpr);
    removeUnnecessaryPharanthesis(copiedExpr);
    
    char exprList[100][100];
    // reset exprList.
    for(int i = 0;i< 100 ;i++){
        for(int j = 0;j< 100 ;j++){
            exprList[i][j] = '\0';
        }
    }

    char operatorList[100][3];
    // reset operatorList.
    for(int i = 0;i< 100 ;i++){
        for(int j = 0;j< 3 ;j++){
            operatorList[i][j] = '\0';
        }
    }
    int indexOperator = 0;
    char open = '(';
    char close = ')';
    char openBracket = '[';
    char closeBracket = ']';
    int pharanthesisCount = 0; // stores the number of parantheses.
    int bracketCount = 0;
    int numOfOperand = 0;

    for (int i = 0; i < strlen(copiedExpr); i++) {
        if (open == copiedExpr[i]) {
            pharanthesisCount++;
            continue;
        } if (openBracket == copiedExpr[i]) {
            bracketCount++;
            continue;
        }else if (close == copiedExpr[i]) {
            if (pharanthesisCount > 0) {
                pharanthesisCount--;
            } else {
                error(currentLine);
            }
            continue;
        } else if (closeBracket == copiedExpr[i]) {
            if (bracketCount > 0) {
                bracketCount--;
            } else {
                error(currentLine);
            }
            continue;
        } else if ((copiedExpr[i] == '+' || copiedExpr[i] == '-' || copiedExpr[i] == '*') &&
                   pharanthesisCount == 0 && bracketCount == 0 ) { //   5*3  (x*5)/(7+2)
            if (copiedExpr[i] == '+') {
                strcpy(operatorList[indexOperator], "+");
            } else if (copiedExpr[i] == '-') {
                strcpy(operatorList[indexOperator], "-");
            } else if (copiedExpr[i] == '*') {
                strcpy(operatorList[indexOperator], "*");
            }
            copiedExpr[i] = '`';
            indexOperator++;
        }
    }
    if (pharanthesisCount != 0) {
        error(currentLine);
    }
    if (bracketCount != 0) {
        error(currentLine);
    }

    

    int insertionInd = 1; //3
    int indexOp = 0;
    char *operand = strtok(copiedExpr, "`");
    strcpy(exprList[0], operand);
    numOfOperand++;
    while (operand != NULL) {
        operand = strtok(NULL, "`");
        if (operand != NULL) {
            numOfOperand++;
            strcpy(exprList[insertionInd], operatorList[indexOp]);
            strcpy(exprList[insertionInd + 1], operand);
            insertionInd += 2;
            indexOp++;
        }
    }
   
    if (indexOperator + 1 != numOfOperand) {
        error(currentLine);
    }
    for (int i = 0; i < indexOperator + numOfOperand; i++) {
         remove_white_spaces(exprList[i]);
       }
    char str[1000];
    str[0] = '\0';
    int *cur = (int *) malloc(1 * sizeof(int));
    *cur = 0;
    expr(exprList, str, cur);
    free(cur);
    
    char arr[1200];
    char content[30][100];
    char *token = strtok(str, " ");
    strcpy(content[0], token);
    int index = 1;
    while (token != NULL) {
        token = strtok(NULL, " ");
        if (token != NULL) {
            strcpy(content[index], token);

            index++;
        }
    }
    
    
    handleStack(content, arr, index, currentLine, rightPart, craftedArray);
    

}
int isExpression(char exprStmt[], char rightPart[1200], int currentLine) {  // checks the statements has the operator in the outermost layer.
    char copiedExpr[1000];
    strcpy(copiedExpr, exprStmt);
    trim(copiedExpr);
    

    char open = '(';
    char close = ')';
    char openBracket = '[';
    char closeBracket = ']';
    int isInside = 0; // shows whether [ or ] character is inside an expression or not.
    int isInsideBracket = 0;

    for (int i = 0; i < strlen(copiedExpr); i++) {
        if (open == copiedExpr[i] || openBracket == copiedExpr[i]) {
            if (open == copiedExpr[i])
                isInside++;
            else
                isInsideBracket++;
            continue;
        } else if (close == copiedExpr[i] || closeBracket == copiedExpr[i]) {
            if (isInside > 0 && close == copiedExpr[i] ) {
                isInside--;
            } else if (isInsideBracket > 0 && closeBracket == copiedExpr[i] ){
                isInsideBracket--;
            }
            else {
                
                error(currentLine);
            }
            continue;
        }
        if ((copiedExpr[i] == '+' || copiedExpr[i] == '-' || copiedExpr[i] == '*') && isInside == 0 && isInsideBracket == 0) {
            return 1;
        }
    }
    return 0;
}

void removeUnnecessaryPharanthesis(char copiedExpr[]) {  // removes the excess parentheses (for example ((1+2))).

    char open = '(';
    char close = ')';
    while (strlen(copiedExpr) > 2) {
        
        int error = 0;
        int unnecessaryPharanthesis = 0;
        for (int i = 1; i < strlen(copiedExpr) - 1; i++) {
            if (open == copiedExpr[i]) {
                unnecessaryPharanthesis++;
                continue;
            } else if (close == copiedExpr[i]) {
                if (unnecessaryPharanthesis > 0) {
                    unnecessaryPharanthesis--;
                } else {
                    error = 1;
                    break;
                }
            }
        }
        if (error == 0 && unnecessaryPharanthesis == 0 && copiedExpr[0] == open &&
            copiedExpr[strlen(copiedExpr) - 1] == close) {
            copiedExpr[0] = ' ';
            copiedExpr[strlen(copiedExpr) - 1] = ' ';
            trim(copiedExpr);
        } else {
            break;
        }
        
    }
}

// add -1 to each dimension of matrix or vector. Because dimensions are start from 1.
void handleIndexing(char content[]){

    int index = 0;

    while(content[index] != '\0'){
        if(content[index] == ']'){
            char copiedContent[1000];
            for(int j = 0;j <index;j++){
                copiedContent[j] = content[j];
            }
            
            copiedContent[index] = '-';
            copiedContent[index+1] = '1';
            copiedContent[index+2] = ']';
            int temp = 0;
            for(int k = index + 1;content[k] != '\0';k++){
                copiedContent[k+2] = content[k];
                temp = k;
            }
            if(temp != 0){
                copiedContent[temp+3] = '\0';
            }else{
                copiedContent[index+3] = '\0';
                strcpy(content,copiedContent);
                return;
            }

            strcpy(content,copiedContent);

            index+=3;
        }else{
            index ++;
        }
    }
}

// Additionally adds [1] to the end of the vector indexing.
void handleVector(char content[]){

    int index = 0;
    int checker = 0;
    while(content[index] != '\0'){
        if(content[index] == '['){
            if(content[index-1] == ']' || (content[index-1] == ' ' && content[index-2] == ']')){
                checker = 1;
            }else{
                checker = 0;
            }
        }
        if(content[index] == ']'){
            char copiedContent[1000];
            if(content[index+1] == '[' || (content[index+1] == ' ' && content[index+2] == '[') || checker == 1){
                index ++;
                continue;
            }
            for(int j = 0;j <= index;j++){
                copiedContent[j] = content[j];
            }
            
            copiedContent[index+1] = '[';
            copiedContent[index+2] = '1';
            copiedContent[index+3] = ']';
            int temp = 0;
            for(int k = index + 1;content[k] != '\0';k++){
                copiedContent[k+3] = content[k];
                temp = k;
            }
            if(temp != 0){
                copiedContent[temp+4] = '\0';
            }else{
                copiedContent[index+4] = '\0';
                strcpy(content,copiedContent);
                return;
            }

            strcpy(content,copiedContent);

            index+=4;
        }else{
            index ++;
        }
    }
}


//handles the inside of square brackets. (For example A[1*23+2]).
int array(char varName[], char insideArrayFirst[], char insideArraySecond[],char rightPart[1200], int currentLine, char craftedArray[3][1000]){
    char copyInsideFirst[1000];
    char copyInsideSecond[1000];
    strcpy(copyInsideFirst, insideArrayFirst);
    strcpy(copyInsideSecond, insideArraySecond);
    removeUnnecessaryPharanthesis(copyInsideFirst);
    removeUnnecessaryPharanthesis(copyInsideSecond);
    char craftedInsideFirst[3][1000];
    for(int i = 0;i< 3 ;i++){
        for(int j = 0;j< 1000 ;j++){
            craftedInsideFirst[i][j] = '\0';
        }
    }
    char craftedInsideSecond[3][1000];
    for(int i = 0;i< 3 ;i++){
        for(int j = 0;j< 1000 ;j++){
            craftedInsideSecond[i][j] = '\0';
        }
    }
    char crafted[1000];


    if (isExpression(copyInsideFirst, rightPart, currentLine)) { //checks the statement whether it has operators in the outermost shell
        parseAndCallExpr(copyInsideFirst, rightPart, currentLine, craftedInsideFirst);
    } else if (startsWith("choose", copyInsideFirst)) { // checks whether the statement is in choose function.
        parseAndCallChoose(copyInsideFirst, rightPart, currentLine, craftedInsideFirst);
    } else if (startsWith("sqrt", copyInsideFirst)) { // checks whether the statement is in sqrt function.
        parseAndCallSqrt(copyInsideFirst, rightPart, currentLine, craftedInsideFirst);
    } else if (startsWith("tr", copyInsideFirst)) { // checks whether the statement is in tr function.
        parseAndCallTranspose(copyInsideFirst, rightPart, currentLine, craftedInsideFirst);
    }  else if (strstr(copyInsideFirst,"]")&&strstr(copyInsideFirst,"[")) { // if it is an array statement calls the relevant function.
        parseAndCallArray(copyInsideFirst,rightPart, currentLine, craftedInsideFirst);
    } else if (!((firstDimension(copyInsideFirst, varList, currentIndex) == 0) || isNumber(copyInsideFirst) == 1)) {

        error(currentLine);
    }
    else {
        strcpy(craftedInsideFirst[0], insideArrayFirst);
        strcpy(craftedInsideFirst[1], "0");
        strcpy(craftedInsideFirst[2], "0");
    }

    if (!(strcmp(craftedInsideFirst[1], "0") == 0 && strcmp(craftedInsideFirst[2], "0") == 0)) {
        error(currentLine);
    }

    if (isExpression(copyInsideSecond, rightPart, currentLine)) { //checks the statement whether it has operators in the outermost shell
        parseAndCallExpr(copyInsideSecond, rightPart, currentLine, craftedInsideSecond);
    } else if (startsWith("choose", copyInsideSecond)) { // checks whether the statement is in choose function.
        parseAndCallChoose(copyInsideSecond, rightPart, currentLine, craftedInsideSecond);
    } else if (startsWith("sqrt", copyInsideSecond)) { // checks whether the statement is in sqrt function.
        parseAndCallSqrt(copyInsideSecond, rightPart, currentLine, craftedInsideSecond);
    } else if (startsWith("tr", copyInsideSecond)) { // checks whether the statement is in tr function.
        parseAndCallTranspose(copyInsideSecond, rightPart, currentLine, craftedInsideSecond);
    }  else if (strstr(copyInsideSecond,"]")&&strstr(copyInsideSecond,"[")) { // if it is an array statement calls the relevant function.
        parseAndCallArray(copyInsideSecond,rightPart, currentLine, craftedInsideSecond);
    } else if (!((firstDimension(copyInsideSecond, varList, currentIndex) == 0) || isNumber(copyInsideSecond) == 1)) {

        error(currentLine);
    }
    else {
        strcpy(craftedInsideSecond[0], insideArraySecond);
        strcpy(craftedInsideSecond[1], "0");
        strcpy(craftedInsideSecond[2], "0");
    }

    if (!(strcmp(craftedInsideSecond[1], "0") == 0 && strcmp(craftedInsideSecond[2], "0") == 0)) {
        error(currentLine);
    }

    strcpy(crafted, varName);
    strcat(crafted, "[(int)(");
    strcat(crafted, craftedInsideFirst[0]);
    strcat(crafted, ")]");
    strcat(crafted, "[(int)(");
    strcat(crafted, craftedInsideSecond[0]);
    strcat(crafted, ")]");

    strcpy(craftedArray[0], crafted);
    strcpy(craftedArray[1], "0");
    strcpy(craftedArray[2], "0");
    return 0;
}


void parseAndCallArray(char arrayStmt[], char rightPart[1200], int currentLine, char craftedArray[3][1000]) { // takes the array in the form of "A[x][y]" and parses it to components.
    char copiedExpr[1000];
    strcpy(copiedExpr, arrayStmt);
    trim(copiedExpr);
    removeUnnecessaryPharanthesis(copiedExpr);
    
    char openBracket = '[';
    char closeBracket = ']';
    int isInside = 0;

    for (int i = 0; i < strlen(copiedExpr); i++) {
        if (openBracket == copiedExpr[i]) {
            if (isInside == 0)
                copiedExpr[i] = '`';
            isInside++;
            continue;
        } else if (closeBracket == copiedExpr[i]) {
            if (isInside == 1)
                copiedExpr[i] = '`';
            if (isInside > 0) {
                isInside--;
            } else {
                
                error(currentLine);
            }
            continue;
        }
    }
    

    char* varName = strtok(copiedExpr,"`");
    char* firstDim = strtok(NULL,"`");
    char* secDim = strtok(NULL,"`");


    

    array(varName, firstDim, secDim,rightPart, currentLine, craftedArray);
}



void remove_white_spaces(char *str)
{
    int i = 0, j = 0;
    while (str[i])
    {
        if (str[i] != ' ')
          str[j++] = str[i];
        i++;
    }
    str[j] = '\0';
    
}
