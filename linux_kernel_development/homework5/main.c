#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define RADIO0_PATH ("/dev/radio0")
#define RADIO1_PATH ("/dev/radio1")
#define RADIO2_PATH ("/dev/radio2")
#define BUFFER_SIZE (256)

#define GRN_CLR  "\x1B[32m"
#define DFLT_CLR "\033[0m"

static int read_from_device(char *path, char *buffer, uint16_t buf_len) {
    if(path == NULL) {
        printf("Path to device is NULL\n");
        return -EINVAL;
    }

    if(buffer == NULL) {
        printf("Buffer is NULL\n");
        return -EINVAL;
    }

    if(buf_len == 0) {
        printf("Buffer length can not be 0\n");
        return -EINVAL;
    }

    printf("Reading information from %s device\n", path);

    int fd = open(path, O_RDONLY);
    if(fd < 0) {
        printf("Failed to open %s device: %d\n", path, fd);
        return fd;
    }

    int err = read(fd, buffer, buf_len);
    if(err < 0) {
        printf("Failed to read from %s device: %d\n", path, err);
        return err;
    }

    printf("READ: " "%s[%s]%s\n", GRN_CLR, buffer, DFLT_CLR);

    err = close(fd);
    if(err < 0) {
        printf("Failed to close %s device: %d\n", path, err);
        return err;
    }

    return 0;
}

static int write_to_device(char *path, const char *buffer, uint16_t buf_len) {
    if(path == NULL) {
        printf("Path to device is NULL\n");
        return -EINVAL;
    }

    if(buffer == NULL) {
        printf("Buffer is NULL\n");
        return -EINVAL;
    }

    if(buf_len == 0) {
        printf("Buffer length can not be 0\n");
        return -EINVAL;
    }

    printf("Writing information to %s device\n", path);

    int fd = open(path, O_WRONLY);
    if(fd < 0) {
        printf("Failed to open %s device: %d\n", path, fd);
        return fd;
    }

    int err = write(fd, buffer, buf_len);
    if(err < 0) {
        printf("Failed to write to %s device: %d\n", path, err);
        return err;
    }

    printf("WRITTEN: " "%s[%s]%s\n", GRN_CLR, buffer, DFLT_CLR);

    err = close(fd);
    if(err < 0) {
        printf("Failed to close %s device: %d\n", path, err);
        return err;
    }

    return 0;
}

int main(int argc, char ** argv) {
    char buffer[BUFFER_SIZE];

    /* Reading from RADIO0 */
    int err = read_from_device(RADIO0_PATH, buffer, BUFFER_SIZE);
    if(err != 0) {
        printf("Failed to read RADIO0: %d\n", err);
        exit(1);
    }

    /* Reading from RADIO1 */
    err = read_from_device(RADIO1_PATH, buffer, BUFFER_SIZE);
    if(err != 0) {
        printf("Failed to read RADIO1: %d\n", err);
        exit(1);
    }

    /* Writing to RADIO2 */
    char msg[] = "Blin blinskiy, Linux!";
    err = write_to_device(RADIO2_PATH, msg, sizeof(msg));
    if(err != 0) {
        printf("Failed to write to RADIO2: %d\n", err);
        exit(1);
    }

    return 0;
}
