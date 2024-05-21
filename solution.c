#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char input[100];

    while(1) {
        printf("Please insert command\n");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // replace enter character

        if (strcmp(input, "leave") == 0)
            break;

        int pid = fork(); //create child process

        if (pid == 0) { // if in child process
            char *allPaths = getenv("PATH");  // get all paths
            char *path = strtok(allPaths, ":"); // get first path
            char filePath[150];
            char *arr[] = {input, NULL};  // create argument array for execv

            while (path != NULL) {
                // Construct the full file path
                strcpy(filePath, path);
                strcat(filePath, "/");
                strcat(filePath, input);

                // Print the full file path
                printf("TRY: %s\n", filePath);

                FILE *filePtr = fopen(filePath, "r");  // Check if the file exists

                if (filePtr != NULL) { //if command exists at this path, execute and terminate child process
                    execv(filePath, arr);
                    exit(1);
                }

                path = strtok(NULL, ":"); // if no command exists at this path, continue to the next path
            }

            // If program reaches this point, input command was not found at any of the possible paths, so such command
            //does not exist. Print error and terminate child process

            printf("Error! No such command\n");
            exit(1);
        }

        else { // if in parent process
            wait(NULL); // Wait for the child process to complete
        }

    }
    return 0;
}
