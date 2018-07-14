#include<stdio.h>
#include<sys/mman.h>
#include<unistd.h>
#include<alloca.h>

struct s{
	int a;
	char b;
	long c;
	long d;
	long f;
	long g;
};

struct s fun(struct s a)
{
	struct s b;
	b.a = a.a;
	b.b = a.b;
	b.g = a.g;
	return b;
}

int main()
{
	int i1 = 12345;
	int i2;
	int *ip1,*ip2;
	FILE *fp;

	struct s s1 = {.a=1,.b='a',.g=3};
	struct s s2;
	s2 = fun(s1);
	printf("%d",s2.b);

	if((fp = fopen("data","w")) == NULL){
		puts("open err");
	}
	if(fwrite(&i1,sizeof(i1),1,fp) != 1){
		puts("write err");
	}
	fclose(fp);

	if((fp = fopen("data","r+")) == NULL){
		puts("open err2");
	}
	ip1 = (int *)mmap(NULL,sizeof(int),PROT_READ,MAP_PRIVATE,fileno(fp),0);
	//ip2 = (int *)mmap(NULL,sizeof(int),PROT_WRITE,MAP_PRIVATE,fileno(fp),0);
	ip2 = (int *)mmap(NULL,sizeof(int),PROT_WRITE,MAP_SHARED,fileno(fp),0);
	if(ip2 == MAP_FAILED){
		perror("mmap2 failed");
	}
	//*ip2 = 54321;
	//msync(ip2,sizeof(int),MS_SYNC);
	if(fork()==0){
		*ip2 = 1;
	}else{
		sleep(3);
	}
	char *buf = (char *)alloca(128);
	sprintf(buf,"%d%d%d%d%d%d\n",*ip1,1,2,3,4,5);
	puts(buf);

	fclose(fp);
	return 0;
}

void __sanitizer_cov_trace_pc_guard_init(int *start, int *stop) 
{
  if (start == stop || *start) return;
}

void __sanitizer_cov_trace_pc_guard(int *g)
{
  void *PC = __builtin_return_address(0);
  char PcDescr[1024];
  //__sanitizer_symbolize_pc(PC, "%p = %F = %L", PcDescr, sizeof(PcDescr));
  printf("%s\n",PcDescr);
}

void __sanitizer_cov_trace_pc(void)
{
  void *PC = __builtin_return_address(0);
  printf("%p\n",PC);
}
