/***********************************************************************/
/**      Author: Minas Spetsakis                                      **/
/**        Date: Jun. 2019                                            **/
/** Description: Skeleton code for Assgn. II                          **/
/***********************************************************************/

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "args.h"
#include "error.h"

int N = 5, T = 100;
double lam = 0.1, mu = 0.2;
int nblocked; /* The number of threads blocked */

int start_line = 0;

int sum = 0;

struct phil_struct
{
  int limit;
  int answer; 
};

/***********************************************************************
                         P H I L O S O P H E R
************************************************************************/
void *philosopher(void *vptr) //aka Threads
{
  // unsigned int seed; /* This is called from main without    */
  // int pthrerr;       /* creating a new thread               */

  while (start_line != 1)
  {
    //We block here untill we get the start signal from main
  }
  struct phil_struct *arg_struct = (struct phil_struct*) vptr;
  for (int i = 0; i < arg_struct->limit; i++)
  {
    sum += i;
  }
  arg_struct->answer = sum;

  // while (1)
  // {
  //   fatalerr("philosopher", 0, "Not yet implemented");
  // }

  pthread_exit(0);
}

/***********************************************************************
                                C L K
************************************************************************/
void *clk(void *vptr)
{
  int tick;
  int pthrerr;
  struct thread_arg *ptr;

  ptr = (struct thread_arg *)vptr;

  

  fatalerr("server", 0, "Not yet implemented");
  exit(0);
}

/***********************************************************************
                               M A I N
************************************************************************/
int main(int argc, char **argv)
{
  int pthrerr, i;
  int nticks;
  float lam, mu;

  int test_value = 1000;
  //Get our args
  int i_c = 1;
  while (i_c < argc - 1)
  {
    if (strcmp(argv[i_c], "-N") == 0)
    {
      N = atoi(argv[++i_c]);
    }
    else if (strcmp(argv[i_c], "-T") == 0)
    {
      nticks = atoi(argv[++i_c]);
    }
    else if (strcmp(argv[i_c], "-L") == 0)
    {
      lam = atof(argv[++i_c]);
    }
    else if (strcmp(argv[i_c], "-M") == 0)
    {
      mu = atof(argv[++i_c]);
    }
    else
    {
      fatalerr(argv[i_c], 0, "Invalid argument\n");
    }
  }

  int num_of_threads = N;

  //Launch threads
  pthread_t tids[num_of_threads]; //array thread id's
  pthread_cond_t something;
  pthread_mutex_t blah;
  struct thread_arg *allargs;
  pthread_t *alltids;

  //Build our struct outside forloop so we can access it later
  struct phil_struct struc_args[num_of_threads];
  //Start our threads
  for (int i = 0; i < num_of_threads; i++)
  {
    //Assign the limit in one of the struct in our struct array
    struc_args[i].limit = test_value;

    //default attributes (change later)
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    //create pthread
    pthread_create(&tids[i], &attr, philosopher, &struc_args[i]); 
  }

  //Since all the threads are now running we can give them the signal to start 
  
  start_line = 1; //Start all the threads

  //wait for all threads to finish
  for (int i = 0; i < num_of_threads; i++)
  {
    pthread_join(tids[i], NULL);
    printf("Sum of thread %d is: %d\n",pthread_self, struc_args[i].answer);
  }

  printf("Should not be here\n");
  exit(-1);
}
