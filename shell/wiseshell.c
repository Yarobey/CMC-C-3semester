/* myshell.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

typedef struct word_item {
    char *word;
    struct word_item *next;
} word_node;

typedef struct tokens_item {
    int pipes;
    int leftbrack;
    int rightbrack;
    int in;
    int out;
    int endin;
} tokens;

int is_token(char c);

/* reading line that was given */
char *readln(int *flag)
{
    enum {size = 4};
    char *str = NULL;
    char token = 0;
    int i = 0, c, len = 0;
    c = getchar();
    while((c != EOF) && (c != '\0') && (c != '\n')){
        if(i == len){
            len += size;
            str = (char *)realloc(str, len);
        }
        /* checking for token */
        if(is_token(c)) {
            len += size;
            str = (char *)realloc(str, len);
            /* checking for token that is longer than 1 symbol */
            if((token == c) && ((c == '&') || (c == '>') || (c == '|'))) {
                str[i-1] = c;
                str[i] = ' ';
            } else {
                str[i] = ' ';
                str[i+1] = c;
                str[i+2] = ' ';
                token = c;
                i += 2;
            }
        /* not token */
        } else {
            str[i] = c;
            token = 0;
        }
        c = getchar();
        i++;
    }
    if(c == EOF)
        *flag = 0;
    if(i)
        str[i] = '\0';
    if(c != '\n')
        printf("\n");
    return str;
}

/* looking for the start of the current word */
char *word_start(char *str)
{
    int i;
    for(i = 0; ((str[i] == ' ') || (str[i] == '\t')) && str[i]; i++)
        {}
    return str + i;
}

/* looking for the end of the current word */
char *word_end(char *str)
{
    int i;
    for(i = 0; (str[i] != ' ') && (str[i] != '\0') && (str[i] != '\t') && (str[i] != '\r'); i++)
        {}
    return str + i - 1;
}

/* counting words in entered string */
int word_count(char *str)
{
    char *start = NULL, *end = NULL;
    int counter = -1;
    end = str - 1;
    do {
        counter++;
        start = word_start(end+1);
        end = word_end(start);
    } while(start != (end + 1));
    return counter;
}

/* creating new node of the word list */
word_node *add_word(char *start, char *end)
{
    word_node *tmp = NULL; 
    int i, len = end - start + 1;
    tmp = malloc(sizeof(word_node));
    tmp->word = malloc((len+1) * sizeof(char));
    tmp->next = NULL;
    for(i = 0; i < len; i++)
        tmp->word[i] = start[i];
    tmp->word[i] = '\0';
    return tmp;
}

/* creating a linked list of words */
word_node *words_into_list(char *str, int *wordcount)
{
    char *start = NULL, *end = NULL;
    word_node *first = NULL, *last = NULL, *tmp = NULL;
    int i, count;
    *wordcount = count = word_count(str);
    end = str - 1;
    for(i = 0; i < count; i++) {
        start = word_start(end+1);
        end = word_end(start);
        tmp = add_word(start, end);
        if(last) {
            last->next = tmp;
            last = last->next;
        } else {
            first = last = tmp;
        }
    }
    return first;
}

void print_list(const word_node *lst)
{
    for(; lst; lst = lst->next)
        printf("[%s]\n", lst->word);
    return;
}

void delete_list(word_node *lst)
{
    if(!lst)
        return;
    delete_list(lst->next);
    /*free(lst->word);*/
    free(lst);
}

/* creating an array of words based on the linked list that was created */
char **get_words(word_node *lst)
{
    char **words = NULL;
    word_node *tmp = lst;
    int counter = 0, i;
    /* the amount of words */
    for(tmp = lst; tmp; tmp = tmp->next)
        counter++;
    /* creating an array of string pointers */
    words = malloc((counter+1) * sizeof(char*));
    tmp = lst;
    for(i = 0; i < counter; i++) {
        words[i] = tmp->word;
        tmp = tmp->next;
    }
    words[counter] = NULL;
    return words;
}

