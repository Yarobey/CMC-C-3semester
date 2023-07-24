#include <stdio.h>
#include <stdlib.h>

struct node{
    int val;
    struct node *left, *right;
};

void int_bin_tree_add(struct node **root, int n) /* адрес указателя на корневой узел поддерева */
{
    if(!*root){
        *root = malloc(sizeof(**root));
        (*root)->val = n;
        (*root)->left = NULL;
        (*root)->right = NULL;
        return;
    }
    /*
    if((*root)->val == n)
        return;
    */
    if(n < (*root)->val)
        int_bin_tree_add(&(*root)->left, n);
    else
        int_bin_tree_add(&(*root)->right, n);
}

struct node *create(FILE *f)
{
    int n;
    struct node *tree = NULL;
    while(fscanf(f, "%d", &n) != -1)
        int_bin_tree_add(&tree, n);
    return tree;
}

void int_bin_tree_print_rec(struct node *r)
{
    if(!r)
        return;
    int_bin_tree_print_rec(r->left);
    printf("%d\n", r->val);
    int_bin_tree_print_rec(r->right);
}

void int_bin_tree_delete_rec(struct node *r)
{
    if(!r)
        return;
    int_bin_tree_delete_rec(r->left);
    int_bin_tree_delete_rec(r->right);
    free(r);
}


int int_bin_tree_search(struct node *r, int n)
{
    int flag = 0, counter = 0;
    struct node *tmp;
    tmp = r;
    while((!flag) && (tmp != NULL)){
        if(tmp->val == n)
            flag = 1;
        else{
            if(n < tmp->val){
                tmp = tmp->left;
                counter++;
            }
            else{
                tmp = tmp->right;
                counter++;
            }
        }
    }
    if(flag)
        return counter;
    else
        return -1;
}

int main(int argc, char **argv)
{
    FILE *f;
    int n = 5, k;
    struct node *tree;
    f = fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }
    tree = create(f);
    int_bin_tree_print_rec(tree);
    k = int_bin_tree_search(tree, n);
    printf("k = %d\n", k);
    if (k == -1)
        int_bin_tree_add(&tree, n);
    printf("New tree: \n");
    int_bin_tree_print_rec(tree);
    int_bin_tree_delete_rec(tree);
    fclose(f);
    return 0;
}