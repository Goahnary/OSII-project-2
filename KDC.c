#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

int sharedKeys[] = {70, 45, 122, 60, 401};
int running = 1;

struct msg {
    long int mtype;
    int mdata[2];
};

//the process for client A
void *ClientA(void* params){
    printf("wefwefwf2");
    //open the mailbox
    int msqidA = msgget(16777217, 0600 | IPC_CREAT);
    int msqidKDC = msgget(16777216, 0600 | IPC_CREAT);
    int msqidB = msgget(16777218, 0600 | IPC_CREAT);
    struct msg msgp;

    msgp.mtype = 0;
    msgp.mdata[0] = 0;
    msgp.mdata[1] = 3;

    msgsnd(msqidKDC, &msgp, sizeof(msgp)-sizeof(long), 0);

    msgrcv(msqidB, &msgp, sizeof(msgp)-sizeof(long), 0, IPC_NOWAIT);

    pthread_exit(0);
}

//the process for client B
void *ClientB(void* params){
    printf("test");
    pthread_exit(0);
}

//the process for the KDC
void *KDC(void* params){
    printf("wefwefwf3");
    int msqid = msgget(16777216, 0600 | IPC_CREAT);
    int msqidA = msgget(16777217, 0600 | IPC_CREAT);
    struct msg msgp;

    printf("wefwefwf4");

    // while(running == 1){
    //     //wait for message from the first process
    //     msgrcv(msqid, &msgp, sizeof(msgp)-sizeof(long), 0, 0);

    //     //generate a random session key
    //     int sessionKey = rand();
        
    //     //get the shared keys fo the two processes
    //     int shKeyA = sharedKeys[msgp.mdata[0]];
    //     int shKeyB = sharedKeys[msgp.mdata[1]];

    //     printf("%d", shKeyA);
    //     printf("%d", shKeyB);

    //     //encrypt the session keys with the shared keys
    //     int eKeyA = sessionKey ^ shKeyA;
    //     int eKeyB = sessionKey ^ shKeyB;

    //     msgsnd(msqid, &msgp, sizeof(msgp)-sizeof(long), 0);
    // }

    pthread_exit(0);
}

int main(int argc, char *argv[]){
        pthread_attr_t attr; /* set of thread attributes */
        pthread_t tidA; /* the thread identifier */
        pthread_t tidB; /* the thread identifier */
        pthread_t tidKDC; /* the thread identifier */

        /* get default attribute */
        pthread_attr_init(&attr);

        /*create the intitial thread */
    	// pthread_create(&tidKDC, &attr, KDC, NULL);

        /*create the client threads */
    	pthread_create(&tidA, &attr, ClientA, NULL);
        pthread_create(&tidB, &attr, ClientB, NULL);

        // pthread_join(tidKDC, NULL);
        pthread_join(tidA, NULL);
        pthread_join(tidB, NULL);
}
