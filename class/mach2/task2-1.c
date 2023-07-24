/* mash2-1.c */
#include <stdio.h>
#include <stdlib.h>

struct item{
    long int data;
    int counter;
    struct item *next;
};

struct item *add_new_item(long int x)
{
    struct item *tmp;
    tmp = malloc(sizeof(struct item));
    tmp->data = x;
    tmp->counter = 1;
    tmp->next = NULL;
    return tmp; 
}

struct item *new_data(struct item *list, long int x)
{
    struct item *tmp;
    if(list == NULL){
        list = add_new_item(x);
        return list;
    }
    tmp = list;
    while(tmp->next != NULL){
        if(tmp->data != x){
            tmp = tmp->next;
        }else{
            (tmp->counter)++;
            return list;
        }
    }
    if(tmp->data == x){
        (tmp->counter)++;
        return list;
    }
    tmp->next = add_new_item(x);
    return list;
}

int max_encounters(struct item *list)
{
    struct item *tmp;
    int max = 0;
    tmp = list;
    while(tmp != NULL){
        if(tmp->counter > max)
            max = tmp->counter;
        else
            tmp = tmp->next;
    }
    return max;
}

void max_items_print(struct item *list)
{
    struct item *tmp;
    int max;
    max = max_encounters(list);
    while (max != 0){
        tmp = list;
        while(tmp != NULL){
            if(tmp->counter == max){
                printf("counter = %d, %ld\n", tmp->counter, tmp->data);
                tmp->counter = 0;
            }
            tmp = tmp->next;
        }
        max = max_encounters(list);
    }
    return;
}

void clear_list(struct item *list)
{
    struct item *p;
    if(list != NULL){
        p = list;
        list = list->next;
        free(p);
        clear_list(list);
    }
    return;
}

void delete_int_list(struct item *lst)
{
    if(!lst)
        return;
    delete_int_list(lst->next);
    free(lst);
}

int main()
{
    struct item *start = NULL;
    int n;
    long int number;

    n = scanf("%ld", &number);
    while(n != -1){
        start = new_data(start, number);
        n = scanf("%ld", &number);
    }
    max_items_print(start);
    clear_list(start);
    return 0;
}