#include<stdio.h>
#include<sys/mman.h>

int main()
{
	int i1 = 12345;
	int i2;
	int *ip1,*ip2;
	FILE *fp;

	if((fp = fopen("data","w")) == NULL){
		printf("open err");
	}
	if(fwrite(&i1,sizeof(i1),1,fp) != 1){
		printf("write err");
	}
	fclose(fp);

	if((fp = fopen("data","r+")) == NULL){
		printf("open err2");
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
	printf("%d\n",*ip1);
	fclose(fp);
	return 0;
}
