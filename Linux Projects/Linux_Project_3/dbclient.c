/*
 * Nathan Dow nmd210001
 * Leonid Mateush lmm220016
*/

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>

#include "msg.h"

#define BUF 256

int put(int socket_fd, bool isFileInput, char *name, int id);
int get(int socket_fd,bool isFileInput, int id);
int userInput(int socket_fd);
int fileInput(int socket_fd, const char *fileName);
//Main
void Usage(char *progname);
int LookupName(char *name, unsigned short port, struct sockaddr_storage *ret_addr, size_t *ret_addrlen);
int Connect(const struct sockaddr_storage *addr,  const size_t addrlen, int *ret_fd);

int put(int socket_fd, bool isFileInput, char *nameLine, int idLine) { //PUT for the client, writes the informaiton provided by the user to the server
  //Checks if this funciton was runned by file or user
  //If user, let's it put its own data, otherwise just uses what file provided
  char name[BUF];
  int id;
  if (!isFileInput) {
    //clear input buffer, doesn't let enter name before this
    while (getchar() != '\n');

    printf("Enter the name: ");
    if (fgets(name, BUF, stdin) == NULL) {
        printf("Error reading name\n");
        return EXIT_FAILURE;
    }
    name[strcspn(name, "\n")] = '\0';

    printf("Enter the id: ");
    if (scanf("%d", &id) != 1) {
        printf("Error reading ID \n");
        return EXIT_FAILURE;
    }
  } else{ 
    //If it's a file and we wan't same viuals as a user, we can uncomment this
    //printf("Enter the name: %s \n", name);
    //printf("Enter the id: %d \n", id);
    strcpy(name, nameLine);
    id = idLine;
  }
   
 
  //Construct the PUT message
  struct msg message;
  message.type = PUT;
  message.rd.id = id;
  strcpy(message.rd.name, name);

  //Writes the provided information to the server
  int wres = write(socket_fd, &message, sizeof(message));
  if (wres == 0) { //Check for errors
    printf("socket closed prematurely\n");
    close(socket_fd);
    return EXIT_FAILURE;
  }
  if (wres == -1) {
    printf("socket write failure\n");
    close(socket_fd);
    return EXIT_FAILURE;
  }

  //Receive response from server
  int res = read(socket_fd, &message, sizeof(message));
  if (res == 0) { //Check for errors
    printf("socket closed prematurely \n");
    close(socket_fd);
    return EXIT_FAILURE;
  }
  if (res == -1) {
    printf("socket read failure \n");
    close(socket_fd);
    return EXIT_FAILURE;
  }

  //Process the response
  if (message.type == SUCCESS) {
    printf("put success.\n");
  } else {
    printf("put failed.\n");
  }

  //Return sucess if no errors
  return EXIT_SUCCESS;
}

int get(int socket_fd, bool isFileInput, int idLine) { // GET for the client, asks for ID and then sends it to server to search
  int id;
  // Checks if this funciton was runned by file or user
  // If user, let's it put its own data, otherwise just uses what file provided
  if (!isFileInput) {
    printf("Enter the id: ");
    scanf("%d", &id);
  } else { 
    //If it's a file and we wan't same viuals as a user, we can uncomment this
    //printf("Enter the id: %d \n", id);
    id = idLine;
  }


  //Construct the GET message
  struct msg message;
  message.type = GET;
  message.rd.id = id;

  //Writes the provided information to the server
  int wres = write(socket_fd, &message, sizeof(message));
  if (wres == 0) { //Check for errors
    printf("socket closed prematurely \n");
    close(socket_fd);
    return EXIT_FAILURE;
  }
  if (wres == -1) {
    printf("socket write failure \n");
    close(socket_fd);
    return EXIT_FAILURE;
  }

  //Receive response from server
  int res = read(socket_fd, &message, sizeof(message));
  if (res == 0) { //Check for errors
    printf("socket closed prematurely \n");
    close(socket_fd);
    return EXIT_FAILURE;
  }
  if (res == -1) {
    printf("socket read failure \n");
    close(socket_fd);
    return EXIT_FAILURE;
  }

  // Process the response
  if (message.type == SUCCESS) {
    printf("name: %s", message.rd.name);
    printf("id: %d \n", message.rd.id);
    printf("get success\n");
  } else {
    printf("get failed\n");
  }

  // reset message name and id
  for (int i = 0; i < MAX_NAME_LENGTH; i++){
    message.rd.name[i] = '\0';
  }
  message.rd.id = 0;

  //Return sucess if no errors
  return EXIT_SUCCESS;
}

int userInput(int socket_fd) { //Menu for user to use
  //If userinput, then not files input
  bool isFileInput = false;
  //Used for user unputed name and id
  char name[BUF];
  int id = 0;

  //Loop trough the menu untill clinet doesn't stop
  while (1) {
    //Prompt user for operation choice
    printf("\nEnter your choice (1 to put, 2 to get, 0 to quit): ");
    int choice;
    scanf("%d", &choice);

    if (socket_fd == 0) { //Check if server is on before going to swtich
      printf("socket closed prematurely\n");
      close(socket_fd);
      return -1;
    }
    if (socket_fd == -1) {
      printf("socket write failure\n");
      close(socket_fd);
      return -1;
    }
    
    //Depending on choice entered by user chooses the right action
    switch (choice) {
    case 1: //Run put
      put(socket_fd, isFileInput, name, id);
      break;
    case 2: //Run get
      get(socket_fd, isFileInput, id);
      break;
    case 0: //Quit the client
      // Clean up and exit when user wants to
      close(socket_fd);
      return 0;
    default:
      printf("Invalid choice\n");
    }
  }

  //If we get to this point for some reason, return basically error
  return -1;
}

