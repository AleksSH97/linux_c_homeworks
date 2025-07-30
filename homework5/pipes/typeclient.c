#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "./fifofile"

int main ()
{
  FILE * f;
  char str[256];
  f = fopen(FIFO_NAME, "r");

  while(fgets(str, sizeof(str), f) != NULL) {
    if (strcmp(str, "q\n") == 0) {
        break;
    }
    printf("%sOK\n\r", str);
  }

  fclose(f);
  unlink(FIFO_NAME);
  return 0;
}
