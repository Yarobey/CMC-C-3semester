/*
4. Поля Эллипсиса - написать генератор случайной карты (все тот же двумерных массив char), 
который первым и вторым аргументом получает размеры, 
а третим строку с объектами генерации, 
а затем для каждого объекта идет желаемое число на карте.

Пример:
ellipsis_map(8,4,"%t%r%w%h", 6, 4, 10, 2)
// t - это деревья |
// r - камни O
// w - вода ~
// h - домики A
// _ - земля или ландшафт по умолчанию
...
~~~~~_||
~~OOO_A_
_|_A_|~|
___~~|_O

Подсказка - создайте массив с элементами по умолчанию,
а затем заполняйте его объектами; элементарно реализовать с rand,
но было бы круто, если бы вероятность выбора клетки, 
зависела от соседей (см wavefunction collapse); набор объектов и вид выбираете сами.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

void ellipsis_map(char *ellipsis, int m, int n, char *str, int counter, ...)
{
    int i, j, elem, sum = 0;
    int counters[counter];

    va_list args;
    va_start(args, counter);
    for(i = 0; i < counter; i++)
        counters[i] = va_arg(args, int);

    for(i = 0; i < counter; i++)
        sum += counters[i];

    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            if(sum){
                again:{}
                elem = rand() % (counter+1);
                if(elem == counter)
                    continue;
                if(counters[elem] == 0)
                    goto again;
                ellipsis[i*n + j] = str[elem];
                counters[elem]--;
                sum--;
            }else
                goto finish;
        }
    }

finish:{}
    va_end(args);
    return;
}

void print_ellipsis(char *ellipsis, int m, int n)
{
    int i, j;
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++)
            printf("%c", ellipsis[i*n + j]);
        printf("\n");
    }
    return;
}

int main(int argc, char **argv)
{
    char *str = NULL, *ellipsis = NULL;
    int m, n, i, j, counter = 0;
    int elem1, elem2, elem3, elem4;

    elem1 = strtol(argv[4], NULL, 10);
    elem2 = strtol(argv[5], NULL, 10);
    elem3 = strtol(argv[6], NULL, 10);
    elem4 = strtol(argv[7], NULL, 10);

    srand(time(NULL));

    if(argc == 1){
        printf("ERROR: too few arguments!\n");
        goto end;
    }

    m = strtol(argv[1], NULL, 10);
    n = strtol(argv[2], NULL, 10);
    ellipsis = malloc(m*n*sizeof(char));

    /* initializing ellipsis */
    for(i = 0; i < m; i++)
        for(j = 0; j < n; j++)
            ellipsis[i*n + j] = '#';
    
    /* counting symbols */
    str = argv[3];
    for(i = 0; str[i]; i++)
        counter++;

    if(counter > argc-4){
        printf("ERROR: not enough elements");
        goto end;
    }


    printf("New ellipsis:\n");
    ellipsis_map(ellipsis, m, n, argv[3], counter, elem1, elem2, elem3, elem4);
    print_ellipsis(ellipsis, m, n);

end:{}
    free(ellipsis);
    return 0;
}