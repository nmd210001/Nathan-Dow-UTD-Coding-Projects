#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 64
#define MAX_COMMANDS 32
const int HISTORY_SIZE = 5; // This constant represents the max amount of command lines that are stored in history
// I create a new structer called Command essetally a string with 100 character limit (C doesn't have strings and string.h just imports methods to manipulate char arrays) 
// This is so it is easier to keep track of each command line for the history Command

// Creates a array of commands of size  100 which is the max
// Keep in mind in order to access the array of strings called command from history instead of the Command structer itself, use .command
char* commandHistory[HISTORY_SIZE];
int historyCount = 0; // This Global varable is to keep track of the current number of commands stored in commandHistory
int commandHistoryIndex = 0;// This Global varable is to keep track of the starting index

char* commands[MAX_COMMANDS]; // This stores the commands instead of the arguments
int commandCount = 0; //This is to keep track of the whole constant original number of commands




void executeCommand(char **argv);
void tokenizer(char* input);
void handleCommands(char **argv);
void addToHistory(char *newCommand);
void printHistory();
void clearHistory();
void historyOffset(long offset, char** argv);
//This should just find the char pointer to be used like input in the main and run it like normal using strtok_r
void historyOffset(long offset, char** argv){
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
    if(historyCount < HISTORY_SIZE){
        commandHistory[historyCount] = strdup(newCommand);
        historyCount++;
    } else {
        free(commandHistory[commandHistoryIndex]);
        // Use strdup() to safely allocate space for the new command and copy it
        commandHistory[commandHistoryIndex] = strdup(newCommand);
        commandHistoryIndex = (commandHistoryIndex + 1) % HISTORY_SIZE;
    }
    printf("Added to history: %s\n", newCommand);    
}
/*

*/
void printHistory() {
    int i = commandHistoryIndex;
    int count = 0;
    printf("Command history:\n");
    do {
        if (commandHistory[i])
            printf("%d %s\n", count, commandHistory[i]);
        i = (i + 1) % HISTORY_SIZE;
        count++;
    } while (count < historyCount);
}
//NEED HELP
void clearHistory(){
	for (int i = 0; i < HISTORY_SIZE; ++i) {
		free(commandHistory[i]);
		commandHistory[i] = NULL;
    }
    historyCount = 0;
    commandHistoryIndex = 0;
}

void executeCommand(char **argv){
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        //printf("I'm the child\n");
        // Child process executes the command
        if (execvp(argv[0], argv) < 0) {
            perror("execvp failed\n");
            exit(EXIT_FAILURE);
        }
        printf("exit ");
        exit(0);
    }else {
        wait(NULL);
        return;
    }
}



void tokenizer(char* input){ // this would be dangerouse and unaffective to be used in handlePipes so a simpler pipeOnlyTokenizer is used
    char *argv[MAX_ARGS];
    argc = 0;

    char *commandLineSavePtr, *tokenSavePtr;
    char *commandLine, *token;

    commandLine = strtok_r(input, "|", &commandLineSavePtr); //tokenize the input command
    while(commandLine != NULL){
        free(commandHistory[commandCount]);
        commandHistory[commandCount] = strdup(commandLine);
        commandCount++;
        token = strtok_r(commandLine, " ", &tokenSavePtr);
        while(token != NULL){
            argv[argc++] = token;
            token = strtok_r(NULL, " ", &tokenSavePtr);              
        }                      
        commandLine = strtok_r(NULL, "|", &commandLineSavePtr);         
    }
    //printf("after while loop \n");
    argv[argc] = NULL; // Null terminate it
    //if(argc == 0) continue;
    //printf("ARGS = null\n");
    printf("There was %d commands \n",commandsCount);
    firstTime = false; //CommandGlobCount and Commands shouldn't change anymore so handlePipes works properly
    if(commandsCount == 1){
        handleCommands(argv);
    }
    else { 
        handlePipes(commands, 0, commandCount, -1);
    }
}
void handleCommands(char **argv) {
    /*  Uses strcmp which compares strings and see if any are the builtIn
    *  If they aren't then the default executeCommmand method is called
    *  handleCommands is called in main at the end
    */
        
    if (strcmp(argv[0], "exit") == 0) {
        exit(EXIT_SUCCESS); // Exit the shell
    } else if (strcmp(argv[0], "cd") == 0) {
        if(argv[1] == NULL){// checks if the directory to be changed to is NULL
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
            printf("I'm in history:\n%s\n", argv[1]);
            if (argv[1] == NULL){
                printf("I print it:\n");
                printHistory();
            }else if(strcmp(argv[1], "-c") == 0){
                printf("Im clear:\n");
                clearHistory();
                //Reference assignment
                /*
                else if(strlen(argv[1]) == 2){ //tests if the secound argument is 2 characters then passes to handle offset
                        historyOffset(argv[1]);
                }
                */
            }else {
                char* endInt;
                long historyIndexLong = strtol(argv[1], &endInt, 10);
                if(*endInt != '\0' || argv[1] == endInt) {
                    printf("nope:\n");
                    fprintf(stderr, "Not a valid argument for history");
				    exit(EXIT_FAILURE);
                }else{
                    historyOffset(historyIndexLong, argv);
                }
            }
        } else{
                executeCommand(argv); // Execute external commands
        }
}

void pipeOnlyTokenizer(char* input){
    char *argv[MAX_ARGS];
    argc = 0;
    
    char *tokenSavePtr;
    char *token;
    
    token = strtok_r(input, " ", &tokenSavePtr);
    while(token != NULL){
            argv[argc++] = token;
            token = strtok_r(NULL, " ", &tokenSavePtr);              
    }
    handleCommand(argv);
}

void handlePipes(char** commands, int index, int commandCount, int cpFd) {
    if (index == commandCount) return; // base case

    int fd[2];
    
    if (index < numCommands - 1) { //Checks if the command is not the last to create a pipe new pipe bc it's the only one that doesn't need to
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
        if (index < numCommands - 1) { //setup output for next command
            close(fd[0]); // Close read end of the pipe
            dup2(fd[1], STDOUT_FILENO); // Duplicate write end to STDOUT
            close(fd[1]); // Close original write end
        }
        pipeOnlyTokenizer(commands[index]); // runs the current command only avoiding the inital steps from the normal tokenizer
    } else { // Parent process
        if (cpFd != -1) 
            close(cpFd); // Close the read end of the previous pipe if it exists
        wait(NULL); // Wait for the child to finish
        if (index < numCommands - 1) close(fd[1]); // Close the write end of the current pipe
        if (index < numCommands - 1) recursivePipeExecution(commands, index + 1, numCommands, fd[0]); // Recurse for the next command
        else if (index == numCommands - 1) close(fd[0]); // If it was the last command, close the read end
    }
}

int main()
{
    char *input = NULL;
    size_t bufSize = 0;
    ssize_t nread;
    
    firstTime = true;
    
    while (1) {
        printf("sish> "); // promt
        isNewInput = true; // 
        nread = getline(&input, &bufSize, stdin); // read the user input
        //printf("after getline\n ");
        if(nread == -1){ // check for read error
            free(input);
            exit(EXIT_SUCCESS);//if user exits with control-D
        }

        if(input[nread - 1] == '\n'){ //check if the last character is a newLine
            input[nread - 1] = '\0';
        }
        addToHistory(input);
        tokenizer(input);
    }     
    free(input);
    exit(EXIT_SUCCESS);
}