#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <dirent.h>
#include <windows.h>
#include <time.h>

// #include <openssl/md5.h>

// Run command
// gcc git-sim.c -o git-sim.exe -IC:/OpenSSL/include -LC:/OpenSSL/lib/VC/x64/MD -lssl -lcrypto
// gcc git-sim.c -o git-sim.exe

int stage_status = 0; // 0 = staging, 1 = not staging
char **stage_files;
int stage_files_no = 0;
int commit_state = 0; // 0 = commit, 1 = not commit

int compare_files(char *file1, char *file2) {
    FILE *fp1 = fopen(file1, "rb");
    FILE *fp2 = fopen(file2, "rb");

    if (fp1 == NULL || fp2 == NULL) {
        perror("Error opening file");
        return -1;
    }

    unsigned char buffer1[8192];
    unsigned char buffer2[8192];
    size_t bytes_read1, bytes_read2;

    while ((bytes_read1 = fread(buffer1, 1, 8192, fp1)) > 0 &&
           (bytes_read2 = fread(buffer2, 1, 8192, fp2)) > 0) {
        if (bytes_read1 != bytes_read2 || memcmp(buffer1, buffer2, bytes_read1) != 0) {
            fclose(fp1);
            fclose(fp2);
            return 0; // Files differ
        }
    }

    // Check if both files are completely read
    if (fread(buffer1, 1, 1, fp1) != 0 || fread(buffer2, 1, 1, fp2) != 0) {
        fclose(fp1);
        fclose(fp2);
        return 0; // Files differ in size
    }

    fclose(fp1);
    fclose(fp2);
    return 1; // Files are identical
}


// void compute_md5(char *file_name, unsigned char *result) {
//     FILE *file = fopen(file_name, "rb");
//     if (file == NULL) {
//         perror("fopen");
//         exit(EXIT_FAILURE);
//     }

//     MD5_CTX md5;
//     MD5_Init(&md5);

//     unsigned char buffer[8192];
//     size_t bytes_read;

//     while ((bytes_read = fread(buffer, 1, 8192, file)) != 0) {
//         MD5_Update(&md5, buffer, bytes_read);
//     }

//     MD5_Final(result, &md5);

//     fclose(file);
// }

// Changing few lines of code for prototype display purposes, change later
void clone(char *file_name) {
    char command[1000];
    snprintf(command, sizeof(command), "robocopy %s\\git-sim\\%s.git . /COPYALL /NFL /NDL /NJH /NJS", getenv("APPDATA"), file_name);
    system(command);
    snprintf(command, sizeof(command), "move %s.git %s.tar", file_name, file_name);
    system(command);
    snprintf(command, sizeof(command), "tar -xvf %s.tar", file_name, file_name);
    system(command);
    snprintf(command, sizeof(command), "del /Q %s.tar", file_name);
    system(command);
}

void pull() {
    char *working_dir = (char*)malloc(sizeof(char) * 1000);
    char *current_folder = (char*)malloc(sizeof(char) * 1000);
    if(GetCurrentDirectory(1000, working_dir)) {
            char *last_slash = strrchr(working_dir, '\\');
            if (last_slash != NULL) {
                current_folder = last_slash + 1;
        }
        else {
            current_folder = working_dir;
        }
    }
    char command[1000];
    snprintf(command, sizeof(command), "copy \"%s\\git-sim\\%s.git\" ..", getenv("APPDATA"), current_folder);
    system(command);
    snprintf(command, sizeof(command), "move ..\\%s.git ..\\%s.tar", current_folder, current_folder);
    system(command);
    // snprintf(command, sizeof(command), "tar -cf %s_check.tar ..\\%s", current_folder, current_folder);
    // system(command);

    // MD5 Method
    // char *file1, *file2;
    // snprintf(file1, sizeof(file1), "%s_check.tar", current_folder);
    // snprintf(file2, sizeof(file2), "%s.tar", current_folder);

    // unsigned char md5_file1[16];
    // unsigned char md5_file2[16];

    // compute_md5(file1, md5_file1);
    // compute_md5(file2, md5_file2);

    // if (memcmp(md5_file1, md5_file2, 16) == 0) {
    //     printf("Everything up-to-date!\n");
    // }
    // else {
    //     snprintf(command, sizeof(command), "tar -xvf %s.tar", current_folder, current_folder);
    //     system(command);
    // }

    // // Dirty In-efficient Method
    // char *file1, *file2;
    // snprintf(file1, sizeof(file1), "%s_check.tar", current_folder);
    // snprintf(file2, sizeof(file2), "%s.tar", current_folder);
    // FILE *fp1 = fopen(file1, "rb");
    // FILE *fp2 = fopen(file2, "rb");

    // int result = compare_files(file1, file2);
    // if (result == 1) {
    //     printf("Everything up-to-date!\n");
    // }
    // else {
    //     snprintf(command, sizeof(command), "tar -xvf ..\\%s.tar -C %s", current_folder, working_dir);
    //     system(command);
    // }

    // fclose(fp1);
    // fclose(fp2);

    snprintf(command, sizeof(command), "tar -xvf ..\\%s.tar -C \"%s\" --strip-components=1", current_folder, working_dir);
    system(command);

    snprintf(command, sizeof(command), "del /Q ..\\%s.tar", current_folder);
    system(command);
    // snprintf(command, sizeof(command), "del /Q %s_check.tar", current_folder, working_dir);
    // system(command);
}

