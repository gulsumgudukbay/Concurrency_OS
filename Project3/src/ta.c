#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N;
int* waiting;

void* studentThread( int studentID)
{
  int r;
  while(1) //this is our fate... no escape from it.
  {
    //wait for TA availability.
    waiting[studentID] = 1;
    printf("student %d is waiting for help...\n", studentID);
    while( waiting[studentID]);

    //sleep for 1 to 5 seconds.
    r = rand() % 5 + 1;
    sleep(r);
  }

  pthread_exit(0); //maybe one day... :(
}

void* taThread()
{
  int r;
  while(1) //and, hahah, you too, should settle your own fate.
  {
    printf("--TA woke up!\n");
    sleep(1);
    //help all students
    for( int i = 0; i < N; i++)
    {
      //help student i
      if( waiting[i])
      {
        printf("TA is helping student %d.\n", i);
        waiting[i] = 0;
        sleep(1);
      }
    }

    //take a nap
    printf("TA has fallen asleep!--\n");
    //sleep for 1 to 5 seconds
    r = rand() % 5 + 1;
    sleep(r);
  }

  pthread_exit(0); //never gonna happen for ya!
}

int main()
{
  pthread_t* students;
  pthread_t ta;

  printf("Enter N: ");
  scanf("%d", &N);

  waiting = (int*) malloc( sizeof(int) * N);
  memset( waiting, 0, N);

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
