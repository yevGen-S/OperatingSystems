#include <stdio.h>
#include <pthread.h>
#define MAXTHREADS 2
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *produce(void *arg);


int main ()
{
	int i, count[MAXTHREADS];
	pthread_t tid_produce[MAXTHREADS];
 	for(i = 0; i < MAXTHREADS; i++)
	{
		count[i]=i;
		if(!pthread_create(&tid_produce[i],NULL,produce,&count[i]))
		{
			printf("tid %lu\n",tid_produce[i]);
		}
		else { printf("Can not create thread\n"); return(0); }
	}
	
	for(i=0; i < MAXTHREADS; i++)
	{
		pthread_join(tid_produce[i],NULL);
		
		printf("count[%d]=%d\n", i, count[i]);
		
	}
return(0);
}

void *produce(void *arg)
{	pthread_t tid; 
	int num;
	num=*(int *)arg;
	pthread_mutex_lock(&mutex);

	printf("Hello I am thread %d\n",num);
	sleep(5);
	printf("Hello I am thread %d\n",num);

	pthread_mutex_unlock(&mutex);

	tid=pthread_self();
	printf("Inside tid=%lu\n",tid);
	return(0);
}
	


