/* 
Задача 2. MyLittleT9. Реализовать структуру (дерево) следующего вида:
- Каждый экземпляр содержит 26 указателей, 
по одному на каждую букву английского алфавита (можно расширить и на русский)
- В каждом экземпляре есть счетчик - сколько раз при обходе 
    этой структуры остановка происходила в этой вершине
- В структуру добавляются строки - обход идет посимвольно, 
    выбор следующей вершины производится в соответствии со следующим символом строки; 
    если указатель не инициализирован, 
    то создается новый экземпляр структуры (адрес записывается в указатель) 
    после чего осуществляется переход в неё; 
    обход заканчивается при достижении признака конца строки
- Структура поддерживает поиск - реализовать отдельную функцию, 
    которая принимает на вход слово и корень(самый первый элемент) структуры и 
    выводит k слов (меньше, если было добавлено меньше слов), 
    которые имеют общий префикс, в порядке длины этого префикса, 
    а если длина префикса у вариантов одинаковая, то в порядке частоты, 
    если же и частота одинаковая, 
    то в порядке количества дополнительных символов после префикса
- Написать отдельную функцию для освобождения выделенной под дерево памяти

Пример:
Ввод: cat cow car car cars catnip catnip

Карта прохода по дереву (в скобках значение счетчика, если были остановки при добавлении слов):
       c
    /     \
   a       o
 /   \     |
t(1)  r(2) w(1)
|     |
n    s(1)
|
i
|
p(2)
  
Поиск слова "cart" k=2:
car 2 
cars 1

Поиск слова "cat" k=3:
catnip 2
cat 1
car 2
 
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct node{
    int counter;
    int flag;
    char *word;
    struct node *letter[26];
};

struct item{
    int counter;
    int len;
    char *word;
    struct item *next;
};

struct data{
    int count;
    int length;
    char *str;
};

/* reading string */
char *readln()
{
    enum {size = 4};
    char *str = NULL;
    int i = 0, c, len = 0;
    c = getchar();
    while((c != EOF) && (c != '\0') && (c != '\n')){
        if(i == len){
            len += size;
            str = (char *)realloc(str, len);
        }
        str[i] = c;
        c = getchar();
        i++;
    }
    str[i] = '\0';
    if(c != '\n')
        printf("\n");
    return str;
}

void push(struct item **head, char *word, int counter)
{
    struct item* tmp = malloc(sizeof(struct item));
    tmp->word = word;
    tmp->counter = counter;
    tmp->next = *head;
    *head = tmp;
    return;
}

void delete_list(struct item *lst)
{
    if(!lst)
        return;
    delete_list(lst->next);
    free(lst);
}

/* creating new node */
struct node *create_node()
{
    struct node *root = NULL;
    int i;
    root = malloc(sizeof(struct node));
    root->counter = 0;
    root->flag = 0;
    root->word = NULL;
    for(i = 0; i < 26; i++)
        root->letter[i] = NULL;
    return root;
}

/* adding word to tree */
struct node *add_word_to_tree(struct node *root, char *str)
{
    struct node *tmp = root;
    int i, symb;
    for(i = 0; str[i]; i++){
    /* letter of the word */
        symb = str[i] - 'a';
        /* creating a new node, if not already created */
        if(!tmp->letter[symb])
            tmp->letter[symb] = create_node();
        tmp = tmp->letter[symb];
    }
    (tmp->counter)++;
    tmp->word = str;
    return root;
}

/* deleting tree */
void delete_tree(struct node *root)
{
    int i;
    if(!root)
        return;
    for(i = 0; i < 26; i++)
        delete_tree(root->letter[i]);
    free(root);
}

/* looking for the longest prefix in tree */
int longest_prefix(const struct node* root, char *word)
{
    int i, symb, counter = 0;
    for(i = 0; word[i]; i++){
        symb = word[i] - 'a';
        if(root->letter[symb])
            counter++;
        else
            break;
        root = root->letter[symb];
    }
    return counter;
}

/* searching for word and its alikes in the tree */
void tree_search(struct node* root, struct item **head)
{
    int i;
    if(!root)
        return;
    for(i = 0; i < 26; i++)
        tree_search(root->letter[i], head);
    if(root->word && !root->flag){
        push(head, root->word, root->counter);
        /*printf("pushing = %s\n", root->word);*/
        root->flag = 1;
    }
}

