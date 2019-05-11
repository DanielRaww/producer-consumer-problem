// Daniel Raw, Gabriel Espinoza
//CSCI312

#include <stdio.h>
#include<pthread.h>
#include <stdlib.h>


// Generate one producer and two consumers. The producer generates N tasks, where N is a parameter passed in via the command line.
// The task queue has two slots.
// If the queue is empty, cosumers cannot extract tasks from the queue, while if the queue is full, the producer cannot put task in the queue.
// Print out each step: (1) Who inserts task and who extracts task. (2) At the end of execution, print how many tasks each consumer extracts.


int cur = 0; //initialize variable - cur, and sets that equal to 0
pthread_mutex_t task_queue_lock; //returns the handle, task_queue_lock
int queue[2] = {0,0};
int i; //initalize variable i

void *producer(){
    while (cur < i){ //boolean, when cur is less than i
        pthread_mutex_lock(&task_queue_lock); //critical section below, this blocks the thread if another thread holds the lock
        if (queue[0] == 0 && queue[1] == 0){ //when the queue[0] is 0 and queue[1] is 0
            queue[0] = 100; //change the value of queue[0] to 100
            cur += 1; //increment cur

            printf("This task %d is inserted by producer #1\n", cur);

        }
        else if (queue[0] != 0 && queue[1] ==  0){ //when the task[0] is not 0 and the task[1] is 0
            queue[1] = 100; //set queue 1 to 100
            cur += 1; //increment cur
            printf("This task is inserted by producer #1\n");

        }
        else{

        }
         pthread_mutex_unlock(&task_queue_lock); //makes the mutex thread unlocked
    }
    pthread_exit(NULL); //exits the thread
}

void *consumer(void *s){ // * makes it so it can be converted to other pointer type without explicit cast

    int local_tasks = 0; //initializes local_tasks variable and sets it equal to 0
    int *loc_id = (int*)s; // * makes it so it can be converted to other pointer type without explicit cast
    while (cur < i || queue[0] == 100){ //boolean check, if the cur is less than i or queue[0] is equal to 100
        pthread_mutex_lock(&task_queue_lock); //critical section below, this blocks the thread if another thread holds the lock
        if (queue[0] == 100 && queue[1] == 100){ //if queue[0] is equal to 100 and queue[1] is equal to 100
            queue[0] = 0; //set queue[0] to 0
            queue[0] = 100; //then changes queue[0] back to 100
            queue[1] = 0; //sets queue[1] to 0
            local_tasks += 1; //increment local_tasks
            //printf("hi");
            printf("This task is extracted by consumer #%d\n", *loc_id);
        }
        else if (queue[0] == 100 && queue[1] == 0){ //if queue[0] is 100 and queue[1] is 0
            queue[0] = 0; //set queue[0] to 0
            local_tasks += 1; //increment local_tasks
            printf("This task is extracted by consumer #%d\n", *loc_id);
        }
        else{

        }
        pthread_mutex_unlock(&task_queue_lock); //makes the mutex thread unlocked
    }
    printf("Cousumer #%d extracted %d tasks in total.\n", *loc_id, local_tasks);
    pthread_exit(0); //exits the thread and returns 0
}





int main(int argc, char *argv[]){ //args is the number of strings being pointed to argv

    i = atoi (argv[1]);  //integer representation of argc[1] is stored in i

    pthread_t p_threads[3]; //returns the handle, p_threads[3]
    pthread_attr_t attr; //returns the handle, attr
    pthread_attr_init(&attr); //initialize the thread attribute
    pthread_mutex_init(&task_queue_lock, NULL); //initialize the mutex thread
    int key[2] = {1,2}; //sets key[2] to 1,2

    pthread_create(&p_threads[0], &attr, producer, NULL); //creates a new thread with p_thread[0]
    pthread_create(&p_threads[1], &attr, consumer, (void*) &key[0]); //creates a new thread with p_threads[1] and key[0]
    pthread_create(&p_threads[2], &attr, consumer, (void*) &key[1]); //creates a new thread with p_threads[2] and key[1]

    pthread_join(p_threads[0], NULL); //suspends the execution of p_thread until thread terminates
    pthread_join(p_threads[1], NULL);  //suspends the execution of p_thread until thread terminates
    pthread_join(p_threads[2], NULL); //suspends the execution of p_thread until thread terminates
    return 0;

}
