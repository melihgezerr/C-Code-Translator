#include <string.h>
#include <stdio.h>
#include <ctype.h>


// check if a string starts with a given substring.
int startsWith(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

// remove new line character (if exists) after the string.
void removeNewLine(char * temp){
char *newline = strchr( temp, '\n' );
if ( newline )
  *newline = 0;
}

// remove all the blankes after and before string.
void trim(char *str)
{
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((unsigned char) str[begin]))
        begin++;

    while ((end >= begin) && isspace((unsigned char) str[end]))
        end--;

    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0'; // Null terminate string.
}
