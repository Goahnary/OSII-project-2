#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

struct msg {
    long mtype;
    int mdata[2];
};

//the process for client A
void *ClientA(void* params){
    int sharedKey = 70;
    int id = 0;

    //open the mailboxes
    int msqidA = msgget(16777217, 0600 | IPC_CREAT);
    int msqidKDC = msgget(16777216, 0600 | IPC_CREAT);
    int msqidB = msgget(16777218, 0600 | IPC_CREAT);
    struct msg msgp;

    //set the message data (type, A id, B id)
    msgp.mtype = 1;
    msgp.mdata[0] = 0;
    msgp.mdata[1] = 3;

    //send ids to KDC and wait for reply
    msgsnd(msqidKDC, &msgp, sizeof(msgp)-sizeof(long), 0);
    printf("A requests session key.\n");
    msgrcv(msqidA, &msgp, sizeof(msgp)-sizeof(long), 1, 0);
    printf("A receives session key.\n");

    int sessionKey = msgp.mdata[0] ^ sharedKey;

    msgp.mdata[0] = id;

    msgsnd(msqidB, &msgp, sizeof(msgp)-sizeof(long), 0);
    msgrcv(msqidA, &msgp, sizeof(msgp)-sizeof(long), 1, 0);

    if(msgp.mdata[0] == 0 && msgp.mdata[1] == 0){
        printf("A receives confirmation.\n");
    }

    pthread_exit(0);
}

//the process for client B
void *ClientB(void* params){
    int sharedKey = 60;

    int msqidB = msgget(16777218, 0600 | IPC_CREAT);
    int msqidA = msgget(16777217, 0600 | IPC_CREAT);

    struct msg msgp;    
    msgrcv(msqidB, &msgp, sizeof(msgp) - sizeof(long), 1, 0);
    printf("B receives session key.\n");

    int sessionKey = msgp.mdata[1] ^ sharedKey;

    msgp.mdata[0] = 0;
    msgp.mdata[1] = 0;

    msgsnd(msqidA, &msgp, sizeof(msgp)-sizeof(long), 0);
    printf("B confirms receipt.\n");

    pthread_exit(0);
}

//the process for the KDC
void *KDC(void* params){
    int sharedKeys[] = {70, 45, 122, 60, 401};

    //open the mailboxes
    int msqidKDC = msgget(16777216, 0600 | IPC_CREAT);
    int msqidA = msgget(16777217, 0600 | IPC_CREAT);
    struct msg msgp;

    //wait for message from the first process
    msgrcv(msqidKDC, &msgp, sizeof(msgp)-sizeof(long), 1, 0);
    printf("KDC receives request for session key.\n");

    //generate a random session key
    int sessionKey = rand();
    
    //get the shared keys for the two processes
    int shKeyA = sharedKeys[msgp.mdata[0]];
    int shKeyB = sharedKeys[msgp.mdata[1]];

    //encrypt the session key with the shared keys
    int eKeyA = sessionKey ^ shKeyA;
    int eKeyB = sessionKey ^ shKeyB;

    //update the message
    msgp.mdata[0] = eKeyA;
    msgp.mdata[1] = eKeyB;

    //send the keys to A
    msgsnd(msqidA, &msgp, sizeof(msgp)-sizeof(long), 0);

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
    	pthread_create(&tidKDC, &attr, KDC, NULL);

        /*create the client threads */
    	pthread_create(&tidA, &attr, ClientA, NULL);
        pthread_create(&tidB, &attr, ClientB, NULL);

        // pthread_join(tidKDC, NULL);
        pthread_join(tidA, NULL);
        pthread_join(tidB, NULL);
}
