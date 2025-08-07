#ifndef MSGTYPES
#define MSGTYPES

#define KEY    (1174)
#define MAXLEN (512)

#include <stdio.h>

#define SEND_MSG(buffer) \
do { \
    int i = 0; \
    while ((i < (MAXLEN - 1)) && ((buffer[i] = getchar()) != '\n')) { \
        i++; \
    } \
    buffer[i] = '\0'; \
 } while(0); \

struct msg_t {
   long mtype;
   int snd_pid;
   char body[MAXLEN];
};

#endif /* MSGTYPES */
