#include <sys/ptrace.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <unistd.h>  
#include <sys/reg.h>
#include <stdio.h>
#include <string.h>

union bp {   
            unsigned long val;
            char c[sizeof(long)];
};


int main()
{
    pid_t child;
    union bp data,data2;
    char bak;

    child = fork();  
    if(child == 0) {  
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);  
        execl("./bp","bp",NULL);  
    }  
    else {
        wait(NULL);
        data.val = ptrace(PTRACE_PEEKUSER, child, 8 * RIP, NULL);  
        printf("The RIP is %lx\n", data.val);

        data.val = ptrace(PTRACE_PEEKTEXT,child, (void *)0x4005d3,NULL);
        printf("before The instr is %lx\n", data.val);  
        bak = data.c[0];
        data.c[0] = 0xcc;
        ptrace(PTRACE_POKETEXT,child, (void *)0x4005d3, data.val);
        printf("after The instr is %lx\n", data.val);
        ptrace(PTRACE_CONT, child, NULL, NULL);  
        wait(NULL);
        data2.val = ptrace(PTRACE_PEEKUSER,child, 8 * RIP,NULL);
        printf("The RIP is %lx\n", data2.val);  
        data.c[0] = bak;
        ptrace(PTRACE_POKEDATA, child, (void *)0x4005d3, data.val); 
        ptrace(PTRACE_POKEUSER, child, 8 * RIP, data2.val-1);
        ptrace(PTRACE_CONT, child, NULL, NULL);  
    }  
    return 0;  
}
