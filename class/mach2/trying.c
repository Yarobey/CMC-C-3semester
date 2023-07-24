#include <stdlib.h>

int main() {
    int *array = (int *)malloc(sizeof(int) * 100);
    array = NULL;   /* <--- Memory Leak */
    return 0;
}