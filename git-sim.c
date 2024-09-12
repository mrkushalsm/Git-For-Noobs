#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>

void init() {
    printf("Initialized empty Git repository in ");
    system("cd");
    printf("/.git/\n");
}

struct user{
    char *name;
    char *email;
};

struct user user;
/**
void config(){
    user.name = (char *)malloc(sizeof(char)*256);


}
*/
void print_error(char *error_message){
    fprintf(stderr,"[Error]: in %s\n",error_message);
    return 1;
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