void push() {
    char *working_dir = (char*)malloc(sizeof(char) * 1000);
    char *current_folder = (char*)malloc(sizeof(char) * 1000);
    if(GetCurrentDirectory(1000, working_dir)) {
            char *last_slash = strrchr(working_dir, '\\');
            if (last_slash != NULL) {
                current_folder = last_slash + 1;
        }
        else {
            current_folder = working_dir;
        }
        char command[1000];
        snprintf(command, sizeof(command), "tar -cf %s.tar ..\\%s", current_folder, current_folder);
        system(command);

        // snprintf(command, sizeof(command), "if exist %s\\git-sim\\%s (exit /b 0) else (exit /b 1)", current_folder);
        // int result = system(command);
        // if (result == 0) {
        //     snprintf(command, sizeof(command), "robocopy %s\\git-sim\\%s.git . /COPYALL /NFL /NDL /NJH /NJS", getenv("APPDATA"), current_folder);
        //     system(command);
        //     snprintf(command, sizeof(command), "move %s.git %s_check.tar", current_folder, current_folder);
        //     system(command);

        //     // MD5 Method
        //     // char *file1, *file2;
        //     // snprintf(file1, sizeof(file1), "%s_check.tar", current_folder);
        //     // snprintf(file2, sizeof(file2), "%s.tar", current_folder);

        //     // unsigned char md5_file1[16];
        //     // unsigned char md5_file2[16];

        //     // compute_md5(file1, md5_file1);
        //     // compute_md5(file2, md5_file2);

        //     // if (memcmp(md5_file1, md5_file2, 16) == 0) {
        //     //     printf("Everything up-to-date!\n");
        //     // }
        //     // else {
        //     //     snprintf(command, sizeof(command), "move %s.tar %s.git", current_folder, current_folder);
        //     //     system(command);
        //     //     snprintf(command, sizeof(command), "robocopy %s.git %s\\git-sim /MIR /IS /IT /NFL /NDL", current_folder,  getenv("APPDATA"));
        //     //     system(command);
        //     // }

        //     // Dirty In-efficient Method
        //     char *file1, *file2;
        //     snprintf(file1, sizeof(file1), "%s_check.tar", current_folder);
        //     snprintf(file2, sizeof(file2), "%s.tar", current_folder);
        //     FILE *fp1 = fopen(file1, "rb");
        //     FILE *fp2 = fopen(file2, "rb");

        //     int result = compare_files(file1, file2);
        //     if (result == 1) {
        //         printf("Everything up-to-date!\n");
        //     }
        //     else {
        //         snprintf(command, sizeof(command), "move %s.tar %s.git", current_folder, current_folder);
        //         system(command);
        //         snprintf(command, sizeof(command), "robocopy %s.git %s\\git-sim /MIR /IS /IT /NFL /NDL", current_folder,  getenv("APPDATA"));
        //         system(command);
        //     }

        //     fclose(fp1);
        //     fclose(fp2);
        // }
        snprintf(command, sizeof(command), "move %s.tar %s.git", current_folder, current_folder);
        system(command);
        snprintf(command, sizeof(command), "move /Y %s.git %s\\git-sim", current_folder,  getenv("APPDATA"));
        system(command);
    }
}

