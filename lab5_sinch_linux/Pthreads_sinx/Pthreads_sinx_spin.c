#include <stdio.h>
#include <pthread.h>

static int counter=0;

void plus(void *arg);
void minus(void *arg);

int main ()
{
	int input;
	pthread_t p_thread, m_thread;
	pthread_spinlock_t spin;

	pthread_spin_init(&spin,0);
 	
	if(!pthread_create(&p_thread,NULL,&plus,&spin))
	{
		printf("p_thread %d\n",p_thread);
	}
	else { printf("Can not create p_thread\n"); return(0); }
	
	if(!pthread_create(&m_thread,NULL,&minus,&spin))
	{
		printf("m_thread %d\n",m_thread);
	}
	else { printf("Can not create m_thread\n"); return(0); }
	

	pthread_detach(p_thread);
	pthread_detach(m_thread);	
	scanf("%d",&input);
	pthread_cancel(p_thread);
	pthread_cancel(m_thread);
	
	pthread_spin_destroy(&spin);	
	
return(0);
}

void plus(void *arg)
{
	int local,i;
	pthread_spinlock_t *spin=(pthread_spinlock_t *)arg;
	//for(i=0;i<10;i++)
	//{
		pthread_spin_lock(spin);
		local=counter;
		local++;
		counter=local;
		printf("plus=%d\n",local);
		sleep(1);
		printf("plus=%d\n",local);
		pthread_spin_unlock(spin);
		//sleep(1);
		//printf("plus=%d\n",local);
	//}

	return;
}
	
void minus(void *arg)
{
	int local,i;
	pthread_spinlock_t *spin=(pthread_spinlock_t *)arg;
	//for(i=0;i<10;i++)
	//{
		pthread_spin_lock(spin);
		local=counter;
		local--;
		counter=local;
		printf("minus=%d\n",local);
		printf("minus=%d\n",local);
		pthread_spin_unlock(spin);
		//sleep(1);
		//printf("minus=%d\n",local);
	//}

	return;
}