/* freeing all memory that was allocated */
void clear(word_node *lst, char *str, char **words, tokens *item, int *cmds)
{
    int i, counter;
    delete_list(lst);
    counter = word_count(str);
    free(str);
    for(i = 0; i < counter; i++)
        if(words[i])
            free(words[i]);
    free(words);
    free(item);
    free(cmds);
    return;
}

/* cd command */
void cd_cmd(char **words)
{
    char cwd[PATH_MAX];
    char *home = NULL;
    int check, args = 0, i;
    /* amount of parameters for cd */
    for(i = 0; words[i]; i++)
        args++;
    if(!args)
        return;
    if(args == 1) {
        home = getenv("HOME");
        if(!home) {
            perror("I don't know where's your home");
            exit(1);
        }
        check = chdir(home);
    }
    if(args <= 2) {
        if(args == 2)
            check = chdir(words[1]);
        if(check == -1) {
            perror(words[1]);
            exit(1);
        }
        if(getcwd(cwd, sizeof(cwd)) != NULL)
            printf("Current working dir: %s\n", cwd);
        else {
            perror("getcwd() error");
            exit(1);
        }
    }
    return;
}

/* command execution */
void execute(char **words, int bg)
{
    char cd[] = "cd";
    int pid;
    if(strcmp(words[0], cd) == 0)
        cd_cmd(words);
    pid = fork();
    if(pid == -1) {
        perror("fork");
        exit(1);
    }
    if(pid == 0) {                        /* child process */
        execvp(words[0], words);
        perror(words[0]);        /* exec returned control -> error */
        exit(1);           /* terminating the process with exit code of an error */
    }
    /* parent process */
    /*wait(NULL); */     /* waiting for the child process to end, killing zombie */
    if(!bg)
        wait(NULL);
    return;
}

/* checking for token */
int is_token(char c)
{
    if((c == '&') || (c == '>') || (c == '<') || (c == '|') || (c == '(') || (c == ')') || (c == ';'))
        return 1;
    return 0;
}

tokens *token_counting(word_node *lst)
{
    tokens *item = malloc(sizeof(tokens));
    word_node *tmp = lst;
    item->pipes = 0;
    item->leftbrack = 0;
    item->rightbrack = 0;
    for(; tmp; tmp = tmp->next) {
        if(strcmp(tmp->word, "|") == 0)
            (item->pipes)++;
        if(strcmp(tmp->word, "(") == 0)
            (item->leftbrack)++;
        if(strcmp(tmp->word, ")") == 0){
            (item->rightbrack)++;
            if(item->leftbrack < item->rightbrack) {
                perror("Unmatched brackets");
                exit(1);
            }
        }
    }
    if(item->leftbrack != item->rightbrack){
        perror("Unmatched brackets");
        exit(1);
    }
    return item;
}


int *cmds_starts(char **words, tokens *item)
{
    int *cmds = malloc(sizeof(int)*(item->pipes+1));
    int i, j = 0, flag = 1;
    for(i = 0; words[i]; i++) {
        if(flag) {
            cmds[j] = i;
            flag = 0;
            j++;
        }
        if(strcmp(words[i], "|") == 0) {
            free(words[i]);
            words[i] = NULL;
            flag = 1;
        }
    }
    cmds[j] = -1;
    return cmds;
}

int pids_check(int *pids, int counter)
{
    int i;
    for(i = 0; i < counter; i++)
        if(pids[i])
            return 1;
    return 0;
}

