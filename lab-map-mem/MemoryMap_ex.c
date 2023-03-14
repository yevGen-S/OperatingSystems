#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE 50
#define SHM_CREATE 1
#define SHM_PRINT 3

void usage(const char* s)
{ printf("Usage: %s <create|print> "text"\n",s); }

int main(int argc, char** argv)
{
	int shm,len,cmd,mode=0;
	char *addr;
	
	if(argc<2) { usage(argv[0]); return 1; }
	if((!strcmp(argv[1],"create")&& argc==3))
	{
		len=strlen(argv[2]);
		len=(len<=SHARED_MEMORY_OBJECT_SIZE)? len:SHARED_MEMORY_OBJECT_SIZE;
		
	mode=O_CREAT; cmd=SHM_CREATE;
	}
	else if(!strcmp(argv[1],"print")) {cmd=SHM_PRINT; }
	if((shm=shm_open(SHARED_MEMORY_OBJECT_NAME,mode|O_RDWR,0777))==-1)
	{ perror("shm_open"); return 1; }
	if(cmd==SHM_CREATE)
	{ if(ftruncate(shm,SHARED_MEMORY_OBJECT_SIZE+1)==-1)
		{ perror("fruncate"); return 1; }
	}
	addr=mmap(0,SHARED_MEMORY_OBJECT_SIZE+1,PROT_READ|PROT_WRITE,MAP_SHARED,shm,0);		
	if(addr==MAP_FAILED) {perror("mmap"); return 1; }
	switch(cmd)
	{
		case SHM_CREATE: memcpy(addr,argv[2],len); addr[len]='\0';
					printf("write in mem\n"); break; 
		case SHM_PRINT: printf("read from mem %s \n",addr); break;
	}
		return 1;

}
