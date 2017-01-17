#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "que.h"

sem_t que_full;
sem_t que_ready;
pthread_mutex_t m, m1, m2;
struct timespec ts;
static ELE _que[QUE_MAX];
static int _front = 0, _rear = 0;
extern int producers_working;

static int matches = 0;

void add_match()
{
    //Note: you will need to lock this update because it is a race condition
    pthread_mutex_lock(&m);
    matches++;
    pthread_mutex_unlock(&m);
}

void report_matches(char *pattern)
{
    printf("Found %d total matches of '%s'\n", matches, pattern);
	pthread_mutex_destroy(&m);
	pthread_mutex_destroy(&m1);
	pthread_mutex_destroy(&m2);
	sem_destroy(&que_full);
	sem_destroy(&que_ready);
}

int que_init()
{
    pthread_mutex_init(&m, NULL);
    pthread_mutex_init(&m1, NULL);
    pthread_mutex_init(&m2, NULL);
    sem_init(&que_full, 0, QUE_MAX-1);
    sem_init(&que_ready, 0 , 0);
    time(&ts.tv_sec);
    ts.tv_sec += 5;
}

void que_error(char *msg)
{
    fprintf(stderr, "***** Error: %s\n", msg);
    // exit(-1);
}

int que_is_full()
{
    return (_rear + 1) % QUE_MAX == _front; /* this is why one slot is unused */
}

int que_is_empty()
{
    return _front == _rear;
}

void que_enq(ELE v)
{
    // replace this spin with something better.....
    //while (que_is_full()) // note this is not right
    //    ;
    sem_wait(&que_full);
    if ( que_is_full() )
        que_error("enq on full queue");
    //lock
    pthread_mutex_lock(&m1);
    _que[_rear++] = v;
    //unlock
    //pthread_mutex_unlock(&m1);
    if ( _rear >= QUE_MAX )
        _rear = 0;
    pthread_mutex_unlock(&m1);
    sem_post(&que_ready);
}

ELE que_deq()
{
    // replace this spin with something better.....
    //while (producers_working && que_is_empty()) // note this is not right
    //    ;
	
    if (sem_timedwait(&que_ready, &ts) == -1)
    {
        ELE emptyRet;
        emptyRet.string[0] = '\0';
	//sem_post(&que_full);
	return emptyRet;
    }
    if ( que_is_empty() )
        que_error("deq on empty queue");
    //lock
    pthread_mutex_lock(&m2);
    ELE ret = _que[_front++];
    //unlock
    //pthread_mutex_unlock(&m2);
    if ( _front >= QUE_MAX )
        _front = 0;
    //unlock
    pthread_mutex_unlock(&m2);
    sem_post(&que_full);
    return ret;
}


/*

int main()
{
    for ( int i=0; i<QUE_MAX-1; ++i )
    {
        Buffer b;
        sprintf(&b.string, "%d", i);
        que_enq(b);
    }
    while ( !que_is_empty() )
        printf("%s ", que_deq().string);
    putchar('\n');
}
*/
