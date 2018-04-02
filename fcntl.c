#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

void mylock(int);
void myunlock(int);

int main(int argc, char **argv)
{
    int fd;
    int i, n, seqno;
    pid_t pid;
    char line[10];

    pid = getpid();
    fd = open("seqno", O_RDWR);
    for(i = 0; i < 20; i++){
        mylock(fd);
        lseek(fd,0,SEEK_SET);
        n = read(fd,line,10);
        line[n] = '\0';
        n = sscanf(line,"%d\n",&seqno);
        printf("pid %ld seqno %d\n",(long)pid,seqno);
        seqno++;
        snprintf(line,sizeof(line),"%d\n",seqno);
        lseek(fd,0,SEEK_SET);
        write(fd,line,strlen(line));
        myunlock(fd);
    }
    return 0;
}

void mylock(int fd)
{
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(fd,F_SETLKW,&lock);
}

void myunlock(int fd)
{
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(fd,F_SETLK,&lock);
}