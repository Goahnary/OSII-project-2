#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int KA = 70;
int KB = 221;

void *ClientA(){

}

void *ClientB(){
    
}

void *KDC(){

}

int main(int argc, char *argv[]){
        pthread_attr_t attr; /* set of thread attributes */
        pthread_t tidA; /* the thread identifier */
        pthread_t tidB; /* the thread identifier */
        pthread_t tidKDC; /* the thread identifier */

        /* get default attribute */
        pthread_attr_init(&attr);

        /*create the intitial thread */
    	pthread_create(&tidKDC, &attr, KDC, NULL);

        /*create the client threads */
    	pthread_create(&tidA, &attr, ClientA, NULL);
        pthread_create(&tidB, &attr, ClientB, NULL);
}