void swap(struct data *xp, struct data *yp)
{
    struct data tmp = *xp;
    *xp = *yp;
    *yp = tmp;
}
 
void bubbleSort_len(struct data **arr, int len)
{
    int i, j;
    for (i = 0; i < len - 1; i++) 
        /* Last i elements are already in place */
        for (j = 0; j < len - i - 1; j++)
            if (arr[j]->length < arr[j+1]->length)
                swap(arr[j], arr[j + 1]);
}

void bubbleSort_counter(struct data **arr, int len)
{
    int i, j;
    for (i = 0; i < len - 1; i++) 
        /* Last i elements are already in place */
        for (j = 0; j < len - i - 1; j++)
            if (arr[j]->count < arr[j+1]->count)
                swap(arr[j], arr[j + 1]);
}

char *readword(FILE *f, int *flag)
{
    enum {size = 4};
    char *word = NULL;
    int i = 0, c, len = 0, been = 0;
    c = fgetc(f);
    while((c != EOF) && (c != '\0') && (c != '\n') && (c != ' ')){
        if(i == len){
            len += size;
            word = (char *)realloc(word, len);
        }
        word[i] = c;
        c = fgetc(f);
        i++;
        been = 1;
    }
    if(c == EOF)
        *flag = 1;
    if(been)
        word[i] = '\0';
    return word;
}

int main(int argc, char **argv)
{
    struct node *tree = NULL, *new = NULL;
    struct item *list = NULL, *tmp = NULL;
    struct data **arr = NULL;
    FILE *f = NULL;
    char **words = NULL;
    char *word = NULL, *newword = NULL;
    int i, j = 0, prefix = 0, counter = 0, k, flag = 0, len, count = 0;

    if(argc < 2){
        printf("ERROR: NO DICTIONARY!!!\n");
        return 0;
    }
    /* looking for errors in openning */
    f = fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }

    /* creating tree */
    tree = create_node();
    len = sizeof(char*);
    /* reading words from the file and adding them to the tree*/
    while(!flag){
        newword = readword(f, &flag);
        if(newword){
            words = realloc(words, len);
            words[count] = newword;
            tree = add_word_to_tree(tree, words[count]);
            len += sizeof(char*);
            count++;
        }
    }

    /* Word that needs to be completed */
    printf("Enter word for search: ");
    word = readln();
    printf("Enter 'k' = ");
    scanf("%d", &k);
    printf("Searching suggestions for word: %s, k = %d\n", word, k);

    /* Looking for longest prefix */
    prefix = longest_prefix(tree, word);

    while((k > 0) && (prefix >= 1)){
        counter = 0;
        j = 0;

        /* calling search function for current max prefix */
        new = tree;
        for(i = 0; i < prefix; i++)
            new = new->letter[word[i]-'a'];
        tree_search(new, &list);

        /* the amount of words of that particular prefix */
        for(tmp = list; tmp; tmp = tmp->next)
            counter++;

        /*creating an array of structures */
        arr = malloc(counter * sizeof(struct data*));
        for(tmp = list; tmp; tmp = tmp->next){
            arr[j] = malloc(sizeof(struct data));
            arr[j]->count = tmp->counter;
            arr[j]->length = tmp->len;
            arr[j]->str = tmp->word;
            j++;
        }
        bubbleSort_len(arr, counter);
        bubbleSort_counter(arr, counter);

        /*printing suggested words */
        for(j = 0; (j < counter) && k; j++){
            printf("%s(%d)\n", arr[j]->str, arr[j]->count);
            k--;
        }

        /* dealoccating memory that we need to re-use */
        delete_list(list);
        for(i = 0; i < counter; i++)
            free(arr[i]);
        free(arr);
        list = NULL;
        arr = NULL;
        prefix--;
    }

    /* dealoccating memory */
    delete_tree(tree);
    for(i = 0; i < count; i++)
        free(words[i]);
    free(words);
    free(word);
    fclose(f);
    return 0;
}
/* cat cow car car cars catnip catnip */
