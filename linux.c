#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t wrt;
pthread_mutex_t mutex;
int readn = 1;
int actual = 0;

void *entry(void *eno)
{   
    sem_wait(&wrt);
    readn = readn*2;
    printf("Candidate %d inputs count to %d\n",(*((int *)eno)),readn);
    sem_post(&wrt);

}
void *data(void *dno)
{   
    
    pthread_mutex_lock(&mutex);
    actual++;
    if(actual == 1) {
        sem_wait(&wrt); 
    }
    pthread_mutex_unlock(&mutex);
        printf("Admin %d: read count as %d\n",*((int *)dno),readn);

       pthread_mutex_lock(&mutex);
    actual--;
    if(actual == 0) {
        sem_post(&wrt); 
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{   

    pthread_t dataread[10],datawrite[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int assigned[10] = {1,2,3,4,5,6,7,8,9,10}; 
    for(int i = 0; i < 10; i++) {
        pthread_create(&dataread[i], NULL, (void *)data, (void *)&assigned[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&datawrite[i], NULL, (void *)entry, (void *)&assigned[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(dataread[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(datawrite[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
    
}


