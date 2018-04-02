#include<stdio.h>
#include<string.h>
#include<unistd.h>

struct call{
    unsigned long addr;
    char funcname[50];
    unsigned long taddr;
    char tfuncname[50];
};

struct call calls[10000];

void __sanitizer_cov_trace_pc_guard(int *g)
{
	void *PC = __builtin_return_address(0);
    char PcDesc[1024];
    __sanitizer_symbolize_pc(PC,"%p %F %L",PcDesc,sizeof(PcDesc));
    printf("guard:%p %x PC %s\n",g,*g,PcDesc);
}

int main()
{
    int i = 0;
    FILE *fp;

    //printf("%ld\n",sysconf(_SC_OPEN_MAX));
    
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
