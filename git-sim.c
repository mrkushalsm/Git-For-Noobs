#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>

void init() {
    // char *working_dir = (char*)malloc(sizeof(char) * 1000);
    // *working_dir = system("cd");
    printf("Initialized empty Git repository in ");
    system("cd");
    printf("/.git/\n");
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