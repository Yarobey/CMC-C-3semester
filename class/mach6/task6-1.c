#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

void string_copy(char *dest, const char *src)
{
    int i;
    for(i = 0; src[i]; i++)
        dest[i] = src[i];
    dest[i] = 0;
}

int isDirectory(const char *path)
{
    struct stat statbuf;
    if(stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

int print_symlinks(const char *path)
{
    DIR *dir;
    struct dirent *dent;
    struct stat statbuf;
    char buf[255];
    int end;

    dir = opendir(path);
    chdir(path);

    /* looking for errors */
    if(!dir){
        perror(path);
        return 1;
    }

    /* printing every symbolic link if there are any */
    while((dent = readdir(dir)) != NULL){
        /* checking for symbolic link */
        lstat(dent->d_name, &statbuf);
        /*
        printf("%s\n", strerror(errno));
        */
        if(S_ISLNK(statbuf.st_mode)){
            /* printing symbolic link and it's i-node */
            printf("\"%s\" is a symbolic link, i-node = %lld\n", dent->d_name, statbuf.st_ino);

            /* printing file and it's i-node */
            stat(dent->d_name, &statbuf);
            end = readlink(dent->d_name, buf, sizeof(buf));
            if(end != -1)
                buf[end] = '\0';
            printf("to \"%s\", i-node = %lld\n", buf, statbuf.st_ino);

            printf("\n");
        }
    }
    closedir(dir);
    return 0;
}

void copy_file(int fd1, int fd2)
{
    int count;
    char c;
    lseek(fd1, 0, SEEK_SET);
    count = read(fd1, &c, sizeof(char));
    while((count != 0) && (count != -1)) {
        write(fd2, &c, sizeof(char));
        count = read(fd1, &c, sizeof(char));
    }
    return;
}

void create_file2(int fd1, int fd2)
{
    long firstposs, lastposs, fendposs, lendposs, currposs;
    int count;
    char c;

    /* start possition of the first word */
    firstposs = lseek(fd1, 0, SEEK_SET);
    count = read(fd1, &c, sizeof(char));
    while(((c == ' ') || (c == '\t') || (c == '\n')) && (count == 1)){
        firstposs = lseek(fd1, 0, SEEK_CUR);
        count = read(fd1, &c, sizeof(char));
    }
    /*
    printf("1-st start = %ld\n", firstposs);
    */

    /* no words in file */
    if(count == 0){
        copy_file(fd1, fd2);
        return;
    }

    /* end possition of the first word */
    while((c != ' ') && (c != '\t') && (c != '\n') && (count == 1)){
        fendposs = lseek(fd1, 0, SEEK_CUR);
        count = read(fd1, &c, sizeof(char));
    }
    /*
    printf("1-st end = %ld\n", fendposs);
    */

    /* end possition of the last word */
    lendposs = lseek(fd1, -1, SEEK_END);
    count = read(fd1, &c, sizeof(char));
    while((c == ' ') || (c == '\t') || (c == '\n')){
        lendposs = lseek(fd1, -2, SEEK_CUR);
        count = read(fd1, &c, sizeof(char));
    }
    lendposs++;
    /*
    printf("2-nd end = %ld\n", lendposs);
    */

    /* only one word in file */
    if(fendposs == lendposs){
        copy_file(fd1, fd2);
        return;
    }

    /* start possition of the last word */
    lastposs = lseek(fd1, -1, SEEK_CUR);
    count = read(fd1, &c, sizeof(char));
    while((c != ' ') && (c != '\t') && (c != '\n')) {
        lastposs = lseek(fd1, -2, SEEK_CUR);
        count = read(fd1, &c, sizeof(char));
    }
    lastposs = lseek(fd1, 0, SEEK_CUR);
    lastposs++;
    /*
    printf("2-nd start = %ld\n", lastposs);
    */
    
    /* last word into new file */
    count = read(fd1, &c, sizeof(char));
    while((count != 0) && (count != -1) && (c != ' ') && (c != '\t') && (c != '\n')) {
        write(fd2, &c, sizeof(char));
        count = read(fd1, &c, sizeof(char));
    }

    /* file before the last word and after the first word */
    currposs = lseek(fd1, fendposs, SEEK_SET);
    count = read(fd1, &c, sizeof(char));
    while((count != 0) && (count != -1) && (currposs != lastposs)) {
        write(fd2, &c, sizeof(char));
        count = read(fd1, &c, sizeof(char));
        currposs = lseek(fd1, 0, SEEK_CUR);
    }

    /* first word into new file */
    currposs = lseek(fd1, firstposs, SEEK_SET);
    count = read(fd1, &c, sizeof(char));
    while((count != 0) && (count != -1) && (currposs != fendposs+1)) {
        write(fd2, &c, sizeof(char));
        count = read(fd1, &c, sizeof(char));
        currposs = lseek(fd1, 0, SEEK_CUR);
    }

    /* everything after the last word */
    currposs = lseek(fd1, lendposs, SEEK_SET);
    count = read(fd1, &c, sizeof(char));
    while((count != 0) && (count != -1)) {
        write(fd2, &c, sizeof(char));
        count = read(fd1, &c, sizeof(char));
    }

    return;
}

int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *dent;
    struct stat statbuf;
    int mode;
    const char *name = ".";
    char newname[strlen(argv[1]) + 4];
    int flag = 0, iscopy = 0;
    int fd[2];

    if(argc < 1)
        return 0;
    name = argv[1];
    flag = isDirectory(name);
    printf("\n\"%s\" is directory = %d\n", name, flag);

    /* if directory then printing symlinks */
    if(flag){
        print_symlinks(name);
        return 0;
    }
    /* else creating new file with prefix "new" */
    dir = opendir(".");
    if(!dir) {
        perror(".");
        return 1;
    }

    /* creating a new file name */
    string_copy(newname, "new");
    if((name[0] == '.') && (name[1] == '/')){
        strncat(newname, name+2, strlen(name));
    }else{
        strncat(newname, name, strlen(name));
    }
    printf("newname = %s\n\n", newname);
    
    /* looking for newname's copies */
    while((dent = readdir(dir)) != NULL) {
        if(strcmp(dent->d_name, newname) == 0)
            iscopy = 1;
    }
    /* changing mode of file */
    if(iscopy)
        chmod(newname, 0666);

    /* checking mode of an old file */
    stat(name, &statbuf);
    mode = statbuf.st_mode & 07777;

    /* opening files */
    fd[0] = open(name, O_RDWR);
    if(fd[0] == -1) {
        perror(name);
        return 1;
    }
    fd[1] = open(newname, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if(fd[1] == -1) {
        perror(name);
        return 1;
    }
    create_file2(fd[0], fd[1]);
    printf("comleted...\n");
    close(fd[0]);
    close(fd[1]);
    return 0;
}