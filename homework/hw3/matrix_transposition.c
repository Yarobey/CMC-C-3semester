/*
2. Написать функции транспонирования и перемножения матриц.
Для демонстрации работы написать вспомогательные функции инициализации этих матриц: 
инициализация константой и инициализация случайными числами.
Если матрицы имеют некорректный размер,
то программа должна выводить ошибку с перечислением размерностей несовместимых матриц.
*/

#include <stdio.h>
#include <stdlib.h>

int *read_matrix(int* a, int m, int n)
{
    int i, j;
    printf("Enter the elements of matrix: \n");
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            scanf("%d", &a[n*i + j]);
        }
    }
    return a;
}

void matrix_print(const int* a, int m, int n)
{
    int i, j;
    for (i = 0; i < m; i++){
        for (j = 0; j < n; j++){
            printf("\t%d", a[n*i + j]);
        }
        printf("\n\n");
    }
    return;
}

void matrix_multiplication(long long *c, const int*a, int m1, int n1, const int*b, int m2, int n2)
{
    int i, j, k;
    for(i = 0; i < m1; i++){
        for(j = 0; j < n2; j++){
            c[i*n2 + j] = 0;
            for(k = 0; k < m2; k++){
                c[i*n2 + j] += a[i*n1 + k] * b[k*n2 + j];
            }
        }
    }
    return;
}

void matrix_transpose(int* t, int *a, int mt, int nt)
{
    int i, j;
    for(i = 0; i < mt; i++){
        for(j = 0; j < nt; j++){
            t[j*mt + i] = a[i*nt + j];
        }
    }
    return;
}

int main()
{
    int *a = NULL;
    int *b = NULL;
    int *t1 = NULL;
    int *t2 = NULL;
    long long *c = NULL;
    int m1, n1, m2, n2, m3, n3, i, j;
    printf("Enter number of rows in your 1-st matrix: ");
    scanf("%d", &m1);
    printf("Enter number of columns in your 1-st matrix: ");
    scanf("%d", &n1);
    if((m1 <= 0) || (n1 <= 0)){
        printf("Error: sizes of matrix are incorrect!!! \n");
        return 0;
    }
    a = malloc(sizeof(int)*m1*n1);  /* initialization of matrix A with const */
    for(i = 0; i < m1; i++){
        for(j = 0; j < n1; j++){
            a[i*n1 + j] = i*n1 + j + 1;
        }
    }
    /*
    read_matrix(a, m1, n1); 
    */
    printf("\nThe given matrix is \n\n");
    matrix_print(a, m1, n1);
    printf("Enter number of rows in your 2-nd matrix: ");
    scanf("%d", &m2);
    printf("Enter number of columns in your 2-nd matrix: ");
    scanf("%d", &n2);
    if((m2 <= 0) || (n2 <= 0)){
        printf("Error: sizes of matrix are incorrect!!! \n");
        return 0;
    }
    b = malloc(sizeof(int)*m2*n2); /* initialization of matrix B with const */
    for(i = 0; i < m2; i++){
        for(j = 0; j < n2; j++){
            b[i*n2 + j] = rand()%10;
        }
    }
    /*
    read_matrix(b, m2, n2);
    */
    printf("\nThe given matrix is \n\n");
    matrix_print(b, m2, n2);
    if(n1 != m2){
        printf("Error: sizes of matrix are incorrect!!! \n");
        return 0;
    }else{
        c = malloc(sizeof(long long)*m1*n2);
        matrix_multiplication(c, a, m1, n1, b, m2, n2);
        printf("\nThe product of your matrices: \n\n");
        for (i = 0; i < m1; i++){
            for (j = 0; j < n2; j++){
                printf("\t%lld", c[n2*i + j]);
            }
            printf("\n\n");
        }
    }
    printf("Enter number of rows in matrix that you want to transpose: ");
    scanf("%d", &m3);
    printf("Enter number of columns in matrix that you want to transpose: ");
    scanf("%d", &n3);
    if((m3 <= 0) || (n3 <= 0)){
        printf("Error: sizes of matrix are incorrect!!! \n");
        return 0;
    }
    t1 = malloc(sizeof(int)*m3*n3); /* initialization of matrix T with const */
    for(i = 0; i < m3; i++){
        for(j = 0; j < n3; j++){
            t1[i*n3 + j] = rand()%10;
        }
    }
    t2 = malloc(sizeof(int)*n3*m3);
    /*
    read_matrix(t1, m3, n3);
    */
    printf("\nThe given matrix is \n\n");
    matrix_print(t1, m3, n3);
    matrix_transpose(t2, t1, m3, n3);
    printf("\nThe transpose of your matrix is \n\n");
    matrix_print(t2, n3, m3);
    free(a);
    free(b);
    free(c);
    free(t1);
    free(t2);
    return 0;
}
