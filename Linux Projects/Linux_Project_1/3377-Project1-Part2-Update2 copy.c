{\rtf1\ansi\ansicpg1252\cocoartf2639
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;\red99\green226\blue255;\red251\green30\blue255;\red0\green0\blue0;
\red128\green255\blue167;\red50\green244\blue241;\red224\green228\blue9;\red235\green239\blue24;\red254\green215\blue216;
\red19\green150\blue163;}
{\*\expandedcolortbl;;\cssrgb\c44519\c90662\c100000;\cssrgb\c100000\c29675\c100000;\csgray\c0;
\cssrgb\c55755\c100000\c71263;\cssrgb\c19572\c95778\c95688;\cssrgb\c90000\c90000\c0;\cssrgb\c93803\c93590\c10850;\cssrgb\c100000\c87708\c87701;
\cssrgb\c0\c65000\c70000;}
\margl1440\margr1440\vieww23080\viewh13600\viewkind0
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0

\f0\fs22 \cf2 \CocoaLigature0 #include \cf3 <stdio.h>\cf4 \
\cf2 #include \cf3 <stdlib.h>\cf4 \
\cf2 #include \cf3 <string.h>\cf4 \
\cf2 #include \cf3 <sys/types.h>\cf4 \
\cf2 #include \cf3 <sys/wait.h>\cf4 \
\cf2 #include \cf3 <unistd.h>\cf4 \
\
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0
\cf5 const\cf4  \cf5 int\cf4  HISTORY_SIZE = \cf3 100\cf4  \cf6 // This constant represents the max amount of command lines that are stored in history\cf4 \
\cf6 // I create a new structer called Command essetally a string with 100 character limit (C doesn't have strings and string.h just imports methods to manipulate char arrays) \cf4 \
\cf6 // This is so it is easier to keep track of each command line for the history Command\cf4 \
\
\cf6 // Creates a array of commands of size  100 which is the max\cf4 \
\cf6 // Keep in mind in order to access the array of strings called command from history instead of the Command structer itself, use .command\cf4 \
\cf5 char\cf4 * commandHistory[HISTORY_SIZE];\
\cf5 int\cf4  historyCount = \cf3 0\cf4 ; \cf6 // This Global varable is to keep track of the current number of commands stored in commandHistory\
int commandHistoryIndex = 0; // This Global varable is to keep track of the starting index\
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0
\cf4 \
/*\
	if(historyCount < HISTORY_SIZE)\{\
        commandHistory[historyCount] = strdup(newCommand);\
        historyCount++;\
    \} else \{\
		free(commandHistory[historyIndex];\
        strcpy(commandHistory[historyIndex], newCommand);\
        historyIndex = (historyIndex + 1) % HISTORY_SIZE;\
    \}\
    printf("Added to history: %s\\n", newCommand);\
*/\
\
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0
\cf5 void\cf4  addToHistory(\cf5 const\cf4  \cf5 char\cf4  *newCommand) \{\cf6 // takes in an array of chars and creates a pointer to the first char to add the whole array\cf4 \
        \cf6 /*  strcpy is a basic C command that  replaces a char array ("String") with another char array\cf4 \
\cf6          *  When the secound argument is a char pointer it assumes that the pointer points to the first char of the string\cf4 \
\cf6          *  It then replaces the first argument string with the characters starting at the pointer until it reaches a null terminator\cf4 \
\cf6          */\cf4 \
        \cb7 if\cb1  (historyCount < HISTORY_SIZE) \{\
                strcpy(commandHistory[historyCount], newCommand);\
                historyCount++;\
        \cf6 // use circular array %HISTORY_SIZE;\cf4 \
        \cf6 // free the 99 index before using strcpy to reassign new Command\cf4 \
        \} \cf8 else\cf4  \{ \cf6 //Remove the oldest command to make room for the new one\cf4 \
                \cf8 for\cf4  (\cf5 int\cf4  i = \cf3 1\cf4 ; i < HISTORY_SIZE; i++)\
                        strcpy(commandHistory[i - \cf3 1\cf4 ], commandHistory[i]);\cf6 // Replaces each command with the last command except for the oldest\cf4 \
                free(commandHistory[HISTORY_SIZE - \cf3 1\cf4 ]);\
                strcpy(commandHistory[HISTORY_SIZE - \cf3 1\cf4 ], newCommand); \cf6 // Replaces the newest index Command with the new command\cf4 \
                \cf6 // historyCount is not changed because it stays at 100\cf4 \
        \}\
\}\
/*\
int i = historyIndex;\
    int count = 0;\
    printf("Command history:\\n");\
    do \{\
        if (commandHistory[i])\
            printf("%d %s\\n", count, commandHistory[i]);\
        i = (i + 1) % HISTORY_SIZE;\
        count++;\
    \} while (count < historyCount);\
*/\
\cf5 void\cf4  printHistory() \{\
        \cf8 for\cf4  (\cf5 int\cf4  i = \cf3 0\cf4 ; i < historyCount; i++) \{\
                \cf6 //  uses printf() to print the index of the command and the string storred in each Command of commandHistory\cf4 \
                printf(\cf3 "\cf9 %d\cf3  \cf9 %s\\n\cf3 "\cf4 , i,commandHistory[i]);\
        \}\
\}\
\cf6 //NEED HELP\cf4 \
\cf5 void\cf4  clearHistory()\{\
	\cf8 for (int i = 0; i < HISTORY_SIZE; ++i) \{\
		free(commandHistory[i]);\
		commandHistory[i] = NULL;\
    \}\
    historyCount = 0;\
    historyIndex = 0;\cf4 \
\}\
\
\
\
\cf6 //#define MAX_ARGS 64\cf4 \
\cf6 //I left commented prinf statements just in case this code needs debuging once more\cf4 \
\cf5 void\cf4  executeCommand(\cf5 char\cf4  **argv,)\{\
    pid_t pid = fork();\
    \cb7 if\cb1  (pid == -\cf3 1\cf4 ) \{\
        perror(\cf3 "Fork failed\cf9 \\n\cf3 "\cf4 );\
        exit(\cf3 EXIT_FAILURE\cf4 );\
    \} \cf8 else\cf4  \cb7 if\cb1  (pid == \cf3 0\cf4 ) \{\
        \cf6 //printf("I'm the child\\n");\cf4 \
        \cf6 // Child process executes the command\cf4 \
        \cb7 if\cb1  (execvp(argv[\cf3 0\cf4 ], argv) < \cf3 0\cf4 ) \{\
            perror(\cf3 "execvp failed\cf9 \\n\cf3 "\cf4 );\
            exit(\cf3 EXIT_FAILURE\cf4 );\
        \}\
        printf(\cf3 "exit "\cf4 );\
        exit(\cf3 0\cf4 );\
    \}\cf8 else\cf4  \{\
        wait(\cf3 NULL\cf4 );\
        \cf8 return\cf4 ;\
    \}\
\}\
\
\
\cf6 //This should just find the char pointer to be used like input in the main and run it like normal using strtok_r\cf4 \
\cf5 void\cf4  historyOffset(\cf5 const\cf4  \cf5 char\cf4  *offset, char* argv[])\{\
		for (int index = 0, i < strlen(offset); index++)\{\
			if(!isdigit(str[i]))\{\
				fprintf(\cf3 stderr\cf4 , \cf3 "Not a valid argument for history"\cf4 );\
				 exit(\cf3 EXIT_FAILURE\cf4 );\
			\}\
		\} \
		\cf5 int\cf4  offsetInt = atoi(offset); \cf6 // sets the number string to an int\cf4 \
        \cb7 if\cb1 (offsetInt >= \cf3 0\cf4  && num <= \cf3 99\cf4 )\{ // you are going to have to duplicate the string from the argv[ind\
				\
                exicuteCommand(offsetInt, commoandHistory[offsetInt]);\cf6 //runs the command at the specified offset index from history\
		\}\cf4 \
        \cf8 else\{\cf4 \
                fprintf(\cf3 stderr\cf4 , \cf3 "Invalid offset specified. Must be 0 - 99"\cf4 );\
				exit(\cf3 EXIT_FAILURE\cf4 );\
		\}\
\}\
\
\
\cf5 void\cf4  handleCommands(\cf5 char\cf4  *argv[],) \{\
        \cf6 /*  Uses strcmp which compares strings and see \cf4 \cb7 if\cf6 \cb1  any are the builtIn\cf4 \
\cf6          *  If they aren't then the default executeCommmand method is called\cf4 \
\cf6          *  handleCommands is called in main at the end\cf4 \
\cf6          */\cf4 \
        \cb7 if\cb1  (strcmp(argv[\cf3 0\cf4 ], \cf3 "exit"\cf4 ) == \cf3 0\cf4 ) \{\
            exit(\cf3 EXIT_SUCCESS\cf4 ); \cf6 // Exit the shell\cf4 \
        \} \cf8 else\cf4  \cb7 if\cb1  (strcmp(argv[\cf3 0\cf4 ], \cf3 "cd"\cf4 ) == \cf3 0\cf4 ) \{\
                \cb7 if\cb1 (argv[\cf3 1\cf4 ] == \cf3 NULL\cf4 )\{\cf6 // checks \cf4 \cb7 if\cf6 \cb1  the directory to be changed to is NULL\cf4 \
                        fprintf(\cf3 stderr\cf4 , \cf3 "cd is missing a directory argument"\cf4 ); \cf6 // prints custom error msg regarding cd argument missing\
						\cf4 exit(\cf3 EXIT_FAILURE\cf4 );\
                \}\
                \cf6 /*  Gives chdir, which is a standard c command, the secound arg\cf4 \
\cf6                  *  chdir then attempts to change the directory to a directory with the name stored in argv[1]\cf4 \
\cf6                  *  If this is successful it will return a 0\cf4 \
\cf6                  *  else it will print out an error\cf4 \
\cf6                  */\cf4 \
                \cb7 if\cb1 (chdir(argv[\cf3 1\cf4 ]) !=\cf3 0\cf4 )\{\
					perror(\cf3 "cd "\cf4 ); \cf6 //uses perror() which outputs the string given plus the errno value (useful info like 'No such file ore directory')\
					\cf4 exit(\cf3 EXIT_FAILURE\cf4 );\
                \}\
        \} \cf8 else\cf4  \cb7 if\cb1  (strcmp(argv[\cf3 0\cf4 ], \cf3 "history"\cf4 ) == \cf3 0\cf4 ) \{\
                \cb7 if\cb1  (strcmp(argv[\cf3 1\cf4 ], \cf3 ""\cf4 ) == \cf3 0\cf4 )\
                        printHistory();\
                \cf8 else\cf4  \cb7 if\cb1 (strcmp(argv[\cf3 1\cf4 ], \cf3 "-c"\cf4 ) == \cf3 0\cf4 )\
                        clearHistory();\
                \cf6 //Reference assignment\cf4 \
                \cf8 else\cf4  \cb7 if\cb1 (strlen(argv[1]) == 2)\{ //tests if the secound argument is 2 characters then passes to handle offset\cf6 \
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0
\cf4                         historyOffset(argv[\cf3 1\cf4 ]);\
                \}\
                \cf8 else\cf4 \
                        fprintf(\cf3 stderr\cf4 , \cf3 "Not a valid argument for history"\cf4 );\
					   exit(\cf3 EXIT_FAILURE\cf4 );\
        \} \cf8 else\cf4 \
                executeCommand(argv); \cf6 // Execute external commands\cf4 \
\}\
\
/* //Pipe code for reference\
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0
\cf5 int\cf4  handlePipe (\cf5 int\cf4  argc, \cf5 char\cf4  *argv[])\
\{\
    \cf5 int\cf4  pipefd[\cf3 2\cf4 ];\
    pid_t cpid;\
    \cf5 char\cf4  buf;\
\
    \cb7 if\cb1  (argc != \cf3 2\cf4 ) \{\
     fprintf(\cf3 stderr\cf4 , \cf3 "Usage: \cf9 %s\cf3  <string>\cf9 \\n\cf3 "\cf4 , argv[\cf3 0\cf4 ]);\
     exit(\cf3 EXIT_FAILURE\cf4 );\
    \}\
\
    \cb7 if\cb1  (pipe(pipefd) == -\cf3 1\cf4 ) \{\
        perror(\cf3 "pipe"\cf4 );\
        exit(\cf3 EXIT_FAILURE\cf4 );\
    \}\
\
    cpid = fork();\
    \cb7 if\cb1  (cpid == -\cf3 1\cf4 ) \{\
        perror(\cf3 "fork"\cf4 );\
        exit(\cf3 EXIT_FAILURE\cf4 );\
    \}\
\
    \cb7 if\cb1  (cpid == \cf3 0\cf4 ) \{    \cf6 /* Child reads from pipe */\cf4 \
        close(pipefd[\cf3 1\cf4 ]);          \cf6 /* Close unused write end */\cf4 \
