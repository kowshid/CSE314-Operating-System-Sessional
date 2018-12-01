#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <unistd.h>

using namespace std;

int cake;

queue<int> q1, q2, q3; //q1->mix, q2->vanilla, q3->chocolate

//Empty -> to push, Occupied -> to pop; initially can push but can't pop
sem_t q1Empty;
sem_t q1Occupied;
sem_t q2Empty;
sem_t q2Occupied;
sem_t q3Empty;
sem_t q3Occupied;

pthread_mutex_t q1Lock;
pthread_mutex_t q2Lock;
pthread_mutex_t q3Lock;
pthread_mutex_t outputLock;

void init_semaphore()
{
	sem_init(&q1Empty,0,5);
	sem_init(&q1Occupied,0,0);

	sem_init(&q2Empty,0,5);
	sem_init(&q2Occupied,0,0);

	sem_init(&q3Empty,0,5);
	sem_init(&q3Occupied,0,0);

	pthread_mutex_init(&outputLock,0);
	pthread_mutex_init(&q1Lock,0);
	pthread_mutex_init(&q2Lock,0);
	pthread_mutex_init(&q2Lock,0);
}

void * vanilla(void * arg)
{
	pthread_mutex_lock(&outputLock);
	printf("\nVanilla check\n");
	pthread_mutex_unlock(&outputLock);

	while(1)
	{
		sem_wait(&q1Empty);

		pthread_mutex_lock(&q1Lock);
		q1.push(0);
		pthread_mutex_unlock(&q1Lock);

		pthread_mutex_lock(&outputLock);
		printf("\nChef-Y baked a Vanilla cake\n");
		printf("Items in queue 1: %d\n", q1.size());
		pthread_mutex_unlock(&outputLock);

		sem_post(&q1Occupied);

		sleep(rand()%4+1);
	}
}

void * chocolate(void * arg)
{
	pthread_mutex_lock(&outputLock);
	printf("\nChocolate check\n");
	pthread_mutex_unlock(&outputLock);

	while(1)
	{
		sem_wait(&q1Empty);

		pthread_mutex_lock(&q1Lock);
		q1.push(1);
		pthread_mutex_unlock(&q1Lock);

		pthread_mutex_lock(&outputLock);
		printf("\nChef-X baked a Chocolate cake\n");
		printf("Items in queue 1: %d\n", q1.size());
		pthread_mutex_unlock(&outputLock);

		sem_post(&q1Occupied);

		sleep(rand()%4+1);
	}
}

void * decorator(void * arg)
{
	pthread_mutex_lock(&outputLock);
	printf("\nDecorator check\n");
	pthread_mutex_unlock(&outputLock);

	while(1)
	{
		sem_wait(&q1Occupied);

		pthread_mutex_lock(&q1Lock);

		cake = q1.front();
		q1.pop();

		pthread_mutex_unlock(&q1Lock);

		if(cake == 0) //vanilla cake going to queue 2
		{
			pthread_mutex_lock(&outputLock);
			printf("\nChef-Z poped a vanilla cake\n");
			printf("Items in queue 1: %d\n", q1.size());
			pthread_mutex_unlock(&outputLock);

			sem_wait(&q2Empty);

			pthread_mutex_lock(&q2Lock);
			q2.push(cake);
			pthread_mutex_unlock(&q2Lock);

			pthread_mutex_lock(&outputLock);
			printf("\nChef-Z decorated a vanilla cake\n");
			printf("Items in queue 2: %d\n", q2.size());
			pthread_mutex_unlock(&outputLock);

			sem_post(&q2Occupied);
		}

		if(cake == 1) //chocolate cake going to queue 3
		{
			pthread_mutex_lock(&outputLock);
			printf("\nChef-Z poped a chocolate cake\n");
			printf("Items in queue 1: %d\n", q1.size());
			pthread_mutex_unlock(&outputLock);

			sem_wait(&q3Empty);

			pthread_mutex_lock(&q3Lock);
			q3.push(cake);
			pthread_mutex_unlock(&q3Lock);

			pthread_mutex_lock(&outputLock);
			printf("\nChef-Z decorated a chocolate cake\n");
			printf("Items in queue 3: %d\n", q3.size());
			pthread_mutex_unlock(&outputLock);

			sem_post(&q3Occupied);
		}

		//pthread_mutex_unlock(&lock);
		sem_post(&q1Empty);

		sleep(1);
	}
}

void * waiter1(void *arg)
{
	pthread_mutex_lock(&outputLock);
	printf("\nWaiter 1 check\n");
	pthread_mutex_unlock(&outputLock);

	while(1)
	{
		sem_wait(&q3Occupied);

		pthread_mutex_lock(&q3Lock);
		q3.pop();
		pthread_mutex_unlock(&q3Lock);

		pthread_mutex_lock(&outputLock);
		printf("\nWaiter 1 served a chocolate cake\n");
		printf("Items in queue 3: %d\n", q3.size());
		pthread_mutex_unlock(&outputLock);

		sem_post(&q3Empty);

		sleep(rand()%4+1);
	}
}

void * waiter2(void *arg)
{
	pthread_mutex_lock(&outputLock);
	printf("\nWaiter 2 check\n");
	pthread_mutex_unlock(&outputLock);

	while(1)
	{
		sem_wait(&q2Occupied);

		pthread_mutex_lock(&q2Lock);
		q2.pop();
		pthread_mutex_unlock(&q2Lock);

		pthread_mutex_lock(&outputLock);
		printf("\nWaiter 2 served a vanilla cake\n");
		printf("Items in queue 2: %d\n", q2.size());
		pthread_mutex_unlock(&outputLock);

		sem_post(&q2Empty);

		sleep(rand()%4+1);
	}
}

int main()
{
	init_semaphore();

	pthread_t van;
	pthread_t choc;
	pthread_t decor;
	pthread_t wait1;
	pthread_t wait2;

	pthread_create(&van, NULL, vanilla, (void *) 0);
    pthread_create(&choc, NULL, chocolate, (void *) 0);
    pthread_create(&decor, NULL, decorator, (void *) 0);
	pthread_create(&wait1, NULL, waiter1, (void *) 0);
	pthread_create(&wait2, NULL, waiter2, (void *) 0);

    while(1);

    return 0;
}
