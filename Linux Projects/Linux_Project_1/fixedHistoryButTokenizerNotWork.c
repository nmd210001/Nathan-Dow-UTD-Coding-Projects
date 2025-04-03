#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 64
#define MAX_COMMANDS 32
#define HISTORY_SIZE  100 // This constant represents the max amount of command lines that are stored in history
// I create a new structer called Command essetally a string with 100 character limit (C doesn't have strings and string.h just imports methods to manipulate char arrays) 
// This is so it is easier to keep track of each command line for the history Command

char* argv[MAX_ARGS];
int argc = 0;

// Creates a array of commands of size  100 which is the max
// Keep in mind in order to access the array of strings called command from history instead of the Command structer itself, use .command
char* commandHistory[HISTORY_SIZE];
int historyCount = 0; // This Global varable is to keep track of the current number of commands stored in commandHistory
int commandHistoryIndex = 0;// This Global varable is to keep track of the starting index

char* commands[MAX_COMMANDS]; // This stores the commands instead of the arguments
int commandCount = 0; //This is to keep track of the whole constant original number of commands




void executeCommand();
void tokenizer(char* input);
void handleCommands();
void addToHistory(char *newCommand);
void printHistory();
void clearHistory();
void historyOffset(long offset);
void pipeOnlyTokenizer(char* input);
void handlePipes(int index, int cpFd);
//This should just find the char pointer to be used like input in the main and run it like normal using strtok_r


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

void clearCommands(){
    printf("Currently in function: %s\n", __func__);
    for(int i = 0; i < MAX_COMMANDS; i++){
        //printf("%d %s \n",i, array[i]);
        if(commands[i] != NULL){
            free(commands[i]);
            commands[i] = NULL;
        }
    }
    commandCount = 0;
}

void clearArgs(){
    printf("Currently in function: %s\n", __func__);
	for (int i = 0; i < MAX_ARGS; ++i) {
        if(argv[i] != NULL){
            free(argv[i]);
            argv[i] = NULL;
        } 
    }
    argc = 0;
}

void clearHistory(){
    printf("Currently in function: %s\n", __func__);
	for (int i = 0; i < HISTORY_SIZE; ++i) {
        free(commandHistory[i]);
        commandHistory[i] = NULL;
    }

    historyCount = 0;
    commandHistoryIndex = 0;
}


void historyOffset(long offset){
        printf("Currently in function: %s\n", __func__);
		//int offsetInt = atoi(offset); // sets the number string to an int
        if(offset >= 0 && offset <= 99){ // you are going to have to duplicate the string from the argv[ind
				tokenizer(commandHistory[offset]);
               // exicuteCommand(offsetInt, commoandHistory[offsetInt]);//runs the command at the specified offset index from history
		}
        else{
                fprintf(stderr, "Invalid offset specified. Must be 0 - 99");
				exit(EXIT_FAILURE);
		}
}

void addToHistory( char *newCommand) {// takes in an array of chars and creates a pointer to the first char to add the whole array
    /*  strcpy is a basic C command that  replaces a char array ("String") with another char array
    *  When the secound argument is a char pointer it assumes that the pointer points to the first char of the string
    *  It then replaces the first argument string with the characters starting at the pointer until it reaches a null terminator
    */
    printf("Currently in function: %s\n", __func__);
    if (historyCount < HISTORY_SIZE) {
        commandHistory[historyCount] = strdup(newCommand);
        historyCount++;
    } else {
        free(commandHistory[commandHistoryIndex]);
        commandHistory[commandHistoryIndex] = strdup(newCommand);
        commandHistoryIndex = (commandHistoryIndex + 1) % HISTORY_SIZE;
    }
}
/*

*/
void printHistory() {
    printf("Currently in function: %s\n", __func__);
    if (historyCount < HISTORY_SIZE) {
        for (int i = 0; i < historyCount; ++i) {
            printf("%d %s\n", i, commandHistory[i]);
        }
    } else {
        int i = commandHistoryIndex;
        int count = 0;
        do {
            if (commandHistory[i]) {
                printf("%d %s\n", count, commandHistory[i]);
                count++;
            }
            i = (i + 1) % HISTORY_SIZE;
        } while (count < historyCount);
    }
}





void executeCommand(){
    printf("Currently in function: %s\n", __func__);
    printStringArray(argv);
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
    }else {
        wait(NULL);
        return;
    }
}

