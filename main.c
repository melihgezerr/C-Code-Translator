#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "function.h"
#include <math.h>

char varList[300][50]; // stores all the variables.
int currentIndex = 0; // length of the varList.
char* outputFileName;

int main(int argc, char *argv[]) {

    FILE *inFile; //input file.
    FILE *outputFile; // output file.
    FILE *tempFile; // operation.c file that contains expression functions.
    char line[255]; // each line in the input file.
    char tempFileLine[255];
    char *finding;
    int isNested;
    isNested=0;
    int currentLine = 1; // stores the current line in the input file.

    if(argc == 1){
        printf("Pls provide a file as argument\n");
        exit(1);
    }

    // Opening file in reading mode.
    inFile = fopen(argv[1], "r");
    tempFile = fopen("operation.c", "r");
    if (NULL == tempFile) {
        printf("operation.c file can't be opened \n");
    }


    outputFileName = strtok(argv[1], ".");
    strcat(outputFileName, ".c");

    // Opening file in reading and writing mode.
    outputFile = fopen(outputFileName, "w+");
    // Gets each line in the operation.c file and write it to output file.
    while (fgets(tempFileLine, 255, tempFile) != NULL) {
        fputs(tempFileLine,outputFile);
    }
    fclose(tempFile);
    fputs("int main()\n{\n", outputFile);
    if (NULL == inFile) {
        printf("file can't be opened \n");
    }

    // Gets each line in the input file and do the necessary calculations.
    while (fgets(line, 255, inFile) != NULL) {
        trim(line);
        //check if there is a comment
        if (startsWith("#", line)) {
            currentLine++;
            continue;
        }
        // check whether line is an assignment statement.
        finding = strstr(line, "=");
        if (finding) {
            
            char *leftPart = strtok(line, "="); //take the part before =
            char *rightPart = strtok(NULL, "="); //take the part after =
            trim(leftPart);
            trim(rightPart);
            // handle vector or matrix initialization.
             if(firstDimension(leftPart,varList,currentIndex) >= 1 && secondDimension(leftPart,varList,currentIndex) >= 1 && strstr(rightPart,"{") && strstr(rightPart,"}")){
                int firstDim =firstDimension(leftPart,varList,currentIndex);
                int secondDim =secondDimension(leftPart,varList,currentIndex);
                char copiedLeftPart[30];
                char num3[20];
                sprintf(num3, "%d", currentLine );
                strcpy(copiedLeftPart,leftPart);
                strcat(copiedLeftPart,"q");
                strcat(copiedLeftPart,"w");
                strcat(copiedLeftPart,num3);
                assignArray(rightPart, firstDimension(leftPart, varList, currentIndex),
                            secondDimension(leftPart, varList, currentIndex), currentLine); 
                char num1[20];
                sprintf(num1, "%d", firstDim );
                char num2[20];
                sprintf(num2, "%d", secondDim );
                char writtenExpr[200];
                strcpy(writtenExpr,"float ");
                strcat(writtenExpr,copiedLeftPart);
                strcat(writtenExpr,"[");
                strcat(writtenExpr,num1);
                strcat(writtenExpr,"]");
                strcat(writtenExpr,"[");
                strcat(writtenExpr,num2);
                strcat(writtenExpr,"]");
                strcat(writtenExpr," = ");
                strcat(writtenExpr,rightPart);
                strcat(writtenExpr, ";\n");
                fputs(writtenExpr, outputFile);
                strcpy(writtenExpr,"for(int i = 0;i < ");
                strcat(writtenExpr,num1);
                strcat(writtenExpr,";i++){\n");
                fputs(writtenExpr, outputFile);
                strcpy(writtenExpr,"for(int j = 0;j < ");
                strcat(writtenExpr,num2);
                strcat(writtenExpr,";j++){\n");
                fputs(writtenExpr, outputFile);
                strcpy(writtenExpr,leftPart);
                strcat(writtenExpr,"[i][j] =");
                strcat(writtenExpr,copiedLeftPart);
                strcat(writtenExpr,"[i][j];\n");
                fputs(writtenExpr, outputFile);
                fputs("}\n", outputFile);
                fputs("}\n", outputFile);
                currentLine++;
                continue;
                // handle if left and right part are variable that is defined before.
            }else if(firstDimension(leftPart, varList, currentIndex) == firstDimension(rightPart, varList, currentIndex) &&
                     secondDimension(leftPart, varList, currentIndex) == secondDimension(rightPart, varList, currentIndex) && firstDimension(leftPart, varList, currentIndex) >=1){
                int firstDim =firstDimension(leftPart,varList,currentIndex);
                int secondDim =secondDimension(leftPart,varList,currentIndex);
                char num1[20];
                sprintf(num1, "%d", firstDim );
                char num2[20];
                sprintf(num2, "%d", secondDim );
                char writtenExpr[200];
                strcpy(writtenExpr,"for(int i = 0;i < ");
                strcat(writtenExpr,num1);
                strcat(writtenExpr,";i++){\n");
                fputs(writtenExpr, outputFile);
                strcpy(writtenExpr,"for(int j = 0;j < ");
                strcat(writtenExpr,num2);
                strcat(writtenExpr,";j++){\n");
                fputs(writtenExpr, outputFile);
                strcpy(writtenExpr,leftPart);
                strcat(writtenExpr,"[i][j] =");
                strcat(writtenExpr,rightPart);
                strcat(writtenExpr,"[i][j];\n");
                fputs(writtenExpr, outputFile);
                fputs("}\n", outputFile);
                fputs("}\n", outputFile);
                currentLine++;
                continue;
                
            }
            char copiedRight[1200];
            strcpy(copiedRight,rightPart);
            handleAssignment(leftPart, copiedRight,currentLine); // call handleAssignment function that handles almost all calculations about assignment.
            char finalString[1400];
            strcpy(finalString,leftPart);
            strcat(finalString," = ");
            strcat(finalString,copiedRight);
            strcat(finalString, ";\n");
            fputs(finalString, outputFile);
            currentLine++;
            continue;
        }

        // scalar definition part.
        if (startsWith("scalar", line)) {
            char varName[40];
            int index = 0;
            // gets the variable name after the scalar word.
            while(line[index] != 'r'){
                index ++;
            }
            int j = 0;;
            for(int i = index + 1;line[i] != '\0';i++){
                varName[j] = line[i];
                j++;
            }
            varName[j] = '\0';
            removeNewLine(varName);
            trim(varName);
            char str[80];
            strcpy(str, "float ");
            strcat(str, varName);
            strcat(str, "=0;\n");
            fputs(str, outputFile);
            char tempArray[50];
            strcpy(tempArray,varName);
            strcat(tempArray," ");
            strcat(tempArray,"0 ");
            strcat(tempArray,"0");
            strcpy(varList[currentIndex],tempArray); // add varName to varList.
            currentLine++;
            currentIndex++;
            continue;
        }

        // vector definition part.
        if (startsWith("vector", line)) {
            
            char varName[40];
            int index = 0;
            // gets the variable name after the vector word.
            while(line[index] != 'r'){
                index ++;
            }
            int j = 0;;
            for(int i = index + 1;line[i] != '\0';i++){
                varName[j] = line[i];
                j++;
            }
            varName[j] = '\0';
            removeNewLine(varName);
            trim(varName);
            
            char str[80];
            
            char* identifierName = strtok(varName,"[");
            trim(identifierName);
            char* identifierLength = strtok(NULL,"]");
            trim(identifierLength);
            
            char writtenExpr[150]; // create a vector with calloc.(At the end of the program it will be freed).
            strcpy(writtenExpr,"float** ");
            strcat(writtenExpr,identifierName);
            strcat(writtenExpr," = ");
            strcat(writtenExpr,"(float**)calloc(");
            strcat(writtenExpr,identifierLength);
            strcat(writtenExpr,", sizeof(float*));\n");
            fputs(writtenExpr, outputFile);
            strcpy(writtenExpr,"for (int i = 0; i < ");
            strcat(writtenExpr,identifierLength);
            strcat(writtenExpr,"; i++){\n");
            fputs(writtenExpr, outputFile);
            strcpy(writtenExpr,identifierName);
            strcat(writtenExpr,"[i] = (float*)calloc(");
            strcat(writtenExpr,"1");
            strcat(writtenExpr,", sizeof(float));\n");
            fputs(writtenExpr, outputFile);
            fputs("}\n", outputFile);

            char tempArray[50];
            
            strcpy(tempArray,identifierName);
            strcat(tempArray," ");
            strcat(tempArray,identifierLength);
            strcat(tempArray," 1");
            strcpy(varList[currentIndex],tempArray); // add varName to varList.
            currentLine++;
            currentIndex++;
            continue;
        }

        // matrix definition part.
        if (startsWith("matrix", line)) {

            
            char varName[40];
            int index = 0;
            // gets the variable name after the matrix word.
            while(line[index] != 'x'){
                index ++;
            }
            int j = 0;;
            for(int i = index + 1;line[i] != '\0';i++){
                varName[j] = line[i];
                j++;
            }
            varName[j] = '\0';
            
            removeNewLine(varName);
            trim(varName);
            char copiedVarName[48];

            strcpy(copiedVarName,varName);

            char* identifierName = strtok(copiedVarName,"[");

            char* identifierLength1 = strtok(NULL,",");
            char* identifierLength2 = strtok(NULL,"]");
            trim(identifierName);
            trim(identifierLength1);
            trim(identifierLength2);

            char writtenExpr[150]; // create a matrix with calloc.(At the end of the program it will be freed).
            strcpy(writtenExpr,"float** ");
            strcat(writtenExpr,identifierName);
            strcat(writtenExpr," = ");
            strcat(writtenExpr,"(float**)calloc(");
            strcat(writtenExpr,identifierLength1);
            strcat(writtenExpr,", sizeof(float*));\n");
            fputs(writtenExpr, outputFile);
            strcpy(writtenExpr,"for (int i = 0; i < ");
            strcat(writtenExpr,identifierLength1);
            strcat(writtenExpr,"; i++){\n");
            fputs(writtenExpr, outputFile);
            strcpy(writtenExpr,identifierName);
            strcat(writtenExpr,"[i] = (float*)calloc(");
            strcat(writtenExpr,identifierLength2);
            strcat(writtenExpr,", sizeof(float));\n");
            fputs(writtenExpr, outputFile);
            fputs("}\n", outputFile);

            char tempArray[50]; // add varName to varList.
            
            strcpy(tempArray,identifierName);
            strcat(tempArray," ");
            strcat(tempArray,identifierLength1);
            strcat(tempArray," ");
            strcat(tempArray,identifierLength2);
            strcpy(varList[currentIndex],tempArray);
            currentLine++;
            currentIndex++;
            continue;
        }
 
        //for definition part.
        if (startsWith("for", line)) {
            int nested = 0; // shows if there are more than one for loop (inner loop).
            if (strstr(line,","))
                nested=1;
            char forString[500];
            int indOfLastClosePharantheses = 0; // stores the index of last parantheses.
            for (int i =0;i< strlen(line);i++) {
                if (line[i]==')')
                    indOfLastClosePharantheses = i;
            }
            line[indOfLastClosePharantheses] = '`';
            char *forLoopPart = strtok(line, "("); // to take a index of "(".
            forLoopPart = strtok(NULL,"`");
            trim(forLoopPart); // extracts the part inside the parentheses.
            

            char* forStr = "!?forLoopCheck!?"; // arbitrary string to show that current line includes for.

            if (nested) {  //checking for nested-loop.
                
                isNested++;

                char *firstVar = strtok(forLoopPart, ",");
                char *middlePart = strtok(NULL, ",");
                char *secCommands = strtok(NULL, ",");
                trim(firstVar);
                trim(middlePart);
                trim(secCommands);

                // parses before and after the in keyword.
                char copymiddlePart[80];
                strcpy(copymiddlePart,middlePart);
                char *secVar = strtok(copymiddlePart, "in");
                char *firstCommands = strtok(NULL, "in"); // entries of first variable.
                trim(secVar);
                trim(firstCommands);

                // takes each element according to : character.
                char *firstof1stvar = strtok(firstCommands, ":");
                char *secof1stvar = strtok(NULL, ":");
                char *thirdof1stvar = strtok(NULL, ":");
                trim(firstof1stvar);
                trim(secof1stvar);
                trim(thirdof1stvar);
                
                // takes each element according to : character.
                char *firstof2ndvar = strtok(secCommands, ":");
                char *secof2ndvar = strtok(NULL, ":");
                char *thirdof2ndvar = strtok(NULL, ":");
                trim(firstof2ndvar);
                trim(secof2ndvar);
                trim(thirdof2ndvar);


                char firstof1stvarr[1200];
                char secof1stvarr[1200];
                char thirdof1stvarr[1200];

                char firstof2ndvarr[1200];
                char secof2ndvarr[1200];
                char thirdof2ndvarr[1200];

                strcpy(firstof1stvarr,firstof1stvar);
                strcpy(secof1stvarr,secof1stvar);
                strcpy(thirdof1stvarr,thirdof1stvar);

                strcpy(firstof2ndvarr,firstof2ndvar);
                strcpy(secof2ndvarr,secof2ndvar);
                strcpy(thirdof2ndvarr,thirdof2ndvar);

            
                handleAssignment(forStr,firstof1stvarr,currentLine);
                handleAssignment(forStr,secof1stvarr,currentLine);
                handleAssignment(forStr,thirdof1stvarr,currentLine);

                handleAssignment(forStr,firstof2ndvarr,currentLine);
                handleAssignment(forStr,secof2ndvarr,currentLine);
                handleAssignment(forStr,thirdof2ndvarr,currentLine);


                strcpy(forString,"for(int ");
                strcat(forString, firstVar);
                strcat(forString, "=");
                strcat(forString, firstof1stvarr);
                strcat(forString, "; ");
                strcat(forString, firstVar);
                strcat(forString, "<=");
                strcat(forString, secof1stvarr);
                strcat(forString, "; ");
                strcat(forString, firstVar);
                strcat(forString, "+=");
                strcat(forString, thirdof1stvarr);
                strcat(forString, ") {\n ");
                strcat(forString, "\t");

                strcat(forString,"for(int ");
                strcat(forString, secVar);
                strcat(forString, "=");
                strcat(forString, firstof2ndvarr);
                strcat(forString, "; ");
                strcat(forString, secVar);
                strcat(forString, "<=");
                strcat(forString, secof2ndvarr);
                strcat(forString, "; ");
                strcat(forString, secVar);
                strcat(forString, "+=");
                strcat(forString, thirdof2ndvarr);
                strcat(forString, ") {\n ");
                strcat(forString, "\t");
                fputs(forString, outputFile);
                currentLine++;
                continue;
            }
            
            // part for inner for.
            char firstof1stvarr[1200];
            char secof1stvarr[1200];
            char thirdof1stvarr[1200];
            
            
            // takes each element according to : character.
            char* leftpart = strtok(forLoopPart,":");  // left of the first ":".
            char* expr2 = strtok(NULL,":");
            char* expr3 = strtok(NULL,":");
            char* varr = strtok(leftpart," ");
            char* leftpart2 = strtok(NULL," ");
            char* expr1 = strtok(NULL," ");

            trim(expr1);
            trim(expr2);
            trim(expr3);

            strcpy(firstof1stvarr,expr1);
            strcpy(secof1stvarr,expr2);
            strcpy(thirdof1stvarr,expr3);

            handleAssignment(forStr,firstof1stvarr,currentLine); // call handleAssignment function to check syntax and understand the type.
            handleAssignment(forStr,secof1stvarr,currentLine); // call handleAssignment function to check syntax and understand the type.
            handleAssignment(forStr,thirdof1stvarr,currentLine); // call handleAssignment function to check syntax and understand the type.

            strcpy(forString,"for(int ");
            strcat(forString, varr);
            strcat(forString, "=");
            strcat(forString, firstof1stvarr);
            strcat(forString, "; ");
            strcat(forString, varr);
            strcat(forString, "<=");
            strcat(forString, secof1stvarr);
            strcat(forString, "; ");
            strcat(forString, varr);
            strcat(forString, "+=");
            strcat(forString, thirdof1stvarr);
            strcat(forString, ") {\n ");
            fputs(forString, outputFile);
            currentLine++;
            continue;
        }
        if ((strcmp(line, "}")) == 0) {
            fputs("}\n", outputFile);
            if (isNested>0) {
                isNested--;
                fputs("}\n", outputFile);
            }
            currentLine++;
            continue;
        }

        //print part.
        if (startsWith("print(", line)) {
            char copiedLine[255];
            strcpy(copiedLine,line);
            char inside[250];
            int printIndex = 0;
            // get the part inside print function.
            while(line[printIndex] != '\0'){
                if(line[printIndex] == '('){
                    int tempPrintIndex = 0;
                    printIndex++;
                    while(!(line[printIndex] == ')' && line[printIndex+1] == '\0')){
                        inside[tempPrintIndex] = line[printIndex];
                        printIndex++;
                        tempPrintIndex++;
                    }
                    inside[tempPrintIndex] = '\0';
                        break;
                }
                printIndex++;
            }
            
            
            
            
            trim(inside);
            
            
            // if it is scalar it goes to this if.
            if(firstDimension(inside,varList,currentIndex) == 0 && secondDimension(inside,varList,currentIndex) == 0){
                
                char str[150];
                strcpy(str, "if (fabsf(roundf(");
                strcat(str,inside);
                strcat(str,") - ");
                strcat(str,inside);
                strcat(str,") <= 0.00001f) {\n");
                fputs(str, outputFile);
                strcpy(str, "printf(\"%d\\n\",(int)");
                strcat(str,inside);
                strcat(str,");\n");
                fputs(str, outputFile);
                strcpy(str, "  } else {\n");
                fputs(str, outputFile);
                strcpy(str, "printf(\"%f\\n\",");
                strcat(str,inside);
                strcat(str,");\n");
                fputs(str, outputFile);
                fputs("}\n",outputFile);
                currentLine++;
                continue;
                // if it is matrix or vector it goes to this if.
            }else if(firstDimension(inside,varList,currentIndex) >= 1 && secondDimension(inside,varList,currentIndex) >=1 && !strstr(inside,"[")){
                
                char str[250];
                int firstDim =firstDimension(inside,varList,currentIndex);
                int secondDim =secondDimension(inside,varList,currentIndex);
                char num1[20];
                char num2[20];
                sprintf(num1, "%d", firstDim );
                sprintf(num2, "%d", secondDim );
                strcpy(str,"for(int i = 0;i < ");
                strcat(str,num1);
                strcat(str,";i++){\n");
                fputs(str, outputFile);
                strcpy(str,"for(int j = 0;j < ");
                strcat(str,num2);
                strcat(str,";j++){\n");
                fputs(str, outputFile);
                strcpy(str, "if (fabsf(roundf(");
                strcat(str,inside);
                strcat(str,"[i][j] ) - ");
                strcat(str,inside);
                strcat(str,"[i][j] ) <= 0.00001f) {\n"); // handle error at most 0.00001 precision.
                fputs(str, outputFile);
                strcpy(str, "printf(\"%d \",(int)");
                strcat(str,inside);
                strcat(str,"[i][j]");
                strcat(str,");\n");
                fputs(str, outputFile);
                strcpy(str, "  } else {\n");
                fputs(str, outputFile);
                strcpy(str, "printf(\"%f \",");
                strcat(str,inside);
                strcat(str,"[i][j]");
                strcat(str,");\n");
                fputs(str, outputFile);
                fputs("}\n", outputFile);
                fputs("}\n", outputFile);
                fputs("printf(\"\\n\");\n",outputFile);
                fputs("}\n",outputFile);
                currentLine++;
                continue;
            }else{
                // if inside print contains [ or ] it goes to this part.
                char copiedInside[250];
                char temporRightSide[1200];
                char printCraftedArray[3][1000];
                strcpy(copiedInside,inside);
                handleVector(copiedInside); // adjust vector indexing.
                parseAndCallArray(copiedInside,temporRightSide,currentLine,printCraftedArray);
                
                if(strcmp(printCraftedArray[1],"0") != 0 || strcmp(printCraftedArray[2],"0") != 0){
                    error(currentLine);
                }
                char tempCopiedInside[250];
                strcpy(tempCopiedInside,copiedInside);
                char* varName = strtok(tempCopiedInside,"["); // get varName before [.
                trim(varName);
                
                
                strcpy(copiedInside,printCraftedArray[0]);
                
                
                
                int top = 0; // stores the number of [ character.
                int down = 0; // stores the number of ] character.
                int counter = 0;
                while(copiedInside[counter] != '\0'){
                    if(copiedInside[counter] == '['){
                        top++;
                    }else if(copiedInside[counter]  == ']'){
                        down++;
                    }
                    counter++;
                }
                handleIndexing(copiedInside); // add -1 to each index because it starts from 1.

                
                
                if(top != down){
                    error(currentLine);
                }
                
                // if there are two [ and ] character then program goes this if.
                if(top == 2 && down == 2 && firstDimension(varName,varList,currentIndex) >= 1 && secondDimension(varName,varList,currentIndex) >= 1){
                    
                    char str[350];
                    strcpy(str, "if (fabsf(roundf(");
                    strcat(str,copiedInside);
                    strcat(str,") - ");
                    strcat(str,copiedInside);
                    strcat(str,") <= 0.00001f) {\n");
                    fputs(str, outputFile);
                    strcpy(str, "printf(\"%d\\n\",(int)");
                    strcat(str,copiedInside);
                    strcat(str,");\n");
                    fputs(str, outputFile);
                    strcpy(str, "  } else {\n");
                    fputs(str, outputFile);
                    strcpy(str, "printf(\"%f\\n\",");
                    strcat(str,copiedInside);
                    strcat(str,");\n");
                    fputs(str, outputFile);
                    fputs("}\n",outputFile);
                    currentLine++;
                    continue;
                    // if there are one [ and ] character then program goes this if.
                }else  if(top == 1 && down == 1 && firstDimension(varName,varList,currentIndex) >= 1 && secondDimension(varName,varList,currentIndex) >= 1){

                    char str[150];
                    int dimension =secondDimension(varName,varList,currentIndex);

                    char num[20];
                    sprintf(num, "%d", dimension );
                    strcpy(str,"for(int i = 0;i < ");
                    strcat(str,num);
                    strcat(str,";i++){\n");
                    fputs(str, outputFile);
                    strcpy(str, "if (fabsf(roundf(");
                    strcat(str,copiedInside);
                    strcat(str,"[i] ) - ");
                    strcat(str,copiedInside);
                    strcat(str,"[i] ) <= 0.00001f) {\n");
                    fputs(str, outputFile);
                    strcpy(str, "printf(\"%d\\n\",(int)");
                    strcat(str,copiedInside);
                    strcat(str,"[i]");
                    strcat(str,");\n");
                    fputs(str, outputFile);
                    strcpy(str, "  } else {\n");
                    fputs(str, outputFile);
                    strcpy(str, "printf(\"%f\\n\",");
                    strcat(str,copiedInside);
                    strcat(str,"[i]");
                    strcat(str,");\n");
                    fputs(str, outputFile);
                    fputs("}\n",outputFile);
                    fputs("}\n",outputFile);
                    currentLine++;
                    continue;

                }


            }
            currentLine++;
            continue;
        }

        //printsep part
        if (startsWith("printsep(", line)) {
            fputs("printf(\"------------\\n\");\n", outputFile); // write 12 hyphen.
            currentLine++;
            continue;
        }
        currentLine++;
    }
   
    
    
    // free all the variables to avoid memory leak.
    // Get the variables from varList.
    for(int i = 0;i < currentIndex;i++){

            char copiedVarList[50];
            strcpy(copiedVarList,varList[i]);
            char* pointer = copiedVarList;
            char* varName = strtok(pointer," ");
            char* firstDim = strtok(NULL," ");
            char* secondDim = strtok(NULL," ");

            if(firstDimension(varName,varList,currentIndex) >= 1 && secondDimension(varName,varList,currentIndex) >= 1){

                char writtenExpr[150];
                strcpy(writtenExpr,"for (int i = 0; i < ");
                strcat(writtenExpr,firstDim);
                strcat(writtenExpr,"; i++){\n");
                fputs(writtenExpr, outputFile);
                strcpy(writtenExpr,"free(");
                strcat(writtenExpr,varName);
                strcat(writtenExpr,"[i]);\n");
                fputs(writtenExpr, outputFile);
                fputs("}\n", outputFile);
                strcpy(writtenExpr,"free(");
                strcat(writtenExpr,varName);
                strcat(writtenExpr,");\n");
                fputs(writtenExpr, outputFile);




            }

        }
    fputs("return 0; \n", outputFile);
    fputs("} \n", outputFile);
    fclose(outputFile);
    fclose(inFile);

    return 0;
}
