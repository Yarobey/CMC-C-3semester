/*
2. Шифр Ulong Long - написать функцию, которая будет записывать символы в Long Long, 
причем при выводе каждого числа шифра применяется следующая схема:
- если младший бит равен 1, то инвертируем все биты, кроме старшего и младшего
- иначе меняем местами первую и вторую половины числа, 
сохраняя при это значения старшего и младшего бита (нужна маска вида 01...10)
*/
#include <stdio.h>

void print_binary(unsigned n)
{
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

void turnover(unsigned long long *num)
{
    unsigned long long smpl1, smpl2, smpl3, a, b;
    smpl1 = 0x80000001; /* 1000..00000..01 */
    smpl2 = 0x7FFF0000; /* 0111..10000..00 */
    smpl3 = 0x0000FFFE; /* 0000..01111..10 */
    if(*num&1){
        *num ^= ~smpl1;
    }else{
        a = *num & smpl2;
        b = *num & smpl3;
        a >>= 31;
        b <<= 31;
        *num &= smpl1;
        *num |= a;
        *num |= b;
    }
    return;
}

void writing_code_to_longlong(char *str, unsigned long long *n)
{
    int i;
    i = 0;
    while(str[i] != '\0'){
        if(str[i] != ' '){
            *n <<= 8;
            *n |= str[i];
            turnover(n);
        }
        if((i+1)%8 == 0){
            printf("%llu\n", *n);
            *n = 0;
        }
        i++;
    }
    return;
}

int main(int argc, char **argv)
{
    unsigned long long blank;
    int i;
    char *word;
    for(i = 1; i < argc; i++){
        blank = 0;
        word = argv[i];
        printf("String number %d:\n", i);
        writing_code_to_longlong(word, &blank);
        printf("%llu \n", blank);
    }
    return 0;
}