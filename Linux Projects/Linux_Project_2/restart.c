/*
 * Nathan Dow nmd210001
 * Leonid Mateush lmm220016
 * Write your partner's name and netid
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

#include "common.h"
#include "common_threads.h"

#define BSIZE 4096

int numThreadsMade = 0;
uint32_t blocksPerThread;
uint32_t numThreads;
uint32_t numBlocks;

uint8_t *fileData;

void Usage(char*);
uint32_t jenkins_one_at_a_time_hash(const uint8_t*, uint64_t);

void *tree(void *arg) {
    //Start block can be computed using the thread number
    numThreadsMade++;
    printf("numThreadsMade after call = %d\n", numThreadsMade);
    int treeNum = *(int*)arg;
    // !!! Not sure if this is correct in calculating the start index
    uint32_t startIndex = treeNum * (blocksPerThread * BSIZE); //Use the treeNum to calculate the starting byte
    
    int *hash = NULL;
    uint32_t currentHash = 0;

    if (numThreads == numThreadsMade) { // Base Case and should return the final hash
        currentHash = jenkins_one_at_a_time_hash(&fileData[startIndex], blocksPerThread * BSIZE);
        printf("tnum %d hash computed %u\n", treeNum, currentHash);

        hash = malloc(sizeof(int));
        if (hash == NULL) {
            // Handle memory allocation failure
            perror("Memory allocation failed");
            pthread_exit(NULL);
        }
        *hash = currentHash;
        printf("tnum %d hash sent to parent %u\n", treeNum, currentHash);
        pthread_exit(hash); // Return the total hash value
    } else if (numThreadsMade < numThreads) { // Recursive steps
        if ((numThreads - numThreadsMade) >= 2) {
            pthread_t left, right;

            int leftThreadNum = treeNum * 2 + 1;
            int rightThreadNum = treeNum * 2 + 2;
            printf("numThreadsMade before call = %d\n", numThreadsMade);
            Pthread_create(&left, NULL, tree, (void *)&leftThreadNum);
            printf("numThreadsMade before call = %d\n", numThreadsMade);
            Pthread_create(&right, NULL, tree, (void *)&rightThreadNum);
            
            // Calculate the current thread's hash value While the left and right are calculating theirs
            // Jenkins takes in the start address of the first block then the number of bytes (This is the calc blocksPerThread * BSIZE)
            // !!! Is this how to use jenkins
            currentHash = jenkins_one_at_a_time_hash(&fileData[startIndex], blocksPerThread * BSIZE);
            printf("tnum %d hash computed %u\n", treeNum, currentHash);
            
            // Wait for the threads to finish and store their hash values in leftHash and rightHash
            int *leftHash, *rightHash;
            Pthread_join(left, (void **)&leftHash);
            Pthread_join(right, (void **)&rightHash);

            // Use sprintf() to convert to string and use strcat() to concat
            // finalHashString(the concatinated string of the current + left + right)

            char *curHashStr = malloc(30);
            char *leftHashStr = malloc(30);
            char *rightHashStr = malloc(30);
            if (curHashStr == NULL || leftHashStr == NULL || rightHashStr == NULL) {
                perror("Memory allocation failed for hash strings");
                free(curHashStr);
                free(leftHashStr);
                free(rightHashStr);
                free(hash);
                pthread_exit(NULL);
            }
            sprintf(curHashStr, "%u", currentHash);
            sprintf(leftHashStr, "%u", *leftHash);
            sprintf(rightHashStr, "%u", *rightHash);
            printf("tnum %d hash from left child %s\n", treeNum, leftHashStr);
            printf("tnum %d hash from right child %s\n", treeNum, rightHashStr);

            strcat(curHashStr, leftHashStr);
            strcat(curHashStr, rightHashStr);
            printf("tnum %d concat string %s\n", treeNum, curHashStr);


            // Find the length of the string and convert to uint64_t when passing to jenkins
            // !!! Is this the right way to use jenkins
            size_t length = strlen(curHashStr);
            uint32_t finalHash = jenkins_one_at_a_time_hash((const uint8_t *)curHashStr, length);

            //Free the string buffers after use
            free(curHashStr);
            free(leftHashStr);
            free(rightHashStr);
            
            printf("tnum %d hash sent to parent %u\n", treeNum, finalHash);
            // Allocate memory for hash and store the currentHash value
            hash = malloc(sizeof(int));
            if (hash == NULL) {
                // Handle memory allocation failure
                perror("Memory allocation failed");
                pthread_exit(NULL);
            }
            *hash = finalHash;
            pthread_exit(hash); // Return the value of hash
        } else { //Only one more thread is needed
            pthread_t left;
            int leftThreadNum = treeNum * 2 + 1;
            printf("numThreadsMade before call = %d\n", numThreadsMade);
            Pthread_create(&left, NULL, tree, (void *)&leftThreadNum);

            // Calculate the current thread's hash value
            currentHash = jenkins_one_at_a_time_hash(&fileData[startIndex], blocksPerThread * BSIZE);
            printf("tnum %d hashcomputed %u\n", treeNum, currentHash);

            // Wait for the left thread to finish and obtain its hash value
            int *leftHash;
            Pthread_join(left, (void **)&leftHash);

            // concatinate the hash value of the left thread with the current thread's hash
            char *curHashStr = malloc(30);
            char *leftHashStr = malloc(30);
            if (curHashStr == NULL || leftHashStr == NULL) {
                perror("Memory allocation failed for hash strings");
                free(curHashStr);
                free(leftHashStr);
                free(hash);
                pthread_exit(NULL);
            }

            sprintf(curHashStr, "%u", currentHash);
            sprintf(leftHashStr, "%u", *leftHash);
            printf("tnum %d hash from left child %s\n", treeNum, leftHashStr);

            strcat(curHashStr, leftHashStr);
            printf("tnum %d concat string %s\n", treeNum, curHashStr);

            // Find the length of the string and convert to uint64_t when passing to jenkins
            size_t length = strlen(curHashStr);
            uint32_t finalHash = jenkins_one_at_a_time_hash((const uint8_t *)curHashStr, length);

            
            //Free the string buffers after use
            free(curHashStr);
            free(leftHashStr);

            printf("tnum %d hash sent to parent %u\n", treeNum, finalHash);

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
    // !!! not sure if this is right logic
    numBlocks = (fileSize + (BSIZE - 1)) / BSIZE; // (BSIZE - 1) theoretically to account for leftover bytes of info in the file
    numThreads = atoi(argv[2]);
    blocksPerThread = numBlocks / numThreads;

    printf("num Threads = %u \n", numThreads);
    printf("Blocks per Thread = %u \n", blocksPerThread);
    
    fileData = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    //check for error in mmap command
    if(fileData == MAP_FAILED){
        perror("Failed to map");
        exit(EXIT_FAILURE);
    }

    // calculate hash value of the input file
    pthread_t root;
    int treeNum = 0;

    //Create tree
    double start = GetTime();
    printf("numThreadsMade before call = %d\n", numThreadsMade);
    Pthread_create(&root, NULL, tree, (void *)&treeNum);

    uint32_t *hash;
    Pthread_join(root, (void **)&hash);

    double end = GetTime();
    printf("hash value = %d \n", *hash);
    printf("time taken = %f \n", (end - start));
    close(fd);
    return EXIT_SUCCESS;
}

void Usage(char* s) {
    fprintf(stderr, "Usage: %s filename numThreads \n", s);
    exit(EXIT_FAILURE);
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