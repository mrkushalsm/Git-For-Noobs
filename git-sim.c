#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <dirent.h>

int stage_status = 0; // 0 = staging, 1 = not staging
char stage_files[1000][1000];
int stage_files_count;

void get_dir_list() {
    printf("TODO");
}

void add(int i, char * argv[]) {
    DIR* dir = opendir(".git");
    if(dir) {
        if(!strcmp("--all", argv[i])) {
            printf("TODO");
        }
        else {
            stage_status = 1;
            stage_files[stage_files_count][0] = argv[i];
            stage_files_count++;
        }
    }
    else {
        printf("This directory is not a git repositry!\nUse \"git-sim init\" to add the current directory as a repositry!\n");
    }
}

void init() {
    char *working_dir = (char*)malloc(sizeof(char) * 1000);
    *working_dir = system("cd");
    working_dir[strcspn(working_dir, "\n")] = 0;
    printf("Initialized empty Git repository in %s/.git/\n", working_dir);
}

int main(int argc, char * argv[]) {
    for(int i = 1; i <= argc - 1; i++) {
        if(!strcmp(argv[i], "--version")) {
            printf("git simulated version 1.0.0\n");
        }
        else if(!strcmp(argv[i], "init")) {
            init();
        }
        else {
            printf("I do not understand this command!");
            exit(0);
        }
    }
    
    return 0;
}