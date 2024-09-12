#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <dirent.h>

int stage_status = 0; // 0 = staging, 1 = not staging
char *stage_files;
int commit_count = 0; // For void status()

void status() {
    printf("On branch master\n\n");
    // if() TODO
}

void add(int i, char * argv[]) {
    *stage_files = (char*)malloc(sizeof(char) * 1000);
    DIR* dir = opendir(".git");
    if(dir) {
        stage_status = 1;
        stage_files = argv[i + 1];
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