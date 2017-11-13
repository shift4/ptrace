#include <sys/ptrace.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <unistd.h>  
#include <sys/reg.h>

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