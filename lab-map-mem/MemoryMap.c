#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>		

int main(int argc, char** argv)
{
	int n,fd,i;
	unsigned char *ptr;
	struct stat st;
	char mch[256];
	
	do { fd=open("input.txt",O_RDWR); } while( fd==-1 && errno==EINTR );
	if(fd==-1) { const char *const errmsg=strerror(errno); fprintf(stderr,errmsg); }
	
	printf("File is opened fd=%d\n",fd);

	if(fstat(fd,&st)<0) { perror("fstat"); return 1; }
	size_t fsize=(size_t)st.st_size;
	while((n=read(fd,mch,fsize-1))<0) { perror("read"); return 1;}
	for(i=0;i<n;i++)
	{ printf("mch[%d]=%c\n",i,mch[i]); }
	
	ptr=(unsigned char*)mmap(NULL,fsize,PROT_READ|PROT_WRITE,MAP_FILE|MAP_SHARED,fd,0);		
	if(ptr==MAP_FAILED) {perror("mmap"); return 1; }
	for(i=0;i<fsize-1;i++)
	{ printf("%d ptr=%c\n",i,*(ptr+i));
		*(ptr+i)='m'; 
	}
	
	close(fd);

		return 1;

}
