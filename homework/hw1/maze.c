/*
3. Лабиринт Эллипсиса - написать генератор случайного лабиринта 
размера n x m (в виде двумерного массива типа char).
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* Recursive Backtracking algorithm */
void maze_print(char *maze, int m, int n)
{
    enum {up = 1, left = 4, down = 2, right = 8};
    int i, j;

    printf("_");
    for(i = 0; i < (n * 2); i++)
        printf("_");
    printf("\n");

    for(i = 0; i < m; i++){
        printf("|");
        for(j = 0; j < n; j++){
            printf(((maze[i*n + j] & down) != 0) ? " " : "_");
            if((maze[i*n + j] & left) != 0){
                printf((((maze[i*n + j] || maze[i*n + j + 1]) & down) != 0) ? " " : "_");
            }else{
                printf("|");
            }
        }
        printf("\n");
  }
  return;
}

void array_shuffle(int *arr, int size)
{
    int i, r, tmp;
    for(i = 0; i < (size - 1); i++){
        r = i + (rand() % (size - i));
        tmp = arr[i];
        arr[i] = arr[r];
        arr[r] = tmp;
    }
    return;
}

void going_to_from(int startx, int starty, char *maze, int m, int n)
{
    enum {up = 1, left = 4, down = 2, right = 8};
    int directions[4] = {up, left, down, right};
    int dx, dy, nx, ny, i;
    int DX[9];
    int DY[9];
    int opposite[9];
    opposite[up] = down;
    opposite[left] = right;
    opposite[down] = up;
    opposite[right] = left;

    DX[up] = 0;
    DX[left] = 1;
    DX[down] = 0;
    DX[right] = -1;

    DY[up] = -1;
    DY[left] = 0;
    DY[down] = 1;
    DY[right] = 0;

    array_shuffle(directions, 4);

    /*
    for(i = 0; i < 4; i++)
        printf("Direction: %d\n", directions[i]);
    */

    for(i = 0; i < 4; i++) {
        dx = DX[directions[i]];
        dy = DY[directions[i]];

        /*printf("Check direction=x:y - %d=%d:%d\n", directions[i], dx, dy);*/

        /* check if the cell is valid */
        nx = startx + dx;
        ny = starty + dy;
        /* check if we are on valid cell */
        if ( ((nx < n) && (nx >= 0)) && ((ny < m) && (ny >= 0)) ) {
            /* check if cell is not visited */
            if (maze[ny*n + nx] == 0) {
                /*printf("Valid cell x:y %d:%d\n", nx, ny);*/
                maze[starty*n + startx] = (maze[starty*n + startx] | directions[i]);
                maze[ny*n + nx] = (maze[ny*n + nx] | opposite[directions[i]]);
                going_to_from(nx, ny, maze, m, n);
            }
        }
    }
    return;
}

int main()
{
    int m, n, i, j;
    char *maze = NULL;
    srand(time(NULL));
/*again:{}*/
    printf("Enter number of rows: ");
    scanf("%d", &m);
    printf("Enter number of columns: ");
    scanf("%d", &n);
    /*
    if((m != n) || (m == 0) || (n==0)){
        printf("\nWrong sizes!!!\n");
        printf("The maze should be a square\n");
        goto again;
    }
    
    m += 2;
    n += 2;
    */
    maze = malloc(sizeof(char)*m*n);
    for(i = 0; i < m; i++)
        for(j = 0; j < n; j++)
            maze[i*n + j] = 0;
    printf("\nGrid:\n");
    maze_print(maze, m, n);
    printf("\nMaze:\n");
    going_to_from(0, 0, maze, m, n);
    maze_print(maze, m, n);
    return 0;
}