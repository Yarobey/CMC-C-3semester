/* 
Задача 1. Написать отдельную функцию сортировки связного списка Counter 
в соответствии с частотой элементов и их значением 
(то есть сортируем сначала по частоте, а затем по величине числа). 
Использовать реализацию связного списка с семинара.
 */
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

struct item *new_data(struct item *lst, long int x)
{
    struct item *tmp;
    if(!lst){   /* lst == NULL */
        lst = add_new_item(x);
        return lst;
    }
    tmp = lst;
    while(tmp->next){ /* tmp->next != NULL */
        if(tmp->data != x){
            tmp = tmp->next;
        }else{
            (tmp->counter)++;
            return lst;
        }
    }
    if(tmp->data == x){
        (tmp->counter)++;
        return lst;
    }
    tmp->next = add_new_item(x);
    return lst;
}

void item_data_swap(struct item *lst1, struct item *lst2)
{
    long int tmp;
    tmp = lst1->data;
    lst1->data = lst2->data;
    lst2->data = tmp;
    tmp = lst1->counter;
    lst1->counter = lst2->counter;
    lst2->counter = tmp;
    return;
}

void item_counters_swap(struct item *lst1, struct item *lst2)
{
    int tmp;
    tmp = lst1->counter;
    lst1->counter = lst2->counter;
    lst2->counter = tmp;
    tmp = lst1->data;
    lst1->data = lst2->data;
    lst2->data = tmp;
    return;
}
/*
void bubbleSort_for_counters(struct item *lst) 
{ 
    int swapped; 
    struct item *tmp; 
    struct item *end = NULL; 
    if (!lst) 
        return; 
    
    do{ 
        swapped = 0; 
        tmp = lst; 
        while (tmp->next != end){ 
            if (tmp->counter < tmp->next->counter){ 
                item_counters_swap(tmp, tmp->next); 
                swapped = 1; 
            } 
            tmp = tmp->next; 
        } 
        end = tmp; 
    }while (swapped);
    return;
}
*/

void bubbleSort_for_data(struct item *lst) 
{ 
    int swapped; 
    struct item *tmp; 
    struct item *end = NULL; 
    if (!lst) 
        return; 
    do{ 
        swapped = 0; 
        tmp = lst; 
        while (tmp->next != end){ 
            if (tmp->data > tmp->next->data){ 
                item_data_swap(tmp, tmp->next); 
                swapped = 1; 
            } 
            tmp = tmp->next; 
        } 
        end = tmp; 
    }while (swapped);
    /* for counters */
    end = NULL;
    do{ 
        swapped = 0; 
        tmp = lst; 
        while (tmp->next != end){ 
            if (tmp->counter < tmp->next->counter){ 
                item_counters_swap(tmp, tmp->next); 
                swapped = 1; 
            } 
            tmp = tmp->next; 
        } 
        end = tmp; 
    }while (swapped);
    return; 
}

void delete_int_list(struct item *lst)
{
    if(!lst)
        return;
    delete_int_list(lst->next);
    free(lst);
}

void print_list(const struct item *lst)
{
    for(; lst; lst = lst->next)
        printf("counter = %d, %ld\n", lst->counter, lst->data);
    return;
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
    printf("\nYour numbers and the amount of each: \n");
    print_list(start);
    bubbleSort_for_data(start);
    /*
    bubbleSort_for_counters(start);
    */
    printf("\nAfter sorting: \n");
    print_list(start);
    delete_int_list(start);
    return 0;
}