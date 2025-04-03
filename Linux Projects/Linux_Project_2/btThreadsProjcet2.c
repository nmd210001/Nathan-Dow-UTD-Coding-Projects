#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BSIZE 4096

// Function to compute the Jenkins one_at_a_time hash
void 
*tree(void *arg) {
  int m = *(int*) arg; // gotta be arv[2] or something, because we also gonna have a file name in there
  int *hash;
  int numberOfThreadsMade = 0;

  if ( m == numberOfThreadsMade) { // Base Case and should return the final hash 
    pthread_exit(hash);	//return the total hash value
  } else if (numberOfThreadsMade < m) { // Recursive steps
   if(m - numberOfThreadsMade) >=2{
        hashLeft = malloc(sizeof(int));
        hashRight = malloc(sizeof(int));
        pthread_t left, right;
        Pthread_create(&left, NULL, tree, &height); //create threads
        Pthread_create(&right, NULL, tree, &height);
        hash = jenkins_one_at_a_time_hash((const uint8_t*)&hash, sizeof(hash)); // Calculate own hash value
        cout+=2;
        int  hash = *hash * 10000 + hashPthread_join(left, NULL); //Concatinate current threads found hash with it's left                 
        int  hash = *hash * 10000 + hashPthread_join(right, NULL); //Concatinate current threads found hash with it's right
        pthread_exit(hash); //return the value of hash
    }
    else{
        childHash = malloc(sizeof(int));
        pthread_t left;
        Pthread_create(&left, NULL, tree, &height);
        cout++;
        //join threads
        int  hash = *hash * 10000 + hashPthread_join(left, NULL); //Concatinate current threads found hash with it's left 
        pthread_exit(hash);
    }
  }
  return NULL
}

int gettid() {
    return (syscall(SYS_gettid));
}

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, uint64_t length) {
    uint64_t i = 0;
    uint32_t hash = 0;

    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}