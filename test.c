// #include <stdio.h>

// int main() {
//     FILE *fp1 = fopen("file1.txt", "r");
//     FILE *fp2 = fopen("file2.txt", "r");

//     if (fp1 == NULL || fp2 == NULL) {
//         printf("Error: Files not open\n");
//         return 1; // Return non-zero to indicate an error
//     }

//     // Fetch characters from both files
//     char ch1 = getc(fp1);
//     char ch2 = getc(fp2);

//     // Error tracking variables
//     int error = 0, pos = 0, line = 1;

//     // Iterate loop until end of either file
//     while (ch1 != EOF || ch2 != EOF) {
//         pos++;

//         // If both characters are newline, increment line number and reset position
//         if (ch1 == '\n' && ch2 == '\n') {
//             line++;
//             pos = 0;
//         }

//         // If characters differ, increment error count and print details
//         if (ch1 != ch2) {
//             error++;
//             printf("Line Number : %d \tError Position : %d\n", line, pos);
//         }

//         // Fetch the next character from both files
//         ch1 = getc(fp1);
//         ch2 = getc(fp2);
//     }

//     // Print total number of errors
//     printf("Total Errors : %d\n", error);

//     // Close both files
//     fclose(fp1);
//     fclose(fp2);

//     return 0; // Return zero to indicate success
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int main() {
    FILE *file1 = fopen("file1.txt", "r");
    FILE *file2 = fopen("file2.txt", "r");

    if (file1 == NULL || file2 == NULL) {
        printf("Error: Could not open one or both files.\n");
        return 1; // Return non-zero to indicate an error
    }

    char line1[MAX_LINE_LENGTH];
    char line2[MAX_LINE_LENGTH];
    int additions = 0, deletions = 0;

    // Read lines from both files
    while (fgets(line1, sizeof(line1), file1) != NULL) {
        if (fgets(line2, sizeof(line2), file2) == NULL) {
            // File2 ended before File1, so the rest of File1 are deletions
            additions++;
        } else {
            // Compare lines from both files
            if (strcmp(line1, line2) != 0) {
                // If lines differ, count this as a deletion in File1 and an addition in File2
                deletions++;
                additions++;
            }
        }
    }

    // If File2 still has lines after File1 ends, those are additions
    while (fgets(line2, sizeof(line2), file2) != NULL) {
        additions++;
    }

    // Output the results
    printf("Additions: %d\n", additions);
    printf("Deletions: %d\n", deletions);

    // Close the files
    fclose(file1);
    fclose(file2);

    return 0; // Return zero to indicate success
}

