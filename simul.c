/***********************************************************************/
/**      Author: Minas Spetsakis                                      **/
/**        Date: Jun. 2019                                         **/
/** Description: Skeleton code for Assgn. II                               **/
/***********************************************************************/

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "args.h"
#include "error.h"

/* PHILOSOPHER STATES  */
#define THINKING 1
#define HUNGRY 2
#define EATING 3


int N=5, T=100;
double lam=0.1, mu=0.2;
int nblocked;			/* The number of threads blocked */
int thinkingTicks, hungryTicks, eatingTicks; 
int seats;
int start_line = 0;





/***********************************************************************
                         P H I L O S O P H E R
************************************************************************/
void *philosopher(void *vptr)
{
  unsigned int seed;		/* This is called from main without    */
  int pthrerr;			/* creating a new thread               */
  struct thread_arg *ptr;
  int rng;    //int to hold random integer
  ptr = (struct thread_arg*)vptr;
  int start = 0;      // value to wait for start line
  int state = 1;      // state count

  


  // printf("I am %d\n", (ptr -> id));
  while (start == 0)      //start_line creation
    {
      pthrerr = pthread_mutex_lock(ptr -> pMutex);
      if (pthrerr != 0)
        fatalerr("philosopher", pthrerr, "Mutex error! Lock");
      while ((ptr -> id) < (ptr -> N)){
        pthrerr = pthread_cond_wait((ptr -> pstart_line), (ptr -> pMutex)); //wait for condition from main thread to run all threads
        if (pthrerr != 0)
          fatalerr("philosopher", pthrerr, "startline error!");
      }
      
      pthrerr = pthread_mutex_unlock((ptr -> pMutex));
      if (pthrerr != 0)
        fatalerr("philosopher", pthrerr, "Mutex error! Unlock");
      // printf("started thread# %d! \n", (ptr -> id));
      start = 1;      

    }

  while(1){     //dining philosopher algorithm
    rng = rand0_1(&(ptr -> seed));
    pthrerr = pthread_mutex_lock(ptr -> pMutex);
    if (pthrerr != 0)
      fatalerr("philosopher", pthrerr, "Mutex error! lock!");
    
    pthrerr = pthread_cond_wait((ptr -> pclkCond), (ptr -> pMutex));  //wait for clock tick
    if (pthrerr != 0)
      fatalerr("philosopher", pthrerr, "clock Condition error!");

    //roll for state change now

    if(state == THINKING){    //check for THINKING state
      if(rng < (ptr -> lam)){    //if less than lam, change state to HUNGRY and increment thinkingTime
        state = HUNGRY;
      }
      thinkingTicks++;
    }
    else if(state == HUNGRY){   //check for HUNGRY state
      if(seats > 0){    //if enough seats are available, change to EATING state
        state = EATING;
        seats--;
      }
      hungryTicks++;
    }
    else if(state == EATING){
      if(rng < (ptr -> mu)){    //if done eating, change to THINKING state
        state = THINKING;  
        seats++;
      }
      eatingTicks++;
    }
    start++;
    pthread_mutex_unlock(ptr-> pMutex);   //unlock mutex
    if(start > (ptr -> T))
      printf("Thinking=%d Hungry=%d Eating=%d \n", thinkingTicks, hungryTicks, eatingTicks);

      exit(0);
  }


  return NULL;
      
}

/***********************************************************************
                                C L K
************************************************************************/
void *clk(void *vptr)
{

  int tick;
  int pthrerr;
  struct thread_arg *ptr;

  ptr = (struct thread_arg*)vptr;

  for(tick = 0; tick < (ptr -> T); tick++){     //Clock Tick for philosophers 
    pthrerr = pthread_cond_broadcast((ptr -> pclkCond));
    if (pthrerr != 0)
      fatalerr("CLK", pthrerr, "Clock Broadcast Failed!");
  }
  // printf("Thinking=%d Hungry=%d Eating=%d \n", thinkingTicks, hungryTicks, eatingTicks);
  printf("Average Thinking Time: %6.2f\n", (float)thinkingTicks/(float)(T * N));
  printf("Average Hungry Time: %6.2f\n", (float)hungryTicks/(float)(T * N));
  printf("Average Eating Time: %6.2f\n", (float)eatingTicks/(float)(T * N));
  


  // fatalerr("CLK", 0, "Not yet implemented");
  exit(0);
}

/***********************************************************************
                               M A I N
************************************************************************/
int main(int argc, char **argv)
{
  int pthrerr, i;
  int nserver, nclient, nTicks;
  float lam, mu;


  int i_c = 1;
  while (i_c < argc - 1)
  {
    if (strcmp(argv[i_c], "-N") == 0)
    {
      N = atoi(argv[++i_c]);
    }
    else if (strcmp(argv[i_c], "-T") == 0)
    {
      nTicks = atoi(argv[++i_c]);
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
//  printf("N = %d\n", N);

  pthread_t phil_tid;
  pthread_cond_t blockCond, clkCond, startline;
  pthread_mutex_t cpMutex;
  struct thread_arg *allargs;
  pthread_t *alltids;
  thinkingTicks = 0;
  hungryTicks = 0;
  eatingTicks = 0;

  pthrerr = pthread_cond_init(&blockCond, NULL);
  if (pthrerr!=0)		/* Initializers never return an error code */
    fatalerr(argv[0], 0,"Initialization failed\n");
  pthrerr = pthread_cond_init(&clkCond, NULL);
  if (pthrerr!=0)
    fatalerr(argv[0], 0,"Initialization failed\n");
  pthrerr = pthread_cond_init(&startline, NULL);
  if (pthrerr!=0)
    fatalerr(argv[0], 0,"Initialization failed\n");
  pthrerr = pthread_mutex_init(&cpMutex, NULL);
  if (pthrerr!=0)
    fatalerr(argv[0], 0,"Initialization failed\n");

  allargs = (struct thread_arg *)
    malloc((N+1) * sizeof(struct thread_arg));
  if (allargs==NULL)
    fatalerr(argv[0], 0,"Out of memory\n");

  alltids = (pthread_t*)
    malloc((N)*sizeof(pthread_t));
  if (alltids==NULL)
    fatalerr(argv[0], 0,"Out of memory\n");

  allargs[0].N = N;
  allargs[0].T = nTicks;
  allargs[0].lam = lam;
  allargs[0].mu = mu;
  allargs[0].id = 1;
  allargs[0].pBlockCond = &blockCond;
  allargs[0].pclkCond = &clkCond;
  allargs[0].pMutex = &cpMutex;
  allargs[0].pstart_line = &startline;
  allargs[0].seed = 40;
  allargs[0].thinkingTime = 0;
  allargs[0].hungryTime = 0;
  allargs[0].eatingTime = 0;
  seats = N - 1;

  for(i = 0; i < N; i++){
    allargs[i] = allargs[0];
    allargs[1].seed+= i;
    allargs[i].id += i;
    pthrerr = pthread_create(alltids+i, NULL, philosopher, allargs+i);
    if(pthrerr != 0){
      fatalerr(argv[0], pthrerr, "Philosopher creation failed!\n");
    }
  }
  start_line = 1;
  pthread_cond_broadcast(allargs[0].pstart_line);


  clk((void*) (allargs + N));
  printf("Should not be here\n");
  exit(-1);
}
