#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include<pthread.h>
#include<semaphore.h>
#include<queue>
#include<unistd.h>

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

pthread_mutex_t lock;

void init_semaphore()
{
	sem_init(&q1Empty,0,5);
	sem_init(&q1Occupied,0,0);

	sem_init(&q2Empty,0,5);
	sem_init(&q2Occupied,0,0);

	sem_init(&q3Empty,0,5);
	sem_init(&q3Occupied,0,0);

	pthread_mutex_init(&lock,0);
}

void * vanilla(void * arg)
{
	printf("vanilla check\n");

	while(1)
	{
		sem_wait(&q1Empty);
		pthread_mutex_lock(&lock);

		q1.push(0);

		printf("\nVanilla produced\n");
		printf("Items in queue %d\n", q1.size());

		pthread_mutex_unlock(&lock);
		sem_post(&q1Occupied);

		sleep(1);
	}
}

void * chocolate(void * arg)
{
	printf("chocolate check\n");

	while(1)
	{
		sem_wait(&q1Empty);
		pthread_mutex_lock(&lock);

		q1.push(1);

		printf("\nChocolate produced\n");
		printf("Items in queue %d\n", q1.size());

		pthread_mutex_unlock(&lock);
		sem_post(&q1Occupied);

		sleep(1);
	}
}

void * decorator(void * arg)
{
	printf("decorator check\n");

	while(1)
	{
		sleep(1);
		sem_wait(&q1Occupied);
		pthread_mutex_lock(&lock);

		cake = q1.front();
		q1.pop();

		if(cake == 0) //vanilla cake going to queue 2
		{
			sem_wait(&q2Empty);
			q2.push(cake);
			printf("\ndecorated a vanilla cake\n");
			sem_post(&q2Occupied);
		}

		if(cake == 1) //chocolate cake going to queue 3
		{
			sem_wait(&q3Empty);
			q3.push(cake);
			printf("\ndecorated a vanilla cake\n");
			sem_post(&q3Occupied);
		}

		pthread_mutex_unlock(&lock);
		sem_post(&q1Empty);
	}
}

int main()
{
	init_semaphore();

	pthread_t van;
	pthread_t choc;
	pthread_t decor;

	pthread_create(&van, NULL, vanilla, (void *) 0);
    pthread_create(&choc, NULL, chocolate, (void *) 0);
    pthread_create(&decor, NULL, decorator, (void *) 0);

    while(1);

    return 0;
}
