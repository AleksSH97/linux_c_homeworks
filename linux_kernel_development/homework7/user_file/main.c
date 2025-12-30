#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>

#define RADIO_MMAP_PATH ("/dev/mmap-radio")
#define BUFFER_SIZE (256)

#define GRN_CLR  "\x1B[32m"
#define DFLT_CLR "\033[0m"

static int read_from_mmap(char *path, char *buffer, uint16_t buf_len) {
    if(path == NULL) {
        perror("Path to device is NULL\n");
        return -EINVAL;
    }

    if(buffer == NULL) {
        perror("Buffer is NULL\n");
        return -EINVAL;
    }

    if(buf_len == 0) {
        perror("Buffer length can not be 0\n");
        return -EINVAL;
    }

    int fd = open(path, O_RDWR);
    if(fd < 0) {
        printf("Failed to open %s device: %d\n", path, fd);
        return fd;
    }

    void *mmaped_mem = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mmaped_mem == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return -EACCES;
    }

    buffer = (char *)mmaped_mem;
    printf("Data from %s: [%s]\n", path, buffer);

    int ret = munmap(mmaped_mem, 4096);
    if (ret == -1) {
        perror("munmap failed");
    }
    
    close(fd);

    return 0;
}

int main(int argc, char ** argv) {

    char buffer[BUFFER_SIZE];

    int err = read_from_mmap(RADIO_MMAP_PATH, buffer, BUFFER_SIZE);
    if(err != 0) {
        printf("Failed to read %s: %d\n", RADIO_MMAP_PATH, err);
        exit(1);
    }

    return 0;
}
