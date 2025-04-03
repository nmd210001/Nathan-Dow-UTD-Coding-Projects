#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// These are pretty big to accomidate the test cases
#define MAX_ARGS 64
#define MAX_COMMANDS 64

#define HISTORY_SIZE 5// This constant represents the max amount of command lines that are stored in history
// I create a new structer called Command essetally a string with 100 character limit (C doesn't have strings and string.h just imports methods to manipulate char arrays) 
// This is so it is easier to keep track of each command line for the history Command

char* argv[MAX_ARGS]; //made argv global to avoid memory leaks and clear easier and not have to pass
int argc = 0;

// Creates a array of commands of size  100 which is the max
// Keep in mind in order to access the array of strings called command from history instead of the Command structer itself, use .command
char* commandHistory[HISTORY_SIZE];
int historyCount = 0; // This Global varable is to keep track of the current number of commands stored in commandHistory
int commandHistoryIndex = 0; // This Global varable is to keep track of the starting index of the circular array once it reaches HISTORY_SIZE

char* commands[MAX_COMMANDS];
int commandCount = 0;

void executeCommand();
void tokenizer(char* input);
void handleCommands();
void addToHistory(char *newCommand);
void printHistory();
void clearHistory();
void historyOffset(long offset);
void pipeOnlyTokenizer(char* input);
void handlePipes(int index, int cpFd);
void printStringArray(char **arr);

// prints a string array in case of debugging needs
void printStringArray(char **arr) {
    printf("[");
    for (int i = 0; arr[i] != NULL; i++) {
        printf("\"%s\"", arr[i]);
        if (arr[i + 1] != NULL) {
            printf(", ");
        }
    }
    printf("]\n");
}

//These are the clearers for each of the arrays
// Each one frees all the memory and sets the first index to NULL
// Each corisponding varible is also resset to 0
void clearCommands() {
    for (int i = 0; i < MAX_COMMANDS; i++) {
        if (commands[i] != NULL) {
            free(commands[i]);
            commands[i] = NULL;
        }
    }
    commandCount = 0;
}

void clearArgs() {
    for (int i = 0; i < MAX_ARGS; ++i) {
        if (argv[i] != NULL) {
            free(argv[i]);
            argv[i] = NULL;
        }
    }
    argc = 0;
}

void clearHistory() {
    for (int i = 0; i < HISTORY_SIZE; ++i) {
        free(commandHistory[i]);
        commandHistory[i] = NULL;
    }
    historyCount = 0;
    commandHistoryIndex = 0;
}

void historyOffset(long offset) {
    if (offset >= 0 && offset < HISTORY_SIZE) { //Makes sure it is between 0 and HistorySize - 1
        tokenizer(commandHistory[offset]);
    } else {
        fprintf(stderr, "Invalid offset specified");
        exit(EXIT_FAILURE);
    }
}

void addToHistory(char *newCommand) {// takes in an array of chars and creates a pointer to the first char to add the whole array
    /*  strcpy is a basic C command that  replaces a char array ("String") with another char array
    *  When the secound argument is a char pointer it assumes that the pointer points to the first char of the string
    *  It then replaces the first argument string with the characters starting at the pointer until it reaches a null terminator
    */
    // If history isn't full add as normal
    if (historyCount < HISTORY_SIZE) {
        commandHistory[historyCount] = strdup(newCommand);
        historyCount++; // history count should only increment when the History is not full
    } else { // Uses circular Array logic if it isn't full
        free(commandHistory[commandHistoryIndex]); // frees the current index going to replace the front of the array
        commandHistory[commandHistoryIndex] = strdup(newCommand); // sets that value to the new command
        commandHistoryIndex = (commandHistoryIndex + 1) % HISTORY_SIZE; // changes the head of the array to the appropirate index
    }
}

void printHistory() {
    // if the History Size isn't full print normally
    if (historyCount < HISTORY_SIZE) {
        for (int i = 0; i < historyCount; ++i) {
            printf("%d %s\n", i, commandHistory[i]);
        }
    } else { // else use commandHistoryIndex to keep track of the circular array
        int i = commandHistoryIndex;
        int count = 0;
        do {
            if (commandHistory[i]) {
                printf("%d %s\n", count, commandHistory[i]);
                count++;
            }
            i = (i + 1) % HISTORY_SIZE; // keeps track of the appropriate head index
        } while (count < historyCount);
    }
}

void executeCommand() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process executes the command
        if (execvp(argv[0], argv) < 0) {
            perror("execvp failed\n");
            exit(EXIT_FAILURE);
        }
        exit(0);
    } else {
        wait(NULL);
        return;
    }
}

