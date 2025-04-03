//Assignment 9 - Nathan Dow
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "common_threads.h"

void *tree(void *arg);
int gettid();

int
main(int argc, char *argv[])
{
        int ht;

    if (argc != 2) {
                fprintf(stderr, "usage: htree height \n");
                exit(1);
    }

        ht = atoi(argv[1]);

    pthread_t p1;
        Pthread_create(&p1, NULL, tree, &ht);
        Pthread_join(p1, NULL);
    return 0;
}

// It is easier to write a recursive function than an iterative one.
// Remember arg is an address in the stack of the caller.
// I wouldnt modify the value at that location.
void*
tree(void* arg)
{
    int height = *(int*) arg;               

    if(height == 0){    //Base ca se and stops when it reaches the leaf (When the height is 0)                  
        printf("Leaf Thread (id: %d) at height %d\n", gettid(), height);
    } else if(height > 0){      // Recursive step: If it is not a leaf create two children pthreads to represent the left and right branches of the current pthread           
        printf("Int. Thread (id: %d) at height %d\n", gettid(), height);
        height--; // Lower the height to keep track of location going down a lair                          
        pthread_t p1, p2; //intializes two pthread identifers to be passed to Pthread_create
        Pthread_create(&p1, NULL, tree, &height);   //recursivly calls tree and passing the restrict thread of p1 and and height as the restrict arg
        Pthread_create(&p2, NULL, tree, &height);    //recursivly calls tree and passing the restrict thread of p2 and and height as the restrict arg
        // Waits for each thread to finish
        Pthread_join(p1, NULL);                    
        Pthread_join(p2, NULL);
    }
    return NULL;
}

int
gettid()
{
        return (syscall(SYS_gettid));
}