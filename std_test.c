#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<bsd/unistd.h>

struct call{
    unsigned long addr;
    char funcname[50];
    unsigned long taddr;
    char tfuncname[50];
};

struct call calls[10000];

char cmdname[50];

extern void __sanitizer_symbolize_pc(void *, char *, char *, int);

void __sanitizer_cov_trace_pc_guard(int *g)
{
    /*
	void *PC = __builtin_return_address(0);
    char PcDesc[1024];
    __sanitizer_symbolize_pc(PC,"%p %F %L",PcDesc,sizeof(PcDesc));
    printf("guard:%p %x PC %s\n",g,*g,PcDesc);
    */
    if(strstr(cmdname,"child")){
        printf("[%d]%s\n",getpid(),cmdname);
    }
}

int main(int argc, char **argv, char **envp)
{
    int i = 0;
    FILE *fp;
    
    if(fork()==0){
        setproctitle_init(argc,argv,envp);
        setproctitle("child");
    }
    strcpy(cmdname,argv[0]);

    memset(calls,10000,sizeof(struct call));
    fp = fopen("calls","r");
    while(!feof(fp)){
        fscanf(fp,"%lx%s%lx%s",&calls[i].addr,calls[i].funcname,
                                &calls[i].taddr,calls[i].tfuncname);
        i++;
    }
    i = 0;
    while(calls[i].addr != 0){
        if(strstr(calls[i].tfuncname,"*%rax")!=NULL){
            printf("%ld%s\n",calls[i].addr,calls[i].tfuncname);
        }
        i++;
    }
    fclose(fp);
    
    return 0;
}