/* pipeline */
int pipeline(int cmd_count, char **words, int *cmds, int bg) 
{
    int fd[2], in, out, next_in, i;
    int p, pids[cmd_count];

    if(cmd_count == 0)
        return 0;   /* if no arguments specified do nothing */

    if(cmd_count == 1) { /* if only one specified execute command */
        execute(words+cmds[0], bg);
        return 0;
    }

    /* cmd_count > 1 */
    pipe(fd);
    out = fd[1]; /* write fd */
    next_in = fd[0]; /* read fd */

    /* 1-st process */
    if ((pids[0] = fork()) == 0) {
        close(fd[0]);                 /* don't need to read from the pipe */
        dup2(fd[1], 1);                                 /* stdout - into pipe */
        close(fd[1]);                             /* don't need fd[1] anymore */
        execvp(words[cmds[0]], words+cmds[0]);
        perror(words[cmds[0]]);                 /* exec returned control -> error */
        exit(1);      /* terminating the process with exit code of an error */
    }

    in = next_in;

    for(i = 1; i < cmd_count - 1; i++) {
        close(out); /* closing pipe write descriptor parent process, 
        otherwise child procesess will never stop readings */
        
        pipe(fd); /* fd array is used multiple times 
        and after each pipe call it is filled with new descriptors */

        out = fd[1];
        next_in = fd[0];

        if((pids[i] = fork()) == 0) {
            close(next_in); 
            dup2(in, 0);
            close(in);
            dup2(out, 1);
            close(out);
            execvp(words[cmds[i]], words+cmds[i]);
            perror(words[cmds[i]]);                 /* exec returned control -> error */
            exit(1);      /* terminating the process with exit code of an error */
        }

        close(in);
        in = next_in;
    }

    if((pids[cmd_count-1] = fork()) == 0) {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execvp(words[cmds[cmd_count-1]], words+cmds[cmd_count-1]);
        perror(words[cmds[cmd_count-1]]);                 /* exec returned control -> error */
        exit(1);      /* terminating the process with exit code of an error */
    }

    close(fd[0]);
    close(fd[1]);

    if(!bg) {
        do {
            p = wait(NULL);
            for(i = 0; i < cmd_count; i++) {
                if(p == pids[i]){
                    pids[i] = 0;
                    continue;
                }
            }
        } while(pids_check(pids, cmd_count));
    }

    /*
    for(i = 0; i < cmd_count; i++)
        wait(NULL);
    */

    return 0;
}

int background_check(word_node *lst)
{
    word_node *tmp = lst;
    for(; lst; lst = lst->next) {
        /* "&" is the last word */
        if((strcmp(lst->word, "&") == 0) && (!lst->next)){
            free(lst->word);
            free(lst);
            tmp->next = NULL;
            return 1;
        }
        /* "&" is elsewhere on the string */
        else if(strcmp(lst->word, "&") == 0){
            perror("&");
            exit(1);
        }
        tmp = lst;
    }
    return 0;
}

