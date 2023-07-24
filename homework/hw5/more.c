/*
Задача 3. Написать программу, работающую подобно команде more, 
которая осуществляет вывод файла на экран порциями по нажатию клавиши <пробел> (можете выбрать любую).
Нажатие <q>  - прервать работу (дальше не выводить файл).
more [-s] [-num]  [+linenum]  файл
Опции:
-s – предписывает заменять последовательность пустых строк одной пустой строкой.
-num   - целое число задает количество выводимых за один раз строк.
Длина одной порции по умолчанию, т.е. в том случае, если эта опция не задана, 10 строк.
+linenum  -  начать вывод со строки с номером linenum (целое число >=1)
Порядок опций определен, любая из них (в том числе и все) при запуске программы может отсутствовать.

Примеры вызова: 
> ./my_more -s -6 +10 file    
> ./my_more -16 file       
> ./my_more file
*/
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int more(FILE *file, int copyflag, int numflag, int *linenumflag)
{
    int i, c, flag = 0, counter = *linenumflag;
    /* setting file to the line that was selected by user */
    if(counter > 1){
        while(((c = fgetc(file)) != EOF) && (counter != 1)){
            if(c == '\n')
                counter--;
        }
        if(c == EOF){
            printf("You've reached the EOF!!!\n");
            return 1;
        }
        /*fseek(file, -1, SEEK_CUR);*/
    }
    /* printing the amount of lines that was set by user */
    for(i = 0; i < numflag; i++){
        c = fgetc(file);
        if(copyflag)
            if(c == '\n'){
                printf("\n");
                while((c = fgetc(file)) == 'n')
                    (*linenumflag)++;
            }
        while((c != '\n') && (c != EOF)){
            printf("%c", c);
            c = fgetc(file);
            flag = 1;
        }
        if(flag){
            printf("\n");
            flag = 0;
        }
        if(c == EOF){
            printf("You've reached the EOF!!!\n");
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); //сохранение текущего режима
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // изменение

    FILE *f = NULL;
    char copy[] = "-s";
    char *str = NULL;
    int copyflag = 0, numflag = 0, linenumflag = 0;
    int i, j;
    char c;

    if(argc < 2){
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /* восстановление исходного режима */
        return 0;
    }

    /* opening the file, looking for errors in opening */
    f = fopen(argv[argc-1], "r+");
    if(!f){
        perror(argv[argc-1]);
        return 1;
    }

    /* checking flags */
    for(i = 1; i < argc - 1; i++){
        str = argv[i];
        if(strcmp(str, copy) == 0){
            copyflag = 1;
            /*printf("copyflag = %d\n", copyflag);*/
            continue;
        }
        if(str[0] == '+'){
            for(j = 1; str[j]; j++)
                linenumflag = linenumflag*10 + (str[j]-'0');
            /*printf("linenum = %d\n", linenumflag);*/
            continue;
        }
        if(str[0] == '-')
            for(j = 1; str[j]; j++){
                numflag = numflag*10 + (str[j]-'0');
            /*printf("num = %d\n", numflag);*/
            continue;
        }
    }

    /* default settings */
    if(!numflag)
        numflag = 10;
    if(!linenumflag)
        linenumflag = 1;

    printf("If you want to start printing file press any button except 'q': ");
    while((c = getchar()) != 'q'){
        printf("\n");
        /* applying "more" function and checking for EOF */
        if(more(f, copyflag, numflag, &linenumflag)){
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /* восстановление исходного режима */
            fclose(f);
            return 0;
        }
    }
    printf("\n");
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /* восстановление исходного режима */
    fclose(f);
    return 0;
}