void handleCommands() {
    printf("Currently in function: %s\n", __func__);
    printStringArray(argv);

    /*  Uses strcmp which compares strings and see if any are the builtIn
    *  If they aren't then the default executeCommmand method is called
    *  handleCommands is called in main at the end
    */
    /*
    if (strcmp(argv[0], "exit") == 0) {
       free(input);
        clearHistory(); // Clear history before exiting
        exit(EXIT_SUCCESS);
    }
    */
    if (strcmp(argv[0], "cd") == 0) {
        if (argv[1] == NULL){// checks if the directory to be changed to is NULL
            fprintf(stderr, "cd is missing a directory argument"); // prints custom error msg regarding cd argument missing
        }
        /*  Gives chdir, which is a standard c command, the secound arg
        *  chdir then attempts to change the directory to a directory with the name stored in argv[1]
        *  If this is successful it will return a 0
        *  else it will print out an error
        */
        if(chdir(argv[1]) !=0){
			perror("cd "); //uses perror() which outputs the string given plus the errno value (useful info like 'No such file ore directory')
			exit(EXIT_FAILURE);
        }
        
        } else if (strcmp(argv[0], "history") == 0) {

            if (argv[1] == NULL || strcmp(argv[1], "\n") == 0 || strcmp(argv[1], "\0") == 0){
                printHistory();
            }else if(strcmp(argv[1], "-c") == 0){
                clearHistory();
            }else {
                char* endInt;
                long historyIndexLong = strtol(argv[1], &endInt, 10);
                if(*endInt != '\0' || argv[1] == endInt) {
                    fprintf(stderr, "Not a valid argument for history");
				    //exit(EXIT_FAILURE);
                }else{
                    historyOffset(historyIndexLong);
                }
            }
        } else{
                executeCommand(); // Execute external commands
        }
}

void pipeOnlyTokenizer(char* input){
    printf("Currently in function: %s\n", __func__);
    printStringArray(argv);
    clearArgs();
    printf("Currently in function: %s\n", __func__);
    printStringArray(argv);
    
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
    printf("Currently in function: %s\n", __func__);
    if (commandIndex == commandCount) return; // base case

    int fd[2];
    
    if (commandIndex < commandCount - 1) { //Checks if the command is not the last to create a pipe new pipe bc it's the only one that doesn't need to
        if (pipe(fd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) { // Child process
        if (cpFd != -1) { // If there's input from a previous pipe else execute STDIN as normal
            dup2(cpFd, STDIN_FILENO);
            close(cpFd);
        }
        if (commandIndex < commandCount - 1) { //setup output for next command
            close(fd[0]); // Close read end of the pipe
            dup2(fd[1], STDOUT_FILENO); // Duplicate write end to STDOUT
            close(fd[1]); // Close original write end
        }
        
        pipeOnlyTokenizer(commands[commandIndex]); // runs the current command only avoiding the inital steps from the normal tokenizer
        exit(EXIT_SUCCESS);
    } else { // Parent process
        if (cpFd != -1) {
            close(cpFd); // Close the read end of the previous pipe if it exists
        }
        wait(NULL); // Wait for the child to finish
        if (commandIndex < commandCount - 1) {
            close(fd[1]); // Close the write end of the pipe in the parent.
            // Recurse for the next command with the read end of the current pipe.
            handlePipes(commandIndex + 1, fd[0]);
        } else {
            // For the last command in the sequence, ensure to close any opened pipes, if applicable.
            if (cpFd != -1) {
                close(cpFd); // This would already be closed in most cases, but added for completeness.
            }
        }
        }
}

void tokenizer(char* input){ // this would be dangerouse and unaffective to be used in handlePipes so a simpler pipeOnlyTokenizer is used
    printf("Currently in function: %s\n", __func__);
    printStringArray(argv);
    clearArgs();
    printf("Currently in function: %s\n", __func__);
    printStringArray(argv);

    commandCount = 0;
    char *commandLineSavePtr, *argSavePtr;
    char *commandLine, *arg;

    commandLine = strtok_r(input, "|", &commandLineSavePtr); //tokenize the input command
    while(commandLine != NULL){
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
        arg = strtok_r(commandLine, " ",&argSavePtr);
        while(arg != NULL){
             if (argc >= MAX_ARGS - 1) {
                fprintf(stderr, "Error: Exceeded maximum number of arguments.\n");
                return;
            }
            argv[argc++] = strdup(arg);
            arg = strtok_r(NULL, " ", &argSavePtr);              
        }                    
        commandLine = strtok_r(NULL, "|", &commandLineSavePtr);  
               
    }
    //argv[argc] = NULL; // Null terminate it
    //if(argc == 0) continue;
    if (commandCount == 1){
        handleCommands();
    } else {
        printStringArray(commands);
        handlePipes( 0, -1);
    }
}

int main()
{
    char *input = NULL;
    size_t bufSize = 0;
    ssize_t nread;
    
    //firstTime = true;
    
    while (1) {
        printf("sish> "); // promt
        //isNewInput = true; // 
        nread = getline(&input, &bufSize, stdin); // read the user input
        if(nread == -1){ // check for read error
            free(input);
            exit(EXIT_SUCCESS);//if user exits with control-D
        }

        if(input[nread - 1] == '\n'){ //check if the last character is a newLine
            input[nread - 1] = '\0';
        }
        addToHistory(input);
        if (strcmp(input, "exit") == 0) {
            free(input);
            clearHistory();
            clearArgs();
            clearCommands(); // Clear history before exiting
            exit(EXIT_SUCCESS);
        }
        tokenizer(input);
        clearCommands();
    }     
    free(input);
    clearHistory();
    clearArgs();
    clearCommands();
    exit(EXIT_SUCCESS);
}