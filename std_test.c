#include<stdio.h>
#include<string.h>

struct call{
    unsigned long addr;
    char funcname[50];
    unsigned long taddr;
    char tfuncname[50];
};

struct call calls[10000];

int main()
{
    int i = 0;
    FILE *fp;

    memset(calls,10000,sizeof(struct call));
    fp = fopen("calls","r");
    while(!feof(fp)){
        fscanf(fp,"%lx%s%lx%s",&calls[i].addr,calls[i].funcname,
                                &calls[i].taddr,calls[i].tfuncname);
        i++;
    }
    i = 0;
    while(calls[i].addr != 0){
        printf("%ld%s\n",calls[i].addr,calls[i].tfuncname);
        i++;
    }
    fclose(fp);
    return 0;
}