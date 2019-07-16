struct thread_arg
{
  int N, T, id;
  float lam, mu;
  int thinkingTime, hungryTime, eatingTime; 
  pthread_cond_t *pBlockCond, *pclkCond, *pstart_line;	/* find a better name */
  pthread_mutex_t *pMutex;
  unsigned int seed;		/* Needed for the random number genarator */
};
