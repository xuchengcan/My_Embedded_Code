#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <pthread.h> 
#include <assert.h> 

/************************************************
** A pool contains some following structure nodes 
** which are linked together as a link-list, and
** every node includes a thread handler for the
** task, and its argument if it has any.
*************************************************/
typedef struct task
{ 
	void *(*process) (void *); 
	void *arg;	// arguments for a task 

	struct task *next; 

}task; 

/*
** the thread pool
*/
typedef struct 
{ 
	pthread_mutex_t queue_lock; 
	pthread_cond_t queue_ready; 

	// all waiting tasks
	task *queue_head; 

	/*
	** this field indicates the thread-pool's
	** state, if the pool has been shut down
	** the field will be set to true, and it is
	** false by default.
	*/
	bool shutdown; 

	// containning all threads's tid
	pthread_t *tids; 

	// max active taks
	int thread_num; 

	// current waitting tasks
	int cur_queue_size; 

}thread_pool; 

int pool_add_task(void *(*) (void *), void *); 
void *thread_routine(void *); 


static thread_pool *pool = NULL; 

void pool_init(int thread_num) 
{ 
	pool = (thread_pool *)malloc (sizeof(thread_pool)); 

	pthread_mutex_init (&(pool->queue_lock), NULL); 
	pthread_cond_init (&(pool->queue_ready), NULL); 

	pool->queue_head = NULL; 

	pool->thread_num = thread_num; 
	pool->cur_queue_size = 0; 

	pool->shutdown = false; 

	pool->tids = 
		(pthread_t *)malloc(thread_num * sizeof(pthread_t));

	/*
	** create max_thread_num threads
	*/
	int i;
	for(i=0; i<thread_num; i++)
		pthread_create(&(pool->tids[i]), NULL,
			thread_routine ,NULL); 
} 

// add task into the pool!
int pool_add_task(void *(*process)(void *), void *arg) 
{ 
	/*
	** instruct a new task structure
	** 
	** fill the field by parameters pass by and
	** set the next to NULL, then add this task
	** node to the thread-pool
	*/
	task *newtask = (task *)malloc(sizeof(task)); 
	newtask->process = process;
	newtask->arg = arg;
	newtask->next = NULL;

	/*
	** the queue is a critical source, thus whenever
	** operates the queue, it should be protected by
	** a mutex or a semaphore.
	*/

	// ====================================== //
	pthread_mutex_lock(&(pool->queue_lock)); 
	// ====================================== //
	printf(" add_task  mutex_lock \n");
	/*
	** find the last worker is the pool and then add this
	** one to its tail
	**
	** NOTE: since the worker list which pointed by queue_head
	** has no head-node(which means queue_head could be NULL
	** at first), we should deal with the empty queue carefully.
	*/
	task *last_one = pool->queue_head;
	if(last_one == NULL)
		pool->queue_head = newtask;
	else{
		while (last_one->next != NULL)
			last_one = last_one->next;
		last_one->next = newtask; 
	}

	pool->cur_queue_size++; // waiting tasks increase

	printf(" add_task  mutex_unlock \n");
	// ====================================== //
	pthread_mutex_unlock (&(pool->queue_lock)); 
	// ====================================== //

	// wake up waiting task
	pthread_cond_signal(&(pool->queue_ready));

	return 0; 
} 

void *thread_routine(void *arg)
{ 
	while(1)
	{ 
		// ====================================== //
		pthread_mutex_lock (&(pool->queue_lock)); 
		// ====================================== //
               printf(" %lu  mutex_lock \n",pthread_self());
		/*
		** routine will waiting for a task to run, and the
		** condition is cur_queue_size == 0 and the pool
		** has NOT been shutdowned.
		*/
	        while(pool->cur_queue_size == 0 && !pool->shutdown)
		    pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));

		/*
		** the pool has been shutdowned.
		** unlock before any break, contiune or return
		*/
		if (pool->shutdown){
		    pthread_mutex_unlock (&(pool->queue_lock)); 
		    pthread_exit (NULL); 
		} 

		/*
		** consume the first work in the work link-list
		*/
		pool->cur_queue_size--; 
		task *p = pool->queue_head; 
		pool->queue_head = p->next; 

		printf(" %lu  mutex_unlock \n",pthread_self());
		// ====================================== //
		pthread_mutex_unlock(&(pool->queue_lock)); 
		// ====================================== //


		/*
		** Okay, everything is ready, now excutes the process
		** from the worker, with its argument.
		*/
		(*(p->process))(p->arg); 

		/*
		** when the work is done, free the source and continue
		** to excute another work.
		*/
		free(p); 
		p = NULL; 
	} 

	pthread_exit(NULL); 
} 



/*
** tasks waiting in the queue will be discarded
** but will wait for the tasks which are still
** running in the pool
*/
int pool_destroy(void) 
{ 
	// make sure it wont be destroy twice
	if(pool->shutdown) 
	   return -1;

	pool->shutdown = true;  // set the flag

	// wake up all of the tasks
	pthread_cond_broadcast(&(pool->queue_ready)); 


	// wait for all of the task exit
	int i; 
	for(i=0; i < pool->thread_num; i++) 
		pthread_join(pool->tids[i], NULL); 

	free(pool->tids); 

	// destroy the queue
	task *p = NULL; 
	while (pool->queue_head != NULL) 
	{ 
		p = pool->queue_head; 
		pool->queue_head = pool->queue_head->next; 
		free(p); 
	} 

	// destroy the mutex and cond
	pthread_mutex_destroy(&(pool->queue_lock)); 
	pthread_cond_destroy(&(pool->queue_ready)); 
	
	free(pool); 
	pool=NULL; 
	return 0; 
} 