void redirection(char *redir, char *arg)
{
    int fd;
    if (strcmp(redir, "<") == 0){
        fd = open(arg, O_RDONLY);
        if(fd == -1) {
            perror(arg);
            exit(1);
        }
        dup2(fd, 0);
        close(fd);
    } else if (strcmp(redir, ">") == 0){
        fd = open(arg, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if(fd == -1) {
            perror(arg);
            exit(1);
        }
        dup2(fd, 1);
        close(fd);
    } else if (strcmp(redir, ">>") == 0){
        fd = open(arg, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if(fd == -1) {
            perror(arg);
            exit(1);
        }
        dup2(fd, 1);
        close(fd);
    }
    return;
}

int token_word(char *word)
{
    if(strcmp(word, "|") == 0)
        return 1;
    if(strcmp(word, ">") == 0)
        return 1;
    if(strcmp(word, ">>") == 0)
        return 1;
    if(strcmp(word, "<") == 0)
        return 1;
    if(strcmp(word, "&&") == 0)
        return 1;
    if(strcmp(word, "||") == 0)
        return 1;
    if(strcmp(word, "&") == 0)
        return 1;
    if(strcmp(word, ";") == 0)
        return 1;
    if(strcmp(word, "(") == 0)
        return 1;
    if(strcmp(word, ")") == 0)
        return 1;
    return 0;
}

int is_redirection(char *word, int *count1, int *count2, int *count3)
{
    if(strcmp(word, ">") == 0) {
        (*count1)++;
        return 1;
    }
    if(strcmp(word, ">>") == 0) {
        (*count2)++;
        return 1;
    }
    if(strcmp(word, "<") == 0) {
        (*count3)++;
        return 1;
    }
    return 0;
}

int exec_cmd_redirection(char **words, int cmd_count, int cmd_num, int cur_cmd_start, int *redir_count)
{
    int i, flag = 0, newstart = 0, new_start_ind = cur_cmd_start;
    for(i = cur_cmd_start; words[i]; i++) {
        /* if cur word is redirection token */
        if(is_redirection(words[i], &redir_count[0], &redir_count[1], &redir_count[2])) {
            /* if redirection is the first argument of a command */
            if(i == cur_cmd_start)
                newstart = 1;
            if(flag) { /* if there was already redirection, but there is another one right after */
                perror("Incorrect argument for redirection!!!");
                exit(1);
            }
            if((cmd_num != 0) && (cmd_num != cmd_count - 1)) {
                perror("Redirection in pipeline!!!");
                exit(1);
            }
            flag = 1;
            continue;
        /* if cur word is not redirection token */
        } else { 
            if(flag) { /* if prev word was redirection */
                if(token_word(words[i])){
                    perror("Incorrect argument for redirection!!!");
                    exit(1);
                }
                redirection(words[i-1], words[i]);
                free(words[i]);
                free(words[i-1]);
                words[i] = NULL;
                words[i-1] = NULL;
            } else { /* newstart of the command */
                if(newstart) {
                    new_start_ind = i;
                    newstart = 0;
                }
            }
            flag = 0;
        }
    }
    /* printf("'>' = %d, '>>' = %d, '<' = %d\n", redir_count[0], redir_count[1], redir_count[2]); */
    if((redir_count[0] > 1) || (redir_count[1] > 1) || (redir_count[2] > 1)) {
        perror("Too many redirections!!!");
        exit(1);
    }
    redir_count[0] = redir_count[1] = redir_count[2] = 0;
    return new_start_ind;
}

void exec_redirection(char **words, int *cmds, int cmd_count)
{
    int redir_count[] = { 0, 0, 0 };
    int newstart, i;
    for(i = 0; i < cmd_count; i++) {
        newstart = exec_cmd_redirection(words, cmd_count, i, cmds[i], redir_count);
        if(newstart != cmds[i])
            cmds[i] = newstart;
    }
    return;
}

/*
void brackets(word_node *lst, tokens *token_count)
{
    int lbrack = 0, rbrack = 0;
    if(token_count->pipes) {
        perror("Brackets in pipeline!!!");
        exit(1);
    }
}
*/


int main(int argc, char **argv)
{
    char **words = NULL;
    char *str = NULL;
    int *cmds = NULL;
    tokens *token_count = NULL;
    word_node *lst = NULL;
    int wordcount, noteof = 1, cmdcount = 0, bg = 0;
    while(noteof) {
        printf("> ");
        str = readln(&noteof);
        if(str) {
            if(fork() == 0) {
                lst = words_into_list(str, &wordcount);
                token_count = token_counting(lst);
                /* print_list(lst); */
                bg = background_check(lst);
                /* print_list(lst); */
                words = get_words(lst);
                cmds = cmds_starts(words, token_count);    /* looking for start of every command */
                for(cmdcount = 0; cmds[cmdcount] != -1; cmdcount++) /* counting commands */
                    ;
                exec_redirection(words, cmds, cmdcount);
                /*
                for(i = 0; i < wordcount; i++)
                    printf("%s\n", words[i]);
                for(cmdcount = 0; cmds[cmdcount] != -1; cmdcount++)
                    printf("cmd_start[%d] = %d\n", cmdcount, cmds[cmdcount]);
                */
                pipeline(cmdcount, words, cmds, bg);
                clear(lst, str, words, token_count, cmds);
                exit(1);
            }
            wait(NULL);
        }
    }
    return 0;
}
/* abra schwabra kadabra */
/* ls -lR | grep ^d */
/* ls -l -a /var */
/* sleep 10 & ps -a */
/* ls | wc > bcd ; cat bcd */
/* sleep 1 ; ps -a */
/* false || ls */
/* cat egf || ls */
/* sleep 1 | sleep 5 */
/* sleep 5 | sleep 5 */
/* sleep 5 & */
/* cat | grep int | grep -v main < shell.c */
/* cat < shell.c | grep int | grep -v main */
/* < file.txt cat HeyJude.txt | > result.txt sort */
/* cat heyjude.txt > result.txt < file.txt */
/* cat heyjude.txt | sort */
/* cat hello.c | grep int | grep -v main */
/* cat hello.c | cat | cat | grep int | cat | grep -v main | cat */