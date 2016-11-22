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
pthread_mutex_t sleepm;
pthread_cond_t ta_sleeping = PTHREAD_COND_INITIALIZER;
pthread_cond_t ta_helping = PTHREAD_COND_INITIALIZER;


void* studentThread( int studentID)
{
  while(1)
  {
    sleep( rand() % 6 + 1);

    //wake up ta if sleeping
    pthread_mutex_lock( &sleeplock);
    if( ta_state == 0)
    {
      ta_state = 1;
      waker = studentID;
      printf("Student %d awakes TA\n", studentID);
      pthread_cond_signal( &ta_sleeping);
      pthread_mutex_unlock( &sleeplock);
      continue;
    }
    pthread_mutex_unlock( &sleeplock);

    //is there chair available
    pthread_mutex_lock( &chairlock);
    if( available_chairs)
    {
      //take a seat
      chairs[ 3 - available_chairs] = studentID;
      available_chairs--;

      printf("Student %d sits and waits\n", studentID);
    }
    pthread_mutex_unlock( &chairlock);

  }

  pthread_exit(0); //maybe one day... :(
}

void* taThread() //koca yurekli!
{
  while(1)
  {
    //wake up and help
    pthread_mutex_lock( &sleeplock);
    while( ta_state == 0)
      pthread_cond_wait( &ta_sleeping, &sleeplock);
    printf("TA helps to student %d\n", waker);
    pthread_mutex_unlock( &sleeplock);

    //check chairs

    printf("TA checks chairs\n");
    while( available_chairs < 3)
    {
      pthread_mutex_lock( &chairlock);
      printf("TA helps to student %d\n", chairs[0]);
      chairs[0] = chairs[1];
      chairs[1] = chairs[2];
      chairs[2] = -1;
      available_chairs++;
      pthread_mutex_unlock( &chairlock);
      printf("TA checks chairs\n");
    }

    //sleep
    pthread_mutex_lock( &sleeplock);
    ta_state = 0;
    printf("TA takes a nap\n\n");
    sleep(1);
    pthread_mutex_unlock( &sleeplock);
  }
  pthread_exit(0); //never gonna happen for ya!
}


int main()
{
  pthread_t* students;
  pthread_t ta;

  //init
  memset( chairs, -1, N);
  available_chairs = 3;
  ta_state = 0;
  pthread_mutex_init( &sleeplock, 0);
  pthread_mutex_init( &chairlock, 0);
  pthread_mutex_init( &sleepm, 0);

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
