#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

int sharedKeys[] = {70, 45, 122, 60, 401};
int running = 1;

void *ClientA(){

}

void *ClientB(){
    
}

void *KDC(){
    int msqid = msgget(1234, 0600 | IPC_CREAT);
    int msgp;

    while(running == 1){
        //wait for message from the first process
        msgrcv(msqid, &msgp, sizeof(msgp) - sizeof(long), 0, IPC_NOWAIT);

        //generate a random session key
        int sessionKey = rand();
        
        //get the shared keys fo the two processes (fix this later)
        int shKeyA = sharedKeys[msgp];
        int shKeyB = sharedKeys[msgp];

        //encrypt the session keys with the shared keys
        int eKeyA = sessionKey ^ shKeyA;
        int eKeyB = sessionKey ^ shKeyB;

        msgsnd(msqid, &msgp, sizeof(msgp)-sizeof(long), IPC_NOWAIT);
    }
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
