#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10


int chairs[3];
int available_chairs;
int ta_state; // 0: sleeping, 1:helping, 2:checking chairs
int waker;
int* helped;

pthread_mutex_t sleeplock;
pthread_mutex_t chairlock;
pthread_mutex_t* helplock;


pthread_cond_t ta_sleeping = PTHREAD_COND_INITIALIZER;
pthread_cond_t ta_helping = PTHREAD_COND_INITIALIZER;
pthread_cond_t* help_signal;


void* studentThread( int studentID)
{
  while(1)
  {
    sleep( rand() % 6 + 1);

    pthread_mutex_lock( &helplock[ studentID]);
    helped[ studentID] = 0;
    pthread_mutex_unlock( &helplock[ studentID]);

    //wake up ta if sleeping
    pthread_mutex_lock( &sleeplock);
    if( ta_state == 0)
    {
      ta_state = 1;
      waker = studentID;
      printf("Student %d awakes TA\n", studentID);
      pthread_cond_signal( &ta_sleeping);
      pthread_mutex_unlock( &sleeplock);

      //wait for help
      pthread_mutex_lock( &helplock[ studentID]);
      while( !helped[studentID])
        pthread_cond_wait( &help_signal[ studentID], &helplock[studentID]);
      pthread_mutex_unlock( &helplock[ studentID]);
      printf("Student %d resumes programming\n", studentID);
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
      pthread_mutex_unlock( &chairlock);

      //wait for help
      pthread_mutex_lock( &helplock[ studentID]);
      while( !helped[studentID])
        pthread_cond_wait( &help_signal[ studentID], &helplock[studentID]);
      pthread_mutex_unlock( &helplock[ studentID]);
      printf("Student %d resumes programming\n", studentID);
    }
    else
    {
      printf("Student %d resumes programming\n", studentID);
      pthread_mutex_unlock( &chairlock);
    }

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
    pthread_mutex_unlock( &sleeplock);
    printf("TA helps to student %d\n", waker);

    //signal student
    //
    pthread_mutex_lock( &helplock[ waker]);
    helped[ waker] = 1;
    pthread_cond_signal( &help_signal[ waker]);
    pthread_mutex_unlock( &helplock[ waker]);
    //end signal

    //check chairs
    printf("TA checks chairs\n");
    while( available_chairs < 3)
    {
      int stu;
      pthread_mutex_lock( &chairlock);
      stu = chairs[0];
      printf("TA helps to student %d\n", chairs[0]);
      chairs[0] = chairs[1];
      chairs[1] = chairs[2];
      chairs[2] = -1;
      available_chairs++;


      //signal student
      //
      pthread_mutex_lock( &helplock[ stu]);
      helped[ stu] = 1;
      pthread_cond_signal( &help_signal[ stu]);
      pthread_mutex_unlock( &helplock[ stu]);
      //end signal

      pthread_mutex_unlock( &chairlock);


      printf("TA checks chairs\n");
    }

    //sleep
    pthread_mutex_lock( &sleeplock);
    ta_state = 0;
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
  help_signal = (pthread_cond_t*) malloc( sizeof(pthread_cond_t) * N);
  helplock = (pthread_mutex_t*) malloc( sizeof(pthread_mutex_t) * N);
  helped = (int*) malloc( sizeof(int) * N);
  memset( chairs, -1, N);
  available_chairs = 3;
  ta_state = 0;
  pthread_mutex_init( &sleeplock, 0);
  pthread_mutex_init( &chairlock, 0);
  for( int i = 0; i < N; i++)
  {
    helped[i] = 0;
    pthread_mutex_init( &helplock[i], 0);
    pthread_cond_init( &help_signal[i], 0);
  }


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
