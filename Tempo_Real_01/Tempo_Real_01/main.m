//
//  main.m
//  Tempo_Real_01
//
//  Created by Edivando Alves on 6/12/15.
//  Copyright (c) 2015 J7ss. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>  /* Semaphore */

char listchar[31] = {0};
char a,b,c;
int num=0;
sem_t mutexA;
sem_t mutexB;
sem_t mutexC;

void *print_message_function( void *ptr );

void *doA(void *threadno){
    while(num<=27){
        while(a==0){}
        listchar[num++]='a';
        a=0;
        b=1;
    }
    return 0;
}

void *doB(void *threadno){
    while(num<=28){
        while(b==0){}
        listchar[num++]='b';
        b=0;
        c=1;
    }
    return 0;
}

void *doC(void *threadno){
    while(num<=29){
        while(c==0){}
        listchar[num++]='c';
        c=0;
        a=1;
    }
    printf("%s",listchar);
    
    return 0;
}

main()
{
    pthread_t thread1, thread2, thread3;
    b=c=0;
    a=1;
    listchar[30]= '\0';
    
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    int  iret1, iret2,iret3;
    sem_init(&mutexA, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    sem_init(&mutexB, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    sem_init(&mutexC, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    
    /* Create independent threads each of which will execute function */
    
    iret1 = pthread_create( &thread1, NULL, doA, (void*) message1);
    if(iret1)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }
    
    iret2 = pthread_create( &thread2, NULL, doB, (void*) message2);
    if(iret2)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }
    
    iret3 = pthread_create( &thread3, NULL, doC, (void*) message2);
    if(iret3)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }
    
    
    //    printf("pthread_create() for thread 1 returns: %d\n",iret1);
    //    printf("pthread_create() for thread 2 returns: %d\n",iret2);
    //    printf("pthread_create() for thread 2 returns: %d\n",iret3);
    
    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    
    exit(EXIT_SUCCESS);
}


