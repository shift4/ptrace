#include <sys/ptrace.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <unistd.h>  
#include <sys/reg.h>

#include <stdio.h>
#include <string.h>

const int long_size = sizeof(long);

void getdata(pid_t child, long addr,  char *str, int len)  
{     
    char *laddr;  
    int i, j;  
    //union u data; 
    union u {  
            unsigned long val;  
            char c[long_size];  
    }data;   
  
    i = 0;  
    j = len / long_size;  
    laddr = str;  
    while(i < j) {  
        data.val = ptrace(PTRACE_PEEKDATA,   
                          child, addr + i * long_size,   
                          NULL);  
        memcpy(laddr, data.c, long_size);  
        ++i;  
        laddr += long_size;  
    }  
    j = len % long_size;  
    if(j != 0) {  
        data.val = ptrace(PTRACE_PEEKDATA,   
                          child, addr + i * long_size,   
                          NULL);  
        memcpy(laddr, data.c, j);  
    }  
    //str[len] = '\0';  
}  
  
void putdata(pid_t child, long addr,  char *str, int len)  
{     
    char *laddr;  
    int i, j; 
    //union u data;
    union u {  
            unsigned long val;  
            char c[long_size];  
    }data;  
  
    i = 0;  
    j = len / long_size;  
    laddr = str;  
    while(i < j) {  
        memcpy(data.c, laddr, long_size);  
        ptrace(PTRACE_POKEDATA, child,   
               addr + i * long_size, data.val);  
        ++i;  
        laddr += long_size;  
    }  
    j = len % long_size;  
    if(j != 0) {  
        memcpy(data.c, laddr, j);  
        ptrace(PTRACE_POKEDATA, child,   
               addr + i * long_size, data.val);  
    }  
}  

int main()  
{  
    pid_t child;  
    //union u data;
    union u {  
            unsigned long val;  
            char c[long_size];  
    }data;
    child = fork();  
    if(child == 0) {  
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);  
        execl("./bp","bp",NULL);  
    }  
    else {  
        wait(NULL);
        data.val = ptrace(PTRACE_PEEKUSER,   
                          child, 8 * RIP,   
                          NULL);  
        printf("The RIP is %lx\n", data.val); 
        getchar(); 

        data.val = ptrace(PTRACE_PEEKTEXT,   
                          child, (void *)0x400579,   
                          NULL);  
        printf("before The instr is %lx\n", data.val);  
        data.c[0] = 0xcc;
        ptrace(PTRACE_POKETEXT, 
                child, (void *)0x400579, data.val);
        printf("after The instr is %lx\n", data.val);
        ptrace(PTRACE_CONT, child, NULL, NULL);  
        getchar();
        wait(NULL);
        data.val = ptrace(PTRACE_PEEKUSER,   
                          child, 8 * RIP,   
                          NULL);  
        printf("The RIP is %lx\n", data.val);  
        ptrace(PTRACE_CONT, child, NULL, NULL);  
    }  
    return 0;  
}
