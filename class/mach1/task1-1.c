#include <stdio.h>
#include <stdlib.h>
/* abcd efg hijk lmnop qrst uvw xyz */
/* abcd hijk qrst | efg lmnop uvw xyz */

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
    if ((c != EOF) && (c != '\0') && (c != '\n')){
        while((c != EOF) && (c != '\0') && (c != '\n')){
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

char *word_start(char *str) 
{
    int start = 0;
    char *curr;
    if(str[0] != '\0'){
        for(start = 0;str[start] == ' '; start++){
            if(str[start] == '\0')
                break;
        }
    }
    curr = str + start;
    return curr;
}

char *word_end(char *str)  
{
    int end;
    char *curr;
    for(end = 0;(str[end] != ' ') && (str[end] != '\0'); end++)
        {}
    curr = str + end - 1;
    return curr;
}

int word_count(char *str)  
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
    char *str1 = NULL;
    char *str2 = NULL;
    char *str3 = NULL;
    char *start1, *end1, *start2, *end2;
    int len1, len2, i = 0, j = 0, flag = 0, counter1, counter2, lenofword;
    printf("Enter your 1-st string: \n");
    str1 = readln();
    printf("Your 1-st string is: \n");
    printf("%s \n", str1);
    printf("\nEnter your 2-st string: \n");
    str2 = readln();
    printf("Your 2-nd string is: \n");
    printf("%s \n", str2);
    len1 = string_length(str1);
    len2 = string_length(str2);
    counter1 = word_count(str1);
    counter2 = word_count(str2);
    str3 = malloc((len1 + len2)*sizeof(char) + (counter1+counter2-1)*sizeof(char));
    end1 = str1 - 1;
    end2 = str2 - 1; 
    while((counter1 != 0) && (counter2 != 0)){
        switch(flag){
            case(0):
                start1 = word_start(end1+1);
                end1 = word_end(start1);
                lenofword = end1 - start1 + 1;
                for (i = 0; i < lenofword; i++){
                    str3[j] = str1[(start1-str1)+i];
                    j++;
                }    
                counter1--;
                flag = 1;
                break;
            case(1):
                start2 = word_start(end2+1);
                end2 = word_end(start2);
                lenofword = end2 - start2 + 1;
                for (i = 0; i < lenofword; i++){
                    str3[j] = str2[(start2-str2)+i];
                    j++;
                }    
                counter2--;
                flag = 0;
                break;
            default:
                {}
        }
        str3[j] = ' ';
        j++;
    }
    if(counter1 == 0){
        while (counter2 != 0){
            start2 = word_start(end2+1);
            end2 = word_end(start2);
            lenofword = end2 - start2 + 1;
            for (i = 0; i < lenofword; i++){
                str3[j] = str2[(start2-str2)+i];
                j++;
            }    
            counter2--;
            str3[j] = ' ';
            j++;
        }
    }else{
        while (counter1 != 0){
            start1 = word_start(end1+1);
            end1 = word_end(start1);
            lenofword = end1 - start1 + 1;
            for (i = 0; i < lenofword; i++){
                str3[j] = str1[(start1-str1)+i];
                j++;
            }    
            counter1--;
            str3[j] = ' ';
            j++;
        }
    }
    printf("Your 3-d string is: \n");
    printf("%s \n", str3);
    printf("\n");
    free(str1);
    free(str2);
    free(str3);
    return 0;
}
