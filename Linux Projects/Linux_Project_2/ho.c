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

#include "common_threads.h"
#include "common.h"

#define BSIZE 4096

uint32_t blocksPerThread; //Amount of blocks per thread
uint32_t numThreads;//Amount of threads needed to be created
uint32_t numBlocks; //Number of blocks
uint8_t *fileData; //File array

void Usage(char*);
uint32_t jenkins_one_at_a_time_hash(const uint8_t*, uint64_t);

void *tree(void *arg) {
    //Start block can be computed using the thread number  
    int treeNum = *(int*)arg;
    uint32_t startIndex = treeNum * (blocksPerThread * BSIZE); //Use the treeNum to calculate the starting byte
    uint32_t *hash = NULL; // hash
    uint32_t currentHash = 0; // current hash

    int leftThreadNum = treeNum * 2 + 1; //number of left child
    int rightThreadNum = treeNum * 2 + 2; //number of right child

    if (leftThreadNum >= numThreads) { // Base Case and should return the final hash because can't create new thread
        currentHash = jenkins_one_at_a_time_hash(&fileData[startIndex], blocksPerThread * BSIZE);
        hash = malloc(100*sizeof(int));

        if (hash == NULL) { // Handle memory allocation failure
            perror("Memory allocation failed");
            pthread_exit(NULL);
        }
        *hash = currentHash;
        pthread_exit(hash); // Return the total hash value
    } else if (rightThreadNum < numThreads) { // If the leftThreadNum is less than numThreads and rightThreadNum is less than numthreads
        pthread_t left, right;
        Pthread_create(&left, NULL, tree, (void *)&leftThreadNum);
        Pthread_create(&right, NULL, tree, (void *)&rightThreadNum);

        // Calculate the current thread's hash value While the left and right are calculating theirs
        // Jenkins takes in the start address of the first block then the number of bytes (This is the calc blocksPerThread * BSIZE)
        currentHash = jenkins_one_at_a_time_hash(&fileData[startIndex], blocksPerThread * BSIZE);

        // Wait for the threads to finish and store their hash values in leftHash and rightHash
        uint32_t *leftHash, *rightHash;
        Pthread_join(left, (void **)&leftHash);
        Pthread_join(right, (void **)&rightHash);

        // Use sprintf() to convert to string and use strcat() to concatß
        // finalHashString(the concatinated string of the current + left + right)
        // allocate memory large enough for 100 characters for each string to store in        
        char *curHashStr = malloc(100 * sizeof(char));
        char *leftHashStr = malloc(100 * sizeof(char));
        char *rightHashStr = malloc(100 * sizeof(char));
        if (curHashStr == NULL || leftHashStr == NULL || rightHashStr == NULL) {
            perror("Memory allocation failed for hash strings");
            free(curHashStr);
            free(leftHashStr);
            free(rightHashStr);
            free(hash);
            pthread_exit(NULL);
        }
        
        // Converts the integers into string
        sprintf(curHashStr, "%u", currentHash);
        sprintf(leftHashStr, "%u", *leftHash);
        sprintf(rightHashStr, "%u", *rightHash);
        strcat(curHashStr, leftHashStr);

        //Concatinates the strings        
        strcat(curHashStr, rightHashStr);

        // Find the length of the string and convert to uint64_t when passing to jenkins
        size_t length = strlen(curHashStr);
 //to hash the e whole sstringhash vathing        
 uint32_t finalHash = jenkins_one_at_a_time_hash((const uint8_t *)curHashStr, length);

        //Free the string buffers after use
        free(curHashStr);
        free(leftHashStr);
        free(rightHashStr);
        
        // Allocate memory for hash and store the currentHash value
        hash = malloc(sizeof(int));
        if (hash == NULL) { // Handle memory allocation failure
            perror("Memory allocation failed");
            pthread_exit(NULL);
        }
        *hash = finalHash;
        pthread_exit(hash); // Return the value of hash
    } else {//if leftTHreadNum is less than numTHread
        pthread_t left;
        int leftThreadNum = treeNum * 2 + 1;
        Pthread_create(&left, NULL, tree, (void *)&leftThreadNum);

        // Calculate the current thread's hash value
        currentHash = jenkins_one_at_a_time_hash(&fileData[startIndex], blocksPerThread * BSIZE);

        // Wait for the left thread to finish and obtain its hash value
        uint32_t *leftHash;
        Pthread_join(left, (void **)&leftHash);

        // concatinate the hash value of the left thread with the current thread's hash
        char *curHashStr = malloc(100 * sizeof(char));

        // allocate memory large enough for 100 characters for each string to store in        
        char *leftHashStr = malloc(100 * sizeof(char));
        if (curHashStr == NULL || leftHashStr == NULL) {
             perror("Memory allocation failed for hash strings");
            free(curHashStr);
            free(leftHashStr);
            free(hash);
            pthread_exit(NULL);
        }

        sprintf(curHashStr, "%u", currentHash);
        // Converts the integers into strings        
        sprintf(leftHashStr, "%u", *leftHash);

         strcat(curHashStr, leftHashStr);
        //Concatinates the strings
        // Find the length of the string and convert to uint64_t when passing to jenkins
        size_t length = strlen(curHashStr);
        uint32_t finalHash = jenkins_one_at_a_time_hash((const uint8_t *)curHashStr, length);
 
        //Free the string buffers after use
        free(curHashStr);
        free(leftHashStr);

        // Allocate memory for hash and store the currentHash value
        hash = malloc(sizeof(int));
        if (hash == NULL) {
            // Handle memory allocation failure
            perror("Memory allocation failed");
            pthread_exit(NULL);
        }
        *hash = finalHash;
        pthread_exit(hash);
    }
    if(hash != NULL)
        free(hash); // Free memory allocated for hash
    return NULL;
}



int main(int argc, char** argv) {
    int32_t fd;

    // input checking 
    if (argc != 3)
        Usage(argv[0]);

    // open input file
    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    // use fstat to get file size
    struct stat fs;
    if (fstat(fd, &fs) == -1) {
        perror("fstat failed");
        exit(EXIT_FAILURE);
    }
    off_t fileSize = fs.st_size;

    // calculate number of blocks and blocksPerThread
    numBlocks = (fileSize + (BSIZE - 1)) / BSIZE; // (BSIZE - 1) theoretically to account for leftover bytes of info in the file
    numThreads = atoi(argv[2]);
    blocksPerThread = numBlocks / numThreads;

    printf("num Threads = %u \n", numThreads); //print number of threads
    printf("Blocks per Thread = %u \n", blocksPerThread); //print amount of blocks per thread
    
    fileData = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);

    if(fileData == MAP_FAILED){ //check for error in mmap command
        perror("Failed to map");
        exit(EXIT_FAILURE);
    }

    // calculate hash value of the input file
    pthread_t root;
    int treeNum = 0;

    //Create tree
    double start = GetTime(); // start the timer before creating threads
    Pthread_create(&root, NULL, tree, (void *)&treeNum);
    uint32_t *finalHashValue;
    Pthread_join(root, (void **)&finalHashValue);
    double end = GetTime(); // ending timre after calculations end
    
    printf("hash value = %u \n", *finalHashValue); // print final hash value
    printf("time taken = %f \n", (end - start));    // print total time taken, needed 
    close(fd);
    return EXIT_SUCCESS;
}

void Usage(char* s) {
    fprintf(stderr, "Usage: %s filename numThreads \n", s);
    exit(EXIT_FAILURE);
}

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, uint64_t length) { //Jenkkins hashing
    uint64_t i = 0;
 //agorithum    
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