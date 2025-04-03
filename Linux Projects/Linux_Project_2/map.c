#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>     // for EINTR
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>


// Print out the usage of the program and exit.
void Usage(char*);

// Block size in bytes
#define BSIZE 4096

int
main(int argc, char** argv)
{
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
    struct stat fs; //Creates a stat struct  fs (File Storage) where the file data will be stored

    
     // fill fs with file info
    if(fstat(fd, &fs) == -1){
        perror("fstat failed");
        exit(EXIT_FAILURE);
     }
    off_t fileSize = fs.st_size; // creates a varible of type off_t to be used in mmap and other methods as size_t length argument

    // call mmap to map the file to memory. See man page for mmap
    // suggested usage: mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0)
    // assign the return value of mmap to  pointer variable arr 
    // arr[i] is the ith byte of the file
    // Assume the file to consists of sequence of unsigned 8 bit integer (uint8_t)

    //initialize the fileData pointer to store the mapped file info using uint8_t as specified
    uint8_t *fileData;                
    // map file to fileData
    fileData = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
          
    //check for error in mmap command
    if(fileData == MAP_FAILED){
        perror("Failed to map");
        exit(EXIT_FAILURE);
    }

    // The file is logically divided into several blocks. 
    // The size of each block is BSIZE (defined above)
    // find the smallest element in the block # specified by argv[2]
    // print the result

    int maxBSize = ((int)fileSize) / BSIZE;
    int block = atoi(argv[2]); // argv[2] needs to be an int
   
    if(block >= maxBSize){
        printf("The specified block size is too big");
        exit(EXIT_FAILURE);
    }

    // find the smallest element
    int startI = block * BSIZE;
    uint8_t min = fileData[startI];
    int i;
    for(i = startI; i < (startI + BSIZE) && i < fileSize; i++) {
        if(fileData[i] < min){
            min = fileData[i];
        }
     }

    printf("minimum value in block %d is %u\n", block, (unsigned int)min);

    munmap(fileData, fileSize);  //clean up file

    close(fd);
    return EXIT_SUCCESS;
}


void
Usage(char* s)
{
    fprintf(stderr, "Usage: %s filename block_num \n", s);
    exit(EXIT_FAILURE);
}