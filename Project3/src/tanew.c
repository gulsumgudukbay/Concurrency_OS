#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10


int chairs[3];
int available_chairs;
int ta_state; // 0: sleeping, 1:helping, 2:checking chairs
int waker;

pthread_mutex_t sleeplock;
pthread_mutex_t chairlock;
pthread_cond_t ta_sleeping = PTHREAD_COND_INITIALIZER;
pthread_cond_t ta_helping = PTHREAD_COND_INITIALIZER;

void* studentThread( int studentID)
{
  while(1)
  {
    //wake up ta if sleeping
    pthread_mutex_lock( &sleeplock);
    if( ta_state == 0)
    {
      ta_state = 1;
      waker = studentID;
    }
    pthread_cond_signal( &ta_sleeping);
    pthread_mutex_unlock( &sleeplock);

    //is there chair available
    pthread_mutex_lock( &chairlock);
    if( chair_available)
    {
      //take a seat
      available_chairs++;
    }
    pthread_mutex_lock( &chairlock);

    //sıranın gelmesini beqle.
    //get help from ta



  }

  pthread_exit(0); //maybe one day... :(
}

void* taThread()
{
  int more_ppl;

  more_ppl = 0;
  while(1)
  {
    //wake up and help
    pthread_mutex_lock( &sleeplock);
    while( ta_state == 0)
      pthread_cond_wait( &ta_sleeping, &sleeplock);
    printf("TA helps to student %d\n", studentID);
    sleep(1);
    pthread_mutex_unlock( &sleeplock);

    //check chairs
    pthread_mutex_lock( &chairlock);
    if( available_chairs)
    {
      
    }
    pthread_cond_signal( &ta_helping);
    pthread_mutex_unlock( &chairlock);

    //sleep
    pthread_mutex_lock( &sleeplock);
    ta_state = 0;
    pthread_cond_signal( &ta_sleeping);
    pthread_mutex_unlock( &sleeplock);
  }

  pthread_exit(0); //never gonna happen for ya!
}

int main()
{
  pthread_t* students;
  pthread_t ta;

  //init
  waiting = (int*) malloc( sizeof(int) * N);
  memset( chairs, -1, N);
  available_chairs = 3;
  ta_state = 0;

  pthread_mutex_init( &sleeplock, 0);

  students = (pthread_t*) malloc( sizeof(pthread_t) * N);
  for( int i = 0; i < N; i++)
  {
    pthread_create( &students[i], 0, (void*) studentThread, (void*) i);
  }
  pthread_create( &ta, 0, (void*) taThread, 0);

  //never happens xd
  for( int i = 0; i < N; i++)
  {
    pthread_join( students[i], 0);
  }
  pthread_join( ta, 0);
  return 0;
}
