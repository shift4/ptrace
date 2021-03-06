#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

union bp {
    unsigned long val;
    char c[sizeof(long)];
};

struct call
{
    unsigned long addr;
    char funcname[50];
    unsigned long taddr;
    char tfuncname[50];
    unsigned char bak;
};

struct func
{
    unsigned long addr;
    char funcname[50];
};

struct call calls[10000];

struct func funcs[2000];

struct call getcall(unsigned long addr)
{
    int i = 0;
    while (calls[i].addr != addr)
        i++;
    return calls[i];
}

struct func getfunc(unsigned long addr)
{
    int i = 0;
    while (funcs[i].addr != addr)
        i++;
    return funcs[i];
}

int main(int argc, char *argv[])
{
    pid_t child;
    union bp data, data2;
    char bak;
    FILE *fp;
    int i = 0;
    struct call c;
    struct func f;

    memset(calls, 10000, sizeof(struct call));
    fp = fopen("calls", "r");
    while (!feof(fp))
    {
        fscanf(fp, "%lx%s%lx%s", &calls[i].addr, calls[i].funcname,
               &calls[i].taddr, calls[i].tfuncname);
        i++;
    }
    fclose(fp);
    i = 0;
    memset(funcs, 2000, sizeof(struct func));
    fp = fopen("funcs", "r");
    while (!feof(fp))
    {
        fscanf(fp, "%lx%s", &funcs[i].addr, funcs[i].funcname);
        i++;
    }
    fclose(fp);

    child = (pid_t)atoi(argv[1]);
    ptrace(PTRACE_ATTACH, child, NULL, NULL);
    wait(NULL);
    i = 0;
    while (calls[i].addr != 0)
    {
        data.val = ptrace(PTRACE_PEEKTEXT, child, (void *)calls[i].addr, NULL);
        calls[i].bak = data.c[0];
        data.c[0] = 0xcc;
        ptrace(PTRACE_POKETEXT, child, (void *)calls[i].addr, data.val);
        i++;
    }
    ptrace(PTRACE_CONT, child, NULL, NULL);
    while (1)
    {
        wait(NULL);
        data.val = ptrace(PTRACE_PEEKUSER, child, 8 * RIP, NULL);
        //printf("The RIP is %lx\n", data.val);
        c = getcall(data.val - 1);
        if (c.taddr != 0){
            printf("%s > %s\n", c.funcname, c.tfuncname);
        }else{
            memset(&data2, 1, sizeof(union bp));
            if(strstr(c.tfuncname,"*%rax")!=NULL){
                data2.val = ptrace(PTRACE_PEEKUSER, child, 8 * RAX, NULL);
            }else if(strstr(c.tfuncname,"*%rcx")!=NULL){
                data2.val = ptrace(PTRACE_PEEKUSER, child, 8 * RCX, NULL);
            }else if(strstr(c.tfuncname,"*%r9")!=NULL){
                data2.val = ptrace(PTRACE_PEEKUSER, child, 8 * R9, NULL);
            }
            f = getfunc(data2.val);
            printf("%s > %s\n", c.funcname, f.funcname);
        }
        data.val = ptrace(PTRACE_PEEKTEXT, child, (void *)c.addr, NULL);
        data.c[0] = c.bak;
        ptrace(PTRACE_POKEDATA, child, (void *)c.addr, data.val);
        ptrace(PTRACE_POKEUSER, child, 8 * RIP, c.addr);
        ptrace(PTRACE_SINGLESTEP, child, NULL, NULL);
        wait(NULL);
        data.c[0] = 0xcc;
        ptrace(PTRACE_POKEDATA, child, (void *)c.addr, data.val);
        ptrace(PTRACE_CONT, child, NULL, NULL);
    }
    return 0;
}
