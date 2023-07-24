#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void printArray(const int *arr, int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d \n", arr[i]);
}

void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

/*
void bubbleSort(int *array, int counter)
{
    int i, j;
    for(i = 0; i < counter - 1; i++)
        for(j = 0; j < counter - i - 1; j++)
            if(array[j] > array[j+1])
                swap(&array[j], &array[j + 1]);
}
*/

int main(int argc, char **argv)
{
    FILE *f, *ans;
    int *array = malloc(sizeof(int *));
    int *lengths = malloc(sizeof(int));
    int c, num, j, i = 0, counter = 0, size = 0, len = 0, len2 = 0, sz = 0;

    /* opening the file, looking for errors in opening */
    f = fopen(argv[1], "r+");
    if(!f){
        perror(argv[1]);
        return 1;
    }

    /* reading file, counting the number of \n's and the number of symbols in every line*/
    array[0] = len;
    while((c = fgetc(f)) != EOF){
        if(c == '\n'){
            counter++;
            size += sizeof(int *);
            array = realloc(array, size);
            len++;
            array[counter] = len;

            sz += sizeof(int);
            lengths = realloc(lengths, sz);
            lengths[i] = len2;
            i++;
            len2 = 0;
        }else{
            len++;
            len2++;
        }
    }

    /* checking for \n at the EOF */
    fseek(f, -1, SEEK_END);

    if((c = fgetc(f)) != '\n'){
        counter++;
        lengths[i] = len2;
        fputc('\n', f);
    }
    /*
    printf("Your array1: \n");
    printArray(lengths, counter);
    printf("Your array2: \n");
    printArray(array, counter);
    */
    printf("The number of strings in your file: %d \n", counter);

    /* asking user to enter the number of the line, that he wants to be printed */
again:
    {}
    printf("Enter the number of string that you want to get: ");
    scanf("%d", &num);
    if((num < 0) || (num > counter)){
        printf("The string number is out of range!!!\n");
        goto again;
    }

    /* setting current position to the position of entered line */
    fseek(f, array[num-1], SEEK_SET);
    printf("Your string: \n");
    while((c = fgetc(f)) != '\n')
        printf("%c", c);
    printf("\n");

    /* bubble sorting the array of numbers of symbols in each line*/
    for(i = 0; i < counter - 1; i++)
        for(j = 0; j < counter - i - 1; j++)
            if(lengths[j] > lengths[j+1]){
                swap(&array[j], &array[j + 1]);
                swap(&lengths[j], &lengths[j + 1]);
            }

    /*
    printf("Your 1NEW1 array: \n");
    printArray(lengths, counter);
    printf("Your 2NEW2 array: \n");
    printArray(array, counter);
    */

    /* writing the lines into new file */
    ans = fopen("answer.txt", "w");
    for(i = 0; i < counter; i++){
        fseek(f, array[i], SEEK_SET);
        while((c = fgetc(f)) != '\n')
            fputc(c, ans);
        fputc('\n', ans);
    }
    printf("\nYour file was sorted and copied to file 'answer.txt'\n");
    free(lengths);
    free(array);
    fclose(ans);
    fclose(f);
    return 0;
}