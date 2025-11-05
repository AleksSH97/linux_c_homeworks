#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char * argv[])
{
  int pipedes[2];
  pipe(pipedes);
  pid_t pid = fork();

  if(pid > 0) {
    close(pipedes[0]);
    char  *str = "String from parent\n";
    write(pipedes[1], (void *) str, strlen(str) + 1);
    close(pipedes[1]);
  } else {
    pid_t pid2 = fork();

    if(pid2 > 0) {
        close(pipedes[1]);
        char buf[1024];
        int len;
        len = read(pipedes[0], buf, sizeof(buf));
        if (len > 0) {
            write(1, "Child read: ", 12), write(1, buf, len);
        }
        close(pipedes[0]);
    } else {
        close(pipedes[1]);
        char buf[1024];
        int len;
        len = read(pipedes[0], buf, sizeof(buf));
        if(len > 0) {
            write(1, "Grandchild read: ", 17), write(1, buf, len);
        }
        close(pipedes[0]);
    }
  }
  return 0;
}