void commit(char *argv[], int argc) {
    srand((unsigned int)time(NULL));

    int length = 7, charset_size = 62, rand_num;
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char rand_str[length + 1];
    for (int i = 0; i < length; i++) {
        rand_str[i] = charset[rand() % charset_size];
    }
    rand_str[length] = '\0';

    printf("%d\n", stage_files_no);

    int additions = 0, deletions = 0;
    for(int i = 0; i < stage_files_no; i++) {
        char *file1_name = stage_files[i];
        char file2_name_temp[1000];
        snprintf(file2_name_temp, sizeof(file2_name_temp), "%s.bak", file1_name);
        char *file2_name = file2_name_temp;
        FILE *file1 = fopen(file1_name, "r");
        FILE *file2 = fopen(file2_name, "r");

        if (file1 == NULL || file2 == NULL) {
            printf("Error: Could not open one or both files.\n");
        }

        char line1[1024], line2[1024];

        while (fgets(line1, sizeof(line1), file1) != NULL) {
            if (fgets(line2, sizeof(line2), file2) == NULL) {
                additions++;
            }
            else {
                if (strcmp(line1, line2) != 0) {
                    deletions++;
                    additions++;
                }
            }
        }

        while (fgets(line2, sizeof(line2), file2) != NULL) {
            additions++;
        }

        // Close the files
        fclose(file1);
        fclose(file2);
    }

    if(strcmp(argv[2], "-m")) {
        printf("[master  (root-commit) %s] %s\n", rand_str, argv[3]);
        printf(" %d files changed,  5 insertions(+)\n");
        // if(additions > 0) {
        //     printf(" %d insertions(+),", additions);
        // }
        // if(deletions > 0) {
        //     printf(" %d deletions(-),", deletions);
        // }
        // printf("\n");
    }
}

void add(char *argv[], int argc) {
    stage_files = (char**)malloc(sizeof(char) * 1000);
    DIR* dir = opendir(".git");
    if(dir) {
        stage_status = 1;
        if(!strcmp(argv[2], ".")) {
            system("dir /b /od > stage_files.txt");
            FILE *file = fopen("stage_files.txt", "r"); 
            if (file == NULL) { 
                printf("Error opening file.\n"); 
            }
            char line[1000];
            while (fgets(line, sizeof(line), file) != NULL) {
                char command[1000];
                sscanf(line, "%s", stage_files[stage_files_no]);
                // Add these hidden files to .git file to make it more cleaner TODO
                snprintf(command, sizeof(command), "copy %s %s.bak", line);
                system(command);
                snprintf(command, sizeof(command), "attrib +h %s.bak", line);
                system(command);
                stage_files_no++;
            }
            fclose(file); 
            // system("del stage_files.txt");
        }
        else {
            for(int i = 2; i <= argc; i++)
            strcpy(stage_files[i - 2], argv[i + 1]);
        }
    }
    else {
        printf("This directory is not a git repositry!\nUse git-sim init to add the current directory as a repositry!\n");
    }
    system("del /Q stage_files.txt");
}

void init() {
    DIR* dir_check = opendir(".git");
    if(dir_check) {
        printf("This directory is already defined as git repositry!\n");
    }
    else {
        char *working_dir = (char*)malloc(sizeof(char) * 1000);
        if (GetCurrentDirectory(1000, working_dir)) {
            system("mkdir .git");
            system("attrib +h .git");
            printf("Initialized empty git repository in %s\\.git\\\n", working_dir);
        }
        else {
            printf("Error intializing %s as git repositry!\n", working_dir);
        }
    }
}

int main(int argc, char *argv[]) {
    // Add checking system for if git-sim already exists
    // char git_sim[1000];
    // snprintf(git_sim, sizeof(git_sim),"if not exist %s\\git-sim (mkdir %s\\git-sim)", getenv("APPDATA"), getenv("APPDATA"));
    // int initiate = system(git_sim);
    // if(initiate != - 1) {
    //     printf("Local hosted Git repository for simulation created!\n");
    //     if(argv[2] != NULL && argv[2][0] != '\0') {
    //         printf("\n");
    //     }
    // }

    // if (argc < 2) {
    //     printf("Usage: git-sim <command> [options]\n");
    //     return 1;
    // }
    if (!strcmp(argv[1], "--version")) {
        printf("git simulated version 1.0.0\n");
    }
    else if (!strcmp(argv[1], "init")) {
        init();
    }
    else if (!strcmp(argv[1], "add")) {
        add(argv, argc);
    }
    else if (1) {//!strcmp(argv[1], "commit")
        commit(argv, argc);
    }
    else if (!strcmp(argv[1], "push")) {
        push();
    }
    else if (!strcmp(argv[1], "pull")) {
        pull();
    }
    else if (!strcmp(argv[1], "clone")) {
        if (argc < 3) {
            printf("Usage: git-sim clone <url>\n");
            return 1;
        }
        clone(argv[2]);
    }
    else {
        printf("I do not understand this command!\n");
        return 1;
    }
    
    return 0;
}