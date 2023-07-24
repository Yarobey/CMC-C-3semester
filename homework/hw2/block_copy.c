/*
1. Написать функцию char* strcpy_blocks ( char* newstr, const char * oldstr ), 
адреса получены с помощью malloc, т.е. гарантировано выравнивание. 
Копировать строку oldstr (Си-строка) на новое место newstr блоками длиной = размеру double (не посимвольно!).
Написать функцию main(), в которой продемонстрировать работу функции. 
*/

#include <stdio.h>
#include <stdlib.h>

int string_length(const char *str)
{
    int i;
    for(i = 0; str[i]; i++)
        {}
    return i;

}

char* strcpy_blocks (char *newstr, const char *oldstr)
{
    int i, len, len1;
    len = string_length(oldstr);
    len *= sizeof(char);
    len1 = len/sizeof(double);
    for(i = 0; i <= len1; i++){
        *((double *)(newstr + i*sizeof(double))) = *((double *)(oldstr + i*sizeof(double)));
    }
    if(len-len1*sizeof(double)){
        for(i = len1*sizeof(double); oldstr[i]; i++)
            newstr[i] = oldstr[i];
        newstr[i] = 0;
    }else
        newstr[len] = 0;
    return 0;
}


int main()
{
    char src[] = "Hello, friend!!!";
    char dest[] = "                ";
    strcpy_blocks(dest, src);
    printf("New string: %s \n", dest);
    return 0;  
}  