/*
3. Написать функцию, которая заполняет массив размерности MxN числами от 1 до MxN, расположив их по спирали, 
закрученной по часовой стрелке, так, как показано в примере.

Пример: на входе 4 4

Результат:  
           1  2  3  4
           12 13 14 5
           11 16 15 6
           10 9  8  7

В main():

На стандартном потоке ввода задаются два целых числа M и N ( 1<=M, N <=256), 
создается и заполняется массив. Массив вывести на экран.
*/
#include <stdio.h>
#include <stdlib.h>

void spiral_matrix(int m, int n, int *a)
{
    int i, k = 0, l = 0, counter = 1, ind, size;
    /*
        k - индекс начала строки
        l - индекс начала столбца
        size - кол-во столбцов
    */
    size = n;
    ind = 0;
    while ((k < m) && (l < n)) {
        /* Первая строка из оставшихся */
        for (i = l; i < n; i++) {
            ind = size*k + i;
            a[ind] = counter;
            counter++;
        }
        k++;
 
        /* Последний столбец из оставшихся */
        for (i = k; i < m; i++) {
            ind = size*i + (n - 1);
            a[ind] = counter;
            counter++;
        }
        n--;
 
        /* Последняя строка из оставшихся */
        if (k < m) {
            for (i = n - 1; i >= l; i--) {
                ind = size*(m - 1) + i;
                a[ind] = counter;
                counter++;
            }
            m--;
        }
 
        /* Первый столбец из оставшихся */
        if (l < n) {
            for (i = m - 1; i >= k; i--) {
                ind = size*i + l;
                a[ind] = counter;
                counter++;
            }
            l++;
        }
    }
}

int main()
{
    int m, n, i, j, k;
    int *a = NULL;
    printf("Enter number of rows: ");
    scanf("%d", &m);
    printf("Enter number of columns: ");
    scanf("%d", &n);
    a = malloc(sizeof(int)*m*n);
    k = 1;
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            a[n*i + j] = k;
            k++;
        }
    }
    printf("\nThe given matrix is \n\n");
    for (i = 0; i < m; i++){
        for (j = 0; j < n; j++){
            printf("\t%d", a[n*i + j]);
        }
        printf("\n\n");
    }
    spiral_matrix(m, n, a);
    printf("\nNEW matrix is \n\n");
    for (i = 0; i < m; ++i){
        for (j = 0; j < n; ++j){
            printf("\t%d", a[n*i + j]);
        }
        printf("\n\n");
    }
    free(a);
    return 0;
}