//
//  main.m
//  TempoReal_02
//
//  Created by Edivando Alves on 6/12/15.
//  Copyright (c) 2015 J7ss. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>  /* Semaphore */
#include <math.h>

char a,b,c;

float coefA, coefB, coefC;
float x1, x2;
sem_t mutexA;
sem_t mutexB;
sem_t mutexC;


void *print_message_function( void *ptr );

void *doA(void *threadno){
    while(a==0){}
    printf("Thread A\n");
    printf("Insira o valor do coenficiente A: ");
    scanf("%f", &coefA);
    printf("Insira o valor do coenficiente B: ");
    scanf("%f", &coefB);
    printf("Insira o valor do coenficiente C: ");
    scanf("%f", &coefC);

    a=0;
    b=1;
    return 0;
}

void *doB(void *threadno){
    while(b==0){}
    printf("Thread B\n");
    
    float delta = ((coefB*coefB) -  4 * coefA * coefC);
    
    x1 = (-coefB + sqrt(delta)) / 2*coefA;
    x2 = (-coefB - sqrt(delta)) / 2*coefA;
    
    if( delta > 0 ){
        printf("Equacao possui duas raizes distintas");
    }else if(delta == 0){
        printf("Equacao possui duas raizes iguais");
    }else{
        printf("Raizes complexas");
    }
    
    b=0;
    c=1;
    return 0;
}

void *doC(void *threadno){
    while(c==0){}
    printf("\nThread C\n");
    printf("A = %.2f, B = %.2f, C = %.2f\n", coefA, coefB, coefC);
    
    printf("X1 = %.2f, X2 = %.2f", x1, x2);
    
    c=0;
    a=1;
    return 0;
}

int main() {
    pthread_t thread1, thread2, thread3;
    b=c=0;
    a=1;
    
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    int  iret1, iret2,iret3;
    sem_init(&mutexA, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    sem_init(&mutexB, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    sem_init(&mutexC, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    
    /* Create independent threads each of which will execute function */
    
    iret1 = pthread_create( &thread1, NULL, doA, (void*) message1);
    if(iret1) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }
    
    iret2 = pthread_create( &thread2, NULL, doB, (void*) message2);
    if(iret2) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }
    
    iret3 = pthread_create( &thread3, NULL, doC, (void*) message2);
    if(iret3) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }

    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    
    exit(EXIT_SUCCESS);
}



