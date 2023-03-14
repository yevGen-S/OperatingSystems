#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAXTHREADS 2
//static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *produce(void *arg);
sem_t sem;

int main ()
{
	int i, count[MAXTHREADS];
	pthread_t tid_produce[MAXTHREADS];

	sem_init(&sem,0,1);

 	for(i=0; i< MAXTHREADS; i++)
	{
		count[i]=i;
		if(!pthread_create(&tid_produce[i],NULL,produce,&count[i]))
		{
			printf("tid %d\n",tid_produce[i]);
		}
		else { printf("Can not create thread\n"); return(0); }
	}
	
	for(i=0; i< MAXTHREADS; i++)
	{
		pthread_join(tid_produce[i],NULL);
		
		printf("count[%d]=%d\n", i, count[i]);
		
	}
	if(sem_destroy(&sem)<0) { perror("sem_destroy\n"); return(0); }
	printf("sem_destroy\n");
return(0);
}

void *produce(void *arg)
{	
	char str[50];	
	pthread_t tid; 
	int num;
	num=*(int *)arg;

	printf("Hello I am thread %d\n",num);
	while(1)
	{
		printf("I am thread %d in while\n",num);		
		strcpy(str,"t");
		sem_wait(&sem); perror("sem_wait");
		
		//gets(str);
		//printf("user input %s\n",str);
		while(strcmp(str,"good buy"))
		{
			if(!strcmp(str,"exit"))
			{
				sem_post(&sem); perror("sem_post");
				printf("Thread %d finished\n",num);
				return(0);
			}
			fgets(str, 50, stdin);
			printf("user input in thread %d %s\n",num,str);
		}
		printf("Good buy\n");
		sem_post(&sem); perror("sem_post");
		printf("Thread %d released control\n",num);
		
	}
	
}
	


