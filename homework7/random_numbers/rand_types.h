#ifndef RAND_TYPES_H
#define RAND_TYPES_H

#define FTOK_FILE "./rand_server"

#define MAXLEN (64)

struct memory_block {
    char string[MAXLEN];
    int num;
};

#endif
