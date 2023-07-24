/* 
1. Написать функцию, которая будет преобразовывать строку в массив слов этой строки. 
Под словом понимается любая последовательность 
непробельных символов (пробельные символы: ' ', '\t', '\n', '\r').
Количество слов и длины слов не могут быть заданы статически - 
вся необходимая память должна выделяться динамически.
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

char *readln()
{
    enum {size = 4};
    int i = 0, k = 0, c, len = 32;
    char *str = calloc(len, sizeof(char));
    c = getchar();
    if ((c != EOF) && (c != '\0')){
        while((c != EOF) && (c != '\0')){
            if(i == ((8*size - 1) + size*k)){
                len += size;
                str = (char*)realloc(str,len);
                k++;
            }
            str[i] = c;
            c = getchar();
            i++;
        }
    }
    str[i] = '\0';
    printf("\n");
    return str;
}

char *word_start(char *str) /* <- знаю, что нужно по этикету поставить const, но компилятор жаловался */
{
    int start = 0;
    char *curr;
    if(str[0] != '\0'){
        for(start = 0; (str[start] == ' ') || (str[start] == '\t') || (str[start] == '\n') || (str[start] == '\r'); start++){
            if(str[start] == '\0')
                break;
        }
    }
    curr = str + start;
    return curr;
}

char *word_end(char *str)   /* <- знаю, что нужно по этикету поставить const, но компилятор жаловался */
{
    int end;
    char *curr;
    for(end = 0; (str[end] != ' ') && (str[end] != '\0') && (str[end] != '\t') && (str[end] != '\n') && (str[end] != '\r'); end++)
        {}
    curr = str + end - 1;
    return curr;
}

int word_count(char *str)   /* <- знаю, что нужно по этикету поставить const, но компилятор жаловался */
{
    char *start, *end;
    int i, len, counter = 0;
    len = string_length(str);
    end = str - 1;
    for(i = 0; i < len; i++){
        start = word_start(end+1);
        end = word_end(start);
        if(start == (end + 1))
            break;
        i = end - str;
        counter++;
    }
    return counter;
}

int main()
{
    enum {size = 4};
    char *str, *start, *end, *wordc;
    char **word;
    int counter, i = 0, j, lenofword, k, q = 0, len = 32;
    printf("Enter your string: \n");
    str = readln();
    printf("Your string is: \n");
    printf("%s \n", str);
    counter = word_count(str);
    k = counter;
    word = malloc(counter*sizeof(char*));
    for(i = 0; i < counter; i++){
        word[i] = calloc(32, sizeof(char));
    }
    end = str - 1;
    i = 0;
    while(counter != 0){
        start = word_start(end+1);
        end = word_end(start);
        lenofword = end - start + 1;
        for (j = 0; j < lenofword; j++){
            if(j == ((8*size - 1) + size*q)){
                len += size;
                word[i] = (char*)realloc(word[i],len);
                q++;
            }
            wordc = word[i];
            wordc[j] = str[(start-str)+j];
        }
        wordc[j] = '\0';
        len = 32;
        i++;
        counter--;
    }
    printf("\nThe words of your string: \n");
    for(i = 0; i < k; i++){
        printf("word[%d] = %s \n", i, word[i]);
        free(word[i]);
    }
    free(str);
    free(word);
}