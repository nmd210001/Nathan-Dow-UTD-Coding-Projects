/*
 * Nathan Dow nmd210001
 * Leonid Mateush lmm220016
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>     
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/mman.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#include "common_threads.h"
#include "common.h"
#include "msg.h"

//defines the name of the database
#define data "projectDataBase.txt"

void *handler(void *arg);
//Main
void Usage(char *progname);
int Listen(char *portnum, int *sock_family);
int putServer(struct msg *message);
int getServer(struct msg *message);
void HandleClient(int c_fd, struct sockaddr *addr, size_t addrlen, int sock_family);
void PrintOut(int fd, struct sockaddr *addr, size_t addrlen);
void PrintReverseDNS(struct sockaddr *addr, size_t addrlen);
void PrintServerSide(int client_fd, int sock_family);


struct clientInfo {
  int clientFd;
  struct sockaddr_storage clAddr;
  socklen_t addrLen;
  int sockFamily;
};

void *handler(void *arg) { // Handles eaach thread
  struct clientInfo *info = (struct clientInfo *)arg;
  struct sockaddr_storage caddr = info->clAddr;
  socklen_t caddr_len = info->addrLen;
  int client_fd = info->clientFd;
  int sock_family = info->sockFamily;

  // Runs HandleClient for each thread
  HandleClient(client_fd, (struct sockaddr *)(&caddr), caddr_len, sock_family);

  // After the client was handled, frees the memory and exits the thread
  free(info);
  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  // Expect the port number as a command line argument.
  if (argc != 2) {
    Usage(argv[0]);
  }

  int sock_family;
  int listen_fd = Listen(argv[1], &sock_family);
  if (listen_fd <= 0) {
    // We failed to bind/listen to a socket.  Quit with failure.
    printf("Couldn't bind to any addresses.\n");
    return EXIT_FAILURE;
  }

  // Loop forever, accepting a connection from a client and doing
  // an echo trick to it.
  while (1) {
    struct sockaddr_storage caddr;
    socklen_t caddr_len =sizeof(caddr);

    int client_fd = accept(listen_fd, (struct sockaddr *)(&caddr), &caddr_len);
    if (client_fd < 0) {
      if ((errno == EINTR) || (errno == EAGAIN) || (errno == EWOULDBLOCK))
        continue;
        printf("Failure on accept:%s \n ", strerror(errno));
        break;
    }

    struct clientInfo *info = malloc(sizeof(struct clientInfo));
    if (info == NULL) {
      printf("Failed to allocate memory for handler arguments.\n");
      close(client_fd);
      continue;
    }
    info->clientFd = client_fd;
    info->clAddr = caddr;
    info->addrLen = caddr_len;
    info->sockFamily = sock_family;
    
    //after getting all the info on the socket and client, creats a new thread
    pthread_t thread;
    Pthread_create(&thread, NULL, handler, (void *)info);
    //no join because we want the program to run multiple threads, no need to wait for thread to exit
  }

  // Close socket
  close(listen_fd);
  return EXIT_SUCCESS;
}

void Usage(char *progname) {
  printf("usage: %s port \n", progname);
  exit(EXIT_FAILURE);
}

int Listen(char *portnum, int *sock_family) {
  // Populate the "hints" addrinfo structure for getaddrinfo().
  // ("man addrinfo")
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;       // IPv6 (also handles IPv4 clients)
  hints.ai_socktype = SOCK_STREAM;  // stream
  hints.ai_flags = AI_PASSIVE;      // use wildcard "in6addr_any" address
  hints.ai_flags |= AI_V4MAPPED;    // use v4-mapped v6 if no v6 found
  hints.ai_protocol = IPPROTO_TCP;  // tcp protocol
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  // getaddrinfo() returns a list of
  // address structures via the output parameter "result".
  struct addrinfo *result;
  int res = getaddrinfo(NULL, portnum, &hints, &result);

  // Did addrinfo() fail?
  if (res != 0) {
	printf( "getaddrinfo failed: %s", gai_strerror(res));
    return -1;
  }

  // Loop through the returned address structures until we are able
  // to create a socket and bind to one.  The address structures are
  // linked in a list through the "ai_next" field of result.
  int listen_fd = -1;
  struct addrinfo *rp;
  // Attempts to create a socket with the addressinfo we created above
  // loops through each of the address strutrues until it is successfuly created
  // If fail error handle (more below)
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    listen_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (listen_fd == -1) {
      // Creating this socket failed.  So, loop to the next returned
      // result and try again.
      printf("socket() failed:%s \n ", strerror(errno));
      listen_fd = -1;
      continue;
    }

    // Configure the socket; we're setting a socket "option."  In
    // particular, we set "SO_REUSEADDR", which tells the TCP stack
    // so make the port we bind to available again as soon as we
    // exit, rather than waiting for a few tens of seconds to recycle it.
    int optval = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Try binding the socket to the address and port number returned
    // by getaddrinfo().
    if (bind(listen_fd, rp->ai_addr, rp->ai_addrlen) == 0) {
      // Bind worked!  Print out the information about what
      // we bound to.
      //PrintOut(listen_fd, rp->ai_addr, rp->ai_addrlen);

      // Return to the caller the address family.
      *sock_family = rp->ai_family;
      break;
    }

    // The bind failed.  Close the socket, then loop back around and
    // try the next address/port returned by getaddrinfo().
    close(listen_fd);
    listen_fd = -1;
  }

  // Free the structure returned by getaddrinfo().
  freeaddrinfo(result);

  // If we failed to bind, return failure.
  if (listen_fd == -1)
    return listen_fd;

  // Success. Tell the OS that we want this to be a listening socket.
  if (listen(listen_fd, SOMAXCONN) != 0) {
    printf("Failed to mark socket as listening:%s \n ", strerror(errno));
    close(listen_fd);
    return -1;
  }

  // Return to the client the listening file descriptor.
  return listen_fd;
}

int putServer(struct msg *message) { //Used to put the information given by a client to a database file
  //Opens a file with flags listed bellow, file name is defined at the begining of the code
  //O_WRONLY - open file for writing only
  //O_APPEND - puts new data to the end of the file
  //O_CREAT - create a new file if not existed before, 0666 is permission for read and write for all
  int fd = open(data, O_WRONLY | O_APPEND | O_CREAT, 0666);
  if (fd == -1) { //Checks for errors
    perror("open failed");
    message->type = FAIL;
    return (EXIT_FAILURE);
  }

  //Allocates needed memory to store the message->rd.id
  //id is used to write to file
  char *id = malloc(20 * sizeof(message->rd.id));
  if (id == NULL) { //Checks for errors
    perror("Memory allocation failed for hash strings");
    message->type = FAIL;
    free(id);
    return (EXIT_FAILURE);
  }
 
  // convert ID from int to string
  sprintf(id, "%u", message->rd.id); 

  //Stores ID + " " + name + '\n' inside the file that was opened, and checks for errors oppening it
  if (write(fd, id, strlen(id)) == -1 || 
      write(fd, " ", 1) == -1 || 
      write(fd, &message->rd.name, strlen(message->rd.name)) == -1 ||
      write(fd, "\n", 1) == -1) {
    message->type = FAIL;
    perror("Error writing message to file");
    return EXIT_FAILURE;
  }

  //If we get to the end without getting eror, sucess is passed, fd is closed, and putServer returns sucess as well
  close(fd);
  message->type = SUCCESS;
  return(EXIT_SUCCESS);
}

int getServer(struct msg *message) { //Used to get the information from the file using the ID provided by the client
  //Used to see if the ID was found inside the file
  int found = 0;
  //Char to store each ID that is being read from the file
  char idLine[sizeof(message->rd.pad)];
  //Char to store the name that is being read from the file
  char nameLine[MAX_NAME_LENGTH];

  //Allocates needed memory to store the message->rd.id, stores the ID that we need to find in the file
  char *search_id = malloc(20 * sizeof(message->rd.id));
  if (search_id == NULL) {
    perror("Memory allocation failed for hash strings");
    free(search_id);
    return(EXIT_FAILURE);
  }

  //Convert message->rd.id from int to string
  sprintf(search_id, "%u", message->rd.id); 

  //Open a file that is defined at the begining of the code
  FILE *fd = fopen(data, "r");
  if (fd == NULL){ //Checks for erorrs
    perror("Error opening file");
    message->type = FAIL;
    free(search_id);
    return(EXIT_FAILURE);
  }

  //Char to store each line
  char line[256];

  //Loops trought the file until it can't, reads line by line
  while (fgets(line, sizeof(line), fd)) {
    if (sscanf(line, "%s %s", idLine, nameLine) == 2){
      if(strcmp(idLine, search_id) == 0){ //If the ID are the same, copies  the nameLine to the message->rd.name and sets found to 1 (true)
        char* startOfName = strchr(line, ' '); //Needed to get name with spaces in them
        if (startOfName){
          startOfName++;
          strncpy(message->rd.name, startOfName, MAX_NAME_LENGTH - 1);
          message->rd.name[MAX_NAME_LENGTH - 1] = '\0';
        }
        
        found = 1;
        break;
      }
    }   
  }

  if(!found) { //Checks if the file was foudn in the loop or not, if not returns failure
    message->type = FAIL;
    fclose(fd);
    free(search_id);
    return(EXIT_FAILURE);
  }

  //If we get to this point wihtout getting errors, returns sucess to client
  message->type = SUCCESS;
  fclose(fd);
  free(search_id);
  return(EXIT_SUCCESS);
}

void HandleClient(int c_fd, struct sockaddr *addr, size_t addrlen, int sock_family) {
  // Print out information about the client.
  printf("\nNew client connection \n" );
  PrintOut(c_fd, addr, addrlen);
  PrintServerSide(c_fd, sock_family);

  //Construct the PUT message
  struct msg message;

  // Loop, reading data and echo'ing it back, until the client
  // closes the connection.
  while (1) {
    //Reads from the client
    ssize_t res = read(c_fd, &message, sizeof(message));
    if (res == 0) { //Check for errors
      printf("[The client disconnected. ]\n");
      break;
    }
    if (res == -1) {
      if ((errno == EAGAIN) || (errno == EINTR)){
        continue;
      }
      printf(" Error on client socket:%s \n ", strerror(errno));
      break;
    }

    //PUT
    //Get's message struct from the client and puts in into database
    if (message.type == PUT) {
      if (putServer(&message) != EXIT_SUCCESS) { //Runs putServer and gets return fail or pass
        message.type = FAIL;
      }

      //Writes the result back to the client
      int wres = write(c_fd, &message, sizeof(message));
      if (wres == 0) { //Checks for errors
        printf("socket closed prematurely\n");
        close(c_fd);
        break;
      }
      if (wres == -1) {
        printf("socket write failure\n");
        close(c_fd);
        break;
      }
    }

    //GET
    //Get's message struct from the client and puts in into database
    else if (message.type == GET) {
      if (getServer(&message) != EXIT_SUCCESS) { //Runs getServer and gets return fail or pass
        message.type = FAIL;
      }

      //Writes the result back to the client
      int wres = write(c_fd, &message, sizeof(message));
      if (wres == 0) { //Checks for errors
        printf("socket closed prematurely\n");
        close(c_fd);
        break;
      }
      if (wres == -1 ){
        printf("socket write failure\n");
        close(c_fd);
        break;
      }
    } else { //If it's not PUT or GET, then you shouldn't be here
      fprintf(stderr, "Error Reading Input\n");
    }
  }

  //close at the end
  close(c_fd);
}

void PrintOut(int fd, struct sockaddr *addr, size_t addrlen) {
  printf("Socket [%d] is bound to: \n", fd);
  if (addr->sa_family == AF_INET) {
    // Print out the IPV4 address and port

    char astring[INET_ADDRSTRLEN];
    struct sockaddr_in *in4 = (struct sockaddr_in *)(addr);
    inet_ntop(AF_INET, &(in4->sin_addr), astring, INET_ADDRSTRLEN);
    printf(" IPv4 address %s", astring);
    printf(" and port %d\n", ntohs(in4->sin_port));

  } else if (addr->sa_family == AF_INET6) {
    // Print out the IPV6 address and port

    char astring[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *in6 = (struct sockaddr_in6 *)(addr);
    inet_ntop(AF_INET6, &(in6->sin6_addr), astring, INET6_ADDRSTRLEN);
    printf("IPv6 address %s", astring);
    printf(" and port %d\n", ntohs(in6->sin6_port));

  } else {
    printf(" ???? address and port ???? \n");
  }
}

void PrintReverseDNS(struct sockaddr *addr, size_t addrlen) {
  char hostname[1024];  // ought to be big enough.
  if (getnameinfo(addr, addrlen, hostname, 1024, NULL, 0, 0) != 0) {
    sprintf(hostname, "[reverse DNS failed]");
  }
  printf("DNS name: %s \n", hostname);
}

void PrintServerSide(int client_fd, int sock_family) {
  char hname[1024];
  hname[0] = '\0';

  printf("Server side interface is ");
  if (sock_family == AF_INET) {
    // The server is using an IPv4 address.
    struct sockaddr_in srvr;
    socklen_t srvrlen = sizeof(srvr);
    char addrbuf[INET_ADDRSTRLEN];
    getsockname(client_fd, (struct sockaddr *) &srvr, &srvrlen);
    inet_ntop(AF_INET, &srvr.sin_addr, addrbuf, INET_ADDRSTRLEN);
    printf("%s", addrbuf);
    // Get the server's dns name, or return it's IP address as
    // a substitute if the dns lookup fails.
    getnameinfo((const struct sockaddr *) &srvr, srvrlen, hname, 1024, NULL, 0, 0);
    printf(" [%s]\n", hname);
  } else {
    // The server is using an IPv6 address.
    struct sockaddr_in6 srvr;
    socklen_t srvrlen = sizeof(srvr);
    char addrbuf[INET6_ADDRSTRLEN];
    getsockname(client_fd, (struct sockaddr *) &srvr, &srvrlen);
    inet_ntop(AF_INET6, &srvr.sin6_addr, addrbuf, INET6_ADDRSTRLEN);
    printf("%s", addrbuf);
    // Get the server's dns name, or return it's IP address as
    // a substitute if the dns lookup fails.
    getnameinfo((const struct sockaddr *) &srvr, srvrlen, hname, 1024, NULL, 0, 0);
    printf(" [%s]\n", hname);
  }
}