void handleCommands() {
    /*  Uses strcmp which compares strings and see if any are the builtIn
    *  If they aren't then the default executeCommmand method is called
    *  handleCommands is called in main at the end
    */
    if (strcmp(argv[0], "cd") == 0) {
        if (argv[1] == NULL) {
            fprintf(stderr, "cd is missing a directory argument");
        }
        /*  Gives chdir, which is a standard c command, the secound arg
        *  chdir then attempts to change the directory to a directory with the name stored in argv[1]
        *  If this is successful it will return a 0
        *  else it will print out an error
        */
        if (chdir(argv[1]) != 0) {
            perror("cd ");
            //exit(EXIT_FAILURE);
        }
    } else if (strcmp(argv[0], "history") == 0) {
            if (argv[1] == NULL || strcmp(argv[1], "\n") == 0 || strcmp(argv[1], "\0") == 0){
                printHistory();
            }else if(strcmp(argv[1], "-c") == 0){
                clearHistory();
            }else {
                // Uses long functionality to make sure that the argument after history is a decimal number efficently
                char* endInt;
                long historyIndexLong = strtol(argv[1], &endInt, 10); 
                if(*endInt != '\0' || argv[1] == endInt) { //
                    fprintf(stderr, "Not a valid argument for history");
				    //exit(EXIT_FAILURE);
                }else{
                    historyOffset(historyIndexLong);
                }
            }
    } else {
        executeCommand();
    }
}

//Untimitly the same as tokenizer but avoids tokenizing by | as it is only getting one command
void pipeOnlyTokenizer(char* input) {
    clearArgs();
    char *argSavePtr;
    char *arg;
    
    arg = strtok_r(input, " ", &argSavePtr);
    while(arg != NULL){
        argv[argc++] = strdup(arg);
        arg = strtok_r(NULL, " ", &argSavePtr);              
    }
    handleCommands();
}

void handlePipes(int commandIndex, int cpFd) {
    if (commandIndex == commandCount) return; //base case: exits onece there are no more commands

    // Pipe Creation and intiazation
    int fd[2];
    if (commandIndex < commandCount - 1) { //Creates a pipe to the right of the command except if it's the last command (doesn't need one)
        if (pipe(fd) == -1) { //checks for pipe error
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Pipe proccesses
    pid_t pid = fork();
    if (pid == -1) { // checks for fork process error
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        if (cpFd != -1) { // If there's input from a previous pipe else execute STDIN as normal
            dup2(cpFd, STDIN_FILENO); // copys the variable file discriptor cpFd to STDIN_FILENO
            close(cpFd);
        }
        if (commandIndex < commandCount - 1) { //setup output for next command
            close(fd[0]); 
            dup2(fd[1], STDOUT_FILENO); // Depicates the write end of the current to STDOUT_FILENO
            close(fd[1]); 
        }
        pipeOnlyTokenizer(commands[commandIndex]); // tokenizes the current command at commandIndex to arguments
        exit(EXIT_SUCCESS); // exits the current iteration of handlePipes proceess
    } else { 
        if (cpFd != -1) {
            close(cpFd); if // cpFd exits close it
        }
        wait(NULL); // waits for ALL child proccesses to finish execution
        // For the last command in the sequence, ensure to close any opened pipes, if applicable.
        if (commandIndex < commandCount - 1) {
            close(fd[1]); // Close the write end of the pipe in the parent.
            // Recurse for the next command with the read end of the current pipe.
            handlePipes(commandIndex + 1, fd[0]);
        } else {
            // For the last command in the sequence, make sure no cpFd is for sure clossed after recurssion
            if (cpFd != -1) {
                close(cpFd); 
            }
        }
    }
}

void tokenizer(char* input) {
    commandCount = 0;
    char *commandLineSavePtr, *argSavePtr;
    char *commandLine, *arg;

    // sets the values of commands and argv before handling the commands
    commandLine = strtok_r(input, "|", &commandLineSavePtr); // uses strtox_r for '|' to tokenize the input into commands
    while (commandLine != NULL) {
        if (commands[commandCount] != NULL) {
            free(commands[commandCount]);
        }
        commands[commandCount] = strdup(commandLine);
        if (commands[commandCount] == NULL) {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            return;
        }
        commandCount++;
        argc = 0;
        arg = strtok_r(commandLine, " ", &argSavePtr); // uses strtox_r for ' ' to tokenize each command into args
        while (arg != NULL) {
            if (argc >= MAX_ARGS - 1) {
                fprintf(stderr, "Error: Exceeded maximum number of arguments.\n");
                return;
            }
            argv[argc++] = strdup(arg);
            arg = strtok_r(NULL, " ", &argSavePtr);              
        }                    
        commandLine = strtok_r(NULL, "|", &commandLineSavePtr);  
    }
    
    // avoids unneccisary proccesses and potential errors
    if (commandCount == 1)
        handleCommands();
    else
        handlePipes(0, -1);
}

int main() {
    char *input = NULL;
    size_t bufSize = 0;
    ssize_t nread;
    
    while (1) {
        printf("sish> ");
        nread = getline(&input, &bufSize, stdin);
        if (nread == -1) { // check for read error
            free(input);
            exit(EXIT_SUCCESS); //if user exits with control-D
        }

        //get rid of unwanted new line characters in the input
        if (input[nread - 1] == '\n') {
            input[nread - 1] = '\0';
        }
        
        addToHistory(input);
        if (strcmp(input, "exit") == 0) {
            // clear all memory allocated
            free(input);
            clearHistory();
            clearArgs();
            clearCommands();
            exit(EXIT_SUCCESS);
        }
        tokenizer(input);
        clearCommands();
        clearArgs();
    }     
    free(input);
    clearHistory();
    clearArgs();
    clearCommands();
    exit(EXIT_SUCCESS);
}
