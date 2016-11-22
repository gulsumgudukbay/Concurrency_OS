#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10


int chairs[3];
int available_chairs;
int sleeping;

int* waiting;
pthread_mutex_t sleeplock;
pthread_mutex_t chairlock;

void* studentThread( int studentID)
{
  int r;
  int special;

  special = 0;
  while(1) //this is our fate... no escape from it.
  {
    //take a nap
    pthread_mutex_lock( &sleeplock);
    if( sleeping)
    {
      special = 1;
      sleeping = 0;
      printf("Student %d awakes TA\n", studentID);
      printf("TA helps to student %d\n", studentID);
      sleep(1);
    }
    pthread_mutex_unlock( &sleeplock);

    if( special)
    {
      special = 0;
      r = rand() % 5 + 15;
      sleep(r);
      continue;
    }

    pthread_mutex_lock( &chairlock);
    if( available_chairs)
    {
      waiting[ studentID] = 1;
      printf("Student %d sits and waits\n", studentID);
      chairs[ 3 - available_chairs] = studentID;
      available_chairs--;
      sleep(1);
    }
    pthread_mutex_unlock( &chairlock);

    while( waiting[ studentID]);
    printf("Student %d resumes programming\n", studentID);
    sleep(1);

    //sleep thread for 15 to 20 seconds.
    r = rand() % 5 + 15;
    sleep(r);
  }

  pthread_exit(0); //maybe one day... :(
}

void* taThread()
{
  int r;
  while(1) //and, hahah, you too, should settle your own fate.
  {
    while( sleeping);
    sleep(1);

    printf("TA checks chairs\n");
    while( available_chairs < 3)
    {
      pthread_mutex_lock( &chairlock);
      printf("TA helps to student %d\n", chairs[0]);
      waiting[ chairs[0]] = 0;
      chairs[0] = chairs[1];
      chairs[1] = chairs[2];
      chairs[2] = -1;
      available_chairs++;
      sleep(1);
      pthread_mutex_unlock( &chairlock);

      printf("TA checks chairs\n");
      sleep(1);
    }

    //take a nap
    pthread_mutex_lock( &sleeplock);
    sleeping = 1;
    printf("TA takes a nap\n");
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
  waiting = (int*) malloc( sizeof(int) * N);
  memset( waiting, 0, N);
  memset( chairs, -1, N);
  available_chairs = 3;
  sleeping = 1;
  pthread_mutex_init( &chairlock, 0);
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
