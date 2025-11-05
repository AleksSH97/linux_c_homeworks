#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char * argv[])
{
    int fd;
    char str[64];
    memset(str, ' ', sizeof(str));
    struct flock fi;
    int off;
    sprintf(str, "Запись сделана процессом %i", getpid());

    fd = open("testlock.txt", O_RDWR|O_CREAT);
    if(fd == -1) {
    	perror("Ошибка открытия файла");
     	return 1;
    }

    fi.l_type = F_WRLCK;
    fi.l_whence = SEEK_SET;
    fi.l_start = 0;
    fi.l_len = 64;
    off = 0;

    while (fcntl(fd, F_SETLKW, &fi) == -1)
    {
	     if (fcntl(fd, F_GETLK, &fi) == -1) {
	         perror("Ошибка проверки блокировки");
	         close(fd);
	         return 1;
	     }

		if (fi.l_type == F_UNLCK) {
			printf("Блокировка уже снята\n");
			continue;
		}

        printf("байты %i - %i заблокированы процессом %i\n", off, off+64, fi.l_pid);
        fi.l_start += fi.l_len;
        if (fi.l_start > 1024 * 1024) { /* 1 Mb boundary */
            close(fd);
            return 1;
        }
    }

    lseek(fd, fi.l_start, SEEK_SET);
    write(fd, str, strlen(str));

    getchar();

    fi.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fi) == -1)
        perror("Ошибка разблокирования\n");
    else
         printf("Регион разблокирован");

    close(fd);

    return 0;
}
