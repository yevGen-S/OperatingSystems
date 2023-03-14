#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

int main(int argc, char** argv)
{
	int fdin, fdout;	
	void *src, *dst;
	struct stat st;

	if(argc!=3) { printf("use: %s <fromfile> <tofile>",argv[0]); return 1;  }

	if((fdin=open(argv[1],O_RDONLY))<0) { printf("can't open %s for read\n",argv[1]); 
						return 1; }
	if((fdout=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0777))<0) { printf("can't open %s for write\n",argv[2]); 
						return 1; }
	
	if(fstat(fdin,&st)<0) { perror("fstat"); return 1; }
	if(lseek(fdout,st.st_size-1,SEEK_SET)==-1) { perror("lseek"); return 1; }
	if(write(fdout,"",1)!=1) { perror("write"); return 1; }
	
	if((src=mmap(NULL,st.st_size,PROT_READ,MAP_FILE|MAP_SHARED,fdin,0))==MAP_FAILED) {perror("mmap"); return 1; }
	if((dst=mmap(NULL,st.st_size,PROT_READ|PROT_WRITE,MAP_FILE|MAP_SHARED,fdout,0))==MAP_FAILED) 
	{perror("mmap"); return 1; }
	memcpy(dst,src,st.st_size);
	close(fdin);close(fdout);
	munmap(src,st.st_size); munmap(dst,st.st_size);
	close(src); close(dst);
	return 0;
}
