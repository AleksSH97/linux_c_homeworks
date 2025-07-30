#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "./fifofile"

int main(int argc, char * argv[])
{
  FILE * f;
  char str[256];
  mkfifo(FIFO_NAME, 0600);
  f = fopen(FIFO_NAME, "w");
  if (f == NULL)
  {
    printf("Не удалось открыть файл\n");
    return -1;
  }
  while(fgets(str, sizeof(str), stdin) != NULL) {
    if (strcmp(str, "q\n") == 0) {
        break;
    }

    fputs(str, f);
    fflush(f);
  }

  fclose(f);
  unlink(FIFO_NAME);
  return 0;
}
