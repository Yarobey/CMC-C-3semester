/*
1. Reversed Copycat - написать функцию, которая будет воспроизводить любые слова кроме cat наоборот
(можно использовать putchar/getchar и puts/gets):
Пример:
cat caught cow -> cat thguac woc
Ввод через аргументы командной строки: ./reversed_copycat "cat caught cow"
*/
#include <stdio.h>  

int string_length(const char *str)
{
    int i;
    for(i = 0; str[i]; i++)
        {}
    return i;
}

int word_length(const char *word)
{
    int i;
    i = 0;
    while((word[i] != '\0') && (word[i] != ' '))
        i++;
    return i;
}


int cmptocat(const char *exmp)
{
    const char cat[] = "cat";
    int i;
    for(i = 0; i < 3; i++)
        if(cat[i] != exmp[i])
            return 1;
    return 0;
}

void reversed(char *src, int len)
{
    char cur;
    int i;
    len--;
    for(i = 0; i < len; i++, len--) {
        cur = src[i];
        src[i] = src[len];
        src[len] = cur;
    }
}

int main(int argc, char **argv)  
{  
    int i, j, len;
    char *string;
    for(j = 1; j < argc; j++){
        string = argv[j];
        i = 0;
        while(string[i] != '\0'){
            len = word_length(&string[i]);
            switch(len) {
                case 3:
                    if(cmptocat(&string[i]) == 0)
                        break;
                default:
                    reversed(&string[i], len);
            }
            i += len;
            if(string[i] != '\0')
                i++;
        }
        printf("%-10s\n", string);
    }
    return 0;  
}  