\
        \cf8 while\cf4  (read(pipefd[\cf3 0\cf4 ], &buf, \cf3 1\cf4 ) > \cf3 0\cf4 )\
            write(STDOUT_FILENO, &buf, \cf3 1\cf4 );\
\
        write(STDOUT_FILENO, \cf3 "\cf9 \\n\cf3 "\cf4 , \cf3 1\cf4 );\
        close(pipefd[\cf3 0\cf4 ]);\
        exit(\cf3 EXIT_SUCCESS\cf4 );\
\
    \} \cf8 else\cf4  \{            \cf6 /* Parent writes argv[1] to pipe */\cf4 \
        close(pipefd[\cf3 0\cf4 ]);          \cf6 /* Close unused read end */\cf4 \
        write(pipefd[\cf3 1\cf4 ], argv[\cf3 1\cf4 ], strlen(argv[\cf3 1\cf4 ]));\
        close(pipefd[\cf3 1\cf4 ]);          \cf6 /* Reader will see EOF */\cf4 \
        wait(\cf3 NULL\cf4 );                \cf6 /* Wait for child */\cf4 \
        exit(\cf3 EXIT_SUCCESS\cf4 );\
    \}\
\}\
*/\
\
\cf5 int\cf4  main()\
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0
\cb10 \{\cb1 \
    \cf5 char\cf4  *input = \cf3 NULL\cf4 ;\
    \cf5 size_t\cf4  bufSize = \cf3 0\cf4 ;\
    \cf5 ssize_t\cf4  nread;\
\
    \cf5 char\cf4  *commandLineSavePtr;\cf6 //,*commandSavePtr, *tokenSavePtr;\cf4 \
    \cf5 char\cf4  *commandLine;\cf6 //, *command, *token;\cf4 \
    \cf8 while\cf4  (\cf3 1\cf4 ) \{\
        printf(\cf3 "sish> "\cf4 ); \cf6 // promt\cf4 \
\
        nread = getline(&input, &bufSize, \cf3 stdin\cf4 ); \cf6 // read the user input\cf4 \
        \cf6 //printf("after getline\\n ");\cf4 \
        \cb7 if\cb1 (nread == -\cf3 1\cf4 )\{ \cf6 // check for read error\cf4 \
            free(input);\
            exit(\cf3 EXIT_SUCCESS\cf4 );\cf6 //\cf4 \cb7 if\cf6 \cb1  user exits with control-D\cf4 \
        \}\
\
        \cb7 if\cb1 (input[nread - \cf3 1\cf4 ] == \cf9 '\\n'\cf4 )\{ \cf6 //check \cf4 \cb7 if\cf6 \cb1  the last character is a newLine\cf4 \
            input[nread - \cf3 1\cf4 ] = \cf9 '\\0'\cf4 ;\
        \}\
        \cf6 //printf("after removing newLine\\n ");\cf4 \
        \cf5 int\cf4  i = \cf3 0\cf4 ;\
        \cf5 int\cf4  found = \cf3 0\cf4 ;\
        \cf8 while\cf4 (input[i] != \cf9 '\\0'\cf4 )\{\
            \cb7 if\cb1 (input[i] == \cf3 '|'\cf4 )\{ \cf6 //checking \cf4 \cb7 if\cf6 \cb1  the command involves pipe or not\cf4 \
                found = \cf3 1\cf4 ;\
                \cf6 //printf("Pipe was found\\n ");\cf4 \
                \cf8 break\cf4 ;\
\
            \}\
            i++;\
        \}\
\
        \cb7 if\cb1 (!found)\{\
            \cf6 //do stuff\cf4 \
            \cf6 //printf("pipe wasn't found and you are in \cf4 \cb7 if\cf6 \cb1  statenemnt\\n ");\cf4 \
            \cf5 char\cf4  *argv[\cf3 64\cf4 ];\
            \cf5 int\cf4  argc = \cf3 0\cf4 ;\
            commandLine = strtok_r(input, \cf3 " "\cf4 , &commandLineSavePtr); \cf6 //tokenize the input command\cf4 \
            \cf8 while\cf4 (commandLine != \cf3 NULL\cf4 )\{\
                \cf6 //command = strtok_r(command, "|", &commandSavePtr);\cf4 \
                \cf6 //while(command != NULL)\{\cf4 \
                argv[argc++] = commandLine;\
                commandLine = strtok_r(\cf3 NULL\cf4 , \cf3 " "\cf4 , &commandLineSavePtr);\
           \cf6 // \}\cf4 \
            \}\
            \cf6 //printf("after while loop \\n");\cf4 \
            argv[argc] = \cf3 NULL\cf4 ; \cf6 // Null terminate it\cf4 \
            \cb7 if\cb1 (argc == \cf3 0\cf4 ) \cf8 continue\cf4 ;\
            \cf6 //printf("ARGS = null\\n");\cf4 \
            handleCommands(argv);\
\
        \}\
        \cf6 //execute pipe thins for part 3\cf4 \
        \cf8 else\cf4 \{\
                \cf6 //empty right now\cf4 \
        \}\
\
    \}\
    free(input);\
    exit(\cf3 EXIT_SUCCESS\cf4 );\
\}\
\
/*\
\
 // Implement history [offset]\
        \
        /*below conditional checks:\
            -are the first 8 characters "history "?\
            -is the user actually trying to clear history?\
            -is the character after "history " a number?\
        */\
        if (strncmp(line, "history ", 8) == 0 && (strcmp(line, "history -c") != 0)\
              && (line[8] >= '0') && (line[8] <= '9')) \{\
            \
            //point to the start of where the numbers are in the command\
            char* histIndexStr = line + 8;\
            \
            //convert histIndexStr to an integer value\
            int histIndex = atoi(histIndexStr);\
            \
            //check for valid bounds\
            if (histIndex < 0 || histIndex >= history_count) \{\
                fprintf(stderr, "Invalid index\\n");\
                continue;\
            \} else \{\
                //calculate the actual index of the current command\
                int actualIndex = (history_front + histIndex) % HISTORY_SIZE;\
                char *tempLine = malloc(strlen(line) + 1); // +1 for the null terminator\
                if (tempLine == NULL) \{\
                    // Handle memory allocation failure\
                    printf("FAILED\\n");\
                    exit(EXIT_FAILURE);\
                \}\
                strcpy(tempLine, line);\
                \
                free(line); // Free the old line\
                line = strdup(history[actualIndex]); // Duplicate the command from history\
                \
                //add the templine to history\
                add_to_history(tempLine);\
                \
                free(tempLine);\
            \}\
        \} else \{\
            // Add user input (or command retrieved from history) to history\
            add_to_history(line);        \
        \}\