int fileInput(int socket_fd, const char *fileName) { //Menu for file input
  //Open a specified file by user starting a clinet
  FILE *file = fopen(fileName, "r");
  if (file == NULL) {  //Check for errors
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  //Char to store each line
  char line[BUF];
  //since it's fileInput, the fileInput is true
  bool isFileInput = true;
  //Char to store name
  char name[BUF];
  //int to store id
  int id;

  //Loops trought the file until it can't, reads line by line
  while (fgets(line, sizeof(line), file) != NULL) {
    int choice;
   
    if (sscanf(line, "%d", &choice) == 1) {
      // Since it's a file, no visual menu is needed, but if needed we can uncomment this
      //printf("\nEnter your choice (1 to put, 2 to get, 0 to quit): %d \n", choice);

      switch (choice) {
      case 1: //Run put
        if (fgets(name, sizeof(name), file) != NULL && fgets(line, sizeof(line), file) != NULL) { //reads next 2 lines to get name and ID
          // Remove newline character
          name[strcspn(name, "\n")] = '\0';
          //Conver id to int
          id = atoi(line);
          //Run Put with given info
          put(socket_fd, isFileInput, name, id);
        }
        break;
      case 2: //Run get
        if (fgets(line, sizeof(line), file) != NULL) { //reads next line to get ID
          id = atoi(line);
          get(socket_fd, isFileInput, id);
        }
        break;
      case 0: //Quit the client
        fclose(file);
        close(socket_fd);
        return 0;
      default:
        printf("Invalid choice\n");
      }
    }
    else{
      printf("Invalid line format\n");
    }
  }

  //If we get to this point for some reason, return basically error and close file
  fclose(file);
  return -1;
}

int main(int argc, char **argv) {
    if (argc != 3 && argc  != 4) {
        Usage(argv[0]);
    }

    unsigned short port = 0;
    if (sscanf(argv[2], "%hu", &port) != 1) {
        Usage(argv[0]);
    }

    // Get an appropriate sockaddr structure.
    struct sockaddr_storage addr;
    size_t addrlen;
    if (!LookupName(argv[1], port, &addr, &addrlen)) {
        Usage(argv[0]);
    }

    // Connect to the remote host.
    int socket_fd;
    if (!Connect(&addr, addrlen, &socket_fd)) {
        Usage(argv[0]);
    }

    //Check amount of arguments provided
    if (argc == 4 && strlen(argv[3]) == 0) {
        //If no file was provided, user inputs
        userInput(socket_fd);
    } else if (argc == 4) {
        //If file was provided, file inputs
        fileInput(socket_fd, argv[3]);
    } else {
        //If something is not right, just let user input
        userInput(socket_fd);
    }
}

void Usage(char *progname) {
  printf("usage: %s  hostname port \n", progname);
  exit(EXIT_FAILURE);
}

int LookupName(char *name, unsigned short port, struct sockaddr_storage *ret_addr, size_t *ret_addrlen) {
  struct addrinfo hints, *results;
  int retval;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  // Do the lookup by invoking getaddrinfo().
  if ((retval = getaddrinfo(name, NULL, &hints, &results)) != 0) {
    printf( "getaddrinfo failed: %s", gai_strerror(retval));
    return 0;
  }

  // Set the port in the first result.
  if (results->ai_family == AF_INET) {
    struct sockaddr_in *v4addr = (struct sockaddr_in *) (results->ai_addr);
    v4addr->sin_port = htons(port);
  } else if (results->ai_family == AF_INET6) {
    struct sockaddr_in6 *v6addr = (struct sockaddr_in6 *)(results->ai_addr);
    v6addr->sin6_port = htons(port);
  } else {
    printf("getaddrinfo failed to provide an IPv4 or IPv6 address \n");
    freeaddrinfo(results);
    return 0;
  }

  // Return the first result.
  assert(results != NULL);
  memcpy(ret_addr, results->ai_addr, results->ai_addrlen);
  *ret_addrlen = results->ai_addrlen;

  // Clean up.
  freeaddrinfo(results);
  return 1;
}

int Connect(const struct sockaddr_storage *addr, const size_t addrlen, int *ret_fd) {
  // Create the socket.
  int socket_fd = socket(addr->ss_family, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    printf("socket() failed: %s", strerror(errno));
    return 0;
  }

  // Connect the socket to the remote host.
  int res = connect(socket_fd, (const struct sockaddr *)(addr), addrlen);
  if (res == -1) {
    printf("connect() failed: %s", strerror(errno));
    return 0;
  }

  *ret_fd = socket_fd;
  return 1;
}