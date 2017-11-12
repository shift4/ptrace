#include <sys/ptrace.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <unistd.h>  
#include <sys/reg.h>

#include <stdio.h>
#include <string.h>

const int long_size = sizeof(long);
union u {  
            long val;  
            char chars[long_size];  
};  

void getdata(pid_t child, long addr,  char *str, int len)  
{     
    char *laddr;  
    int i, j;  
    union u data;  
  
    i = 0;  
    j = len / long_size;  
    laddr = str;  
    while(i < j) {  
        data.val = ptrace(PTRACE_PEEKDATA,   
                          child, addr + i * long_size,   
                          NULL);  
        memcpy(laddr, data.chars, long_size);  
        ++i;  
        laddr += long_size;  
    }  
    j = len % long_size;  
    if(j != 0) {  
        data.val = ptrace(PTRACE_PEEKDATA,   
                          child, addr + i * long_size,   
                          NULL);  
        memcpy(laddr, data.chars, j);  
    }  
    //str[len] = '\0';  
}  
  
void putdata(pid_t child, long addr,  char *str, int len)  
{     
    char *laddr;  
    int i, j; 
    union u data;
  
    i = 0;  
    j = len / long_size;  
    laddr = str;  
    while(i < j) {  
        memcpy(data.chars, laddr, long_size);  
        ptrace(PTRACE_POKEDATA, child,   
               addr + i * long_size, data.val);  
        ++i;  
        laddr += long_size;  
    }  
    j = len % long_size;  
    if(j != 0) {  
        memcpy(data.chars, laddr, j);  
        ptrace(PTRACE_POKEDATA, child,   
               addr + i * long_size, data.val);  
    }  
}  

int main()  
{  
    pid_t child;  
    long orig_eax;  
    child = fork();  
    if(child == 0) {  
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);  
        execl("./bp","bp",NULL);  
    }  
    else {  
        wait(NULL);
        orig_eax = ptrace(PTRACE_PEEKTEXT,   
                          child, (void *)0x400579,   
                          NULL);  
        printf("The child made a "  
               "system call %ld \n", orig_eax);  
        ptrace(PTRACE_SYSCALL, child, NULL, NULL);  
        wait(NULL);
        orig_eax = ptrace(PTRACE_PEEKUSER,   
                          child, 8 * ORIG_RAX,   
                          NULL);  
        printf("The child made a "  
               "system call %ld \n", orig_eax);  
        ptrace(PTRACE_CONT, child, NULL, NULL);  
    }  
    return 0;  
}