\
void recursivePipeExecution(char** commands, int index, int numCommands, int inFd) \{\
    if (index == numCommands) return; // Base case: reached the end of commands\
\
    int fd[2];\
    if (index < numCommands - 1) \{ // If not last command, create a pipe\
        if (pipe(fd) == -1) \{\
            perror("pipe");\
            exit(EXIT_FAILURE);\
        \}\
    \}\
\
    pid_t pid = fork();\
    if (pid == -1) \{\
        perror("fork");\
        exit(EXIT_FAILURE);\
    \} else if (pid == 0) \{ // Child process\
        if (inFd != -1) \{ // If there's input from a previous pipe\
            dup2(inFd, STDIN_FILENO);\
            close(inFd);\
        \}\
        if (index < numCommands - 1) \{ // Not the last command, setup output for next command\
            close(fd[0]); // Close read end of the pipe\
            dup2(fd[1], STDOUT_FILENO); // Duplicate write end to STDOUT\
            close(fd[1]); // Close original write end\
        \}\
\
        char* args[MAX_ARGS];\
        tokenizeCommand(commands[index], args);\
        execvp(args[0], args);\
        perror("execvp");\
        exit(EXIT_FAILURE);\
    \} else \{ // Parent process\
        if (inFd != -1) close(inFd); // Close the read end of the previous pipe if it exists\
        wait(NULL); // Wait for the child to finish\
        if (index < numCommands - 1) close(fd[1]); // Close the write end of the current pipe\
        if (index < numCommands - 1) recursivePipeExecution(commands, index + 1, numCommands, fd[0]); // Recurse for the next command\
        else if (index == numCommands - 1) close(fd[0]); // If it was the last command, close the read end\
    \}\
\}\
*/\
}