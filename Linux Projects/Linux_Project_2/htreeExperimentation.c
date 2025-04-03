#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <math.h>
#include <sys/mman.h>

#define BSIZE 4096

void Usage(char*);
uint32_t jenkins_one_at_a_time_hash(const uint8_t*, uint64_t);
void *hash_thread(void *);
int gettid();
void create_threads(pthread_t*, ThreadArg*, int, int, int);
void compute_hash(uint8_t*, uint32_t, uint32_t*);

typedef struct {
    char *filename;
    uint32_t startBlock;
    uint32_t numBlocks;
    uint32_t hashValue;
} ThreadArg;

void *tree(void *arg) {
    ThreadArg* threadArg = (ThreadArg*) arg;
    uint32_t* hashValue = &(threadArg->hashValue);

    if (threadArg->numBlocks == 1) { 
        // Read and hash blocks here using mmap
        int fd = open(threadArg->filename, O_RDONLY);
        if (fd == -1) {
            perror("open failed");
            exit(EXIT_FAILURE);
        }

        // Calculate offset and length for mmap
        off_t offset = threadArg->startBlock * BSIZE;
        size_t length = threadArg->numBlocks * BSIZE;

        // Map the file into memory
        uint8_t *data = mmap(NULL, length, PROT_READ, MAP_SHARED, fd, offset);
        if (data == MAP_FAILED) {
            perror("mmap failed");
            exit(EXIT_FAILURE);
        }

        // Compute the hash value
        *hashValue = jenkins_one_at_a_time_hash(data, length);

        // Unmap the file
        if (munmap(data, length) == -1) {
            perror("munmap failed");
            exit(EXIT_FAILURE);
        }

        close(fd);
    } else {
        /*
        Each thread is assigned a number when it is created. The number of root thread is 0. For a thread
        with number i, the number of its left child is 2 * i + 1, and the number of its right child is 2 * i +
        2. For a thread with number i, n/m consecutive blocks starting from block number i * n/m are
        assigned. For example, let n = 100 and m = 4. Then thread 0 will be assigned blocks 0 through
        24, thread 1 will be assigned blocks 25 through 49, thread 2 will be assigned blocks 50 through
        74, and thread 3 will be assigned blocks 75 through 99
        */


        // Create child threads recursively
        pthread_t left_child, right_child;
        int left_blocks = threadArg->numBlocks / 2;
        int right_blocks = threadArg->numBlocks - left_blocks;

        ThreadArg left_arg = {threadArg->filename, threadArg->startBlock, left_blocks, 0};
        ThreadArg right_arg = {threadArg->filename, threadArg->startBlock + left_blocks, right_blocks, 0};

        pthread_create(&left_child, NULL, tree, &left_arg);
        pthread_create(&right_child, NULL, tree, &right_arg);
        pthread_join(left_child, NULL);
        pthread_join(right_child, NULL);

        // Combine hash values
        *hashValue = jenkins_one_at_a_time_hash((uint8_t*)hashValue, sizeof(*hashValue));
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    int32_t fd;
    uint32_t numBlocks;
    uint32_t numThreads;

    // input checking
    if (argc != 3)
        Usage(argv[0]);

    // open input file
    fd = open(argv[1], O_RDONLY);
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

    // calculate number of blocks
    numBlocks = (uint32_t)(fileSize / BSIZE);

    numThreads = atoi(argv[2]);

    printf("Number of blocks = %u \n", numBlocks);

    pthread_t threads[numThreads];
    ThreadArg root_arg = {argv[1], 0, numBlocks, 0};

    // Create root thread
    pthread_create(&threads[0], NULL, tree, &root_arg);
    pthread_join(threads[0], NULL);

    printf("Hash value: %u\n", root_arg.hashValue);

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
