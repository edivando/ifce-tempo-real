//
//  main.c
//  Tempo_Real_04
//
//  Created by Edivando Alves on 7/10/15.
//  Copyright (c) 2015 J7ss. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int chan[3] = {-1, -1, -1};

void delay_s(int t){
    time_t start,end;
    start = time(NULL);
    do{
        end = time(NULL);
    }while (difftime(end, start) <= t);
    return;
}

int alt_wait(int c, int can[]){
    int i=0;
    int j=0;
    while (1) {
        for (i = 0; i < c; i++) {
            if (chan[i] != -1) {
                return i;
            }
        }
        j++;
    }
    return 0;
}

int send_mes_sync_t(int *buf, int can, int timeout){
    time_t start,end;
    start = time(NULL);
    do{
        end = time(NULL);
        printf("difftime: %lf",difftime(end, start));
    }while (difftime(end, start) <= timeout && chan[can]!= -1);
    if (chan[can] != -1) {
        chan[can] = *buf;
        return 0;
    }
    return 1;
}

int receive_t(int *buf, int can, int timeout){
    time_t start,end;
    start = time(NULL);
    do{
        end = time(NULL);
    }while (difftime(end, start)<= timeout && chan[can]!= -1);
    if (chan[can] != -1) {
        *buf = chan[can];
        chan[can] = -1;
        return 0;
    }
    return 1;
}

void *version_00(void *threadno){
    int buf = 10;
    int x=1;
    delay_s(5);
    do{
        x=send_mes_sync_t(&buf, 0, 5);
    }while(x==1);
    
    return 0;
}

void *version_01(void *threadno){
    int buf=20;
    int x = 1;
    int i;
    delay_s(7);
    
    for (i=0; i<2; i++) {
        x=send_mes_sync_t(&buf, 1, 5);
        if (x==0) {
            return 0;
        }
    }
    printf("falha no envio da mensagem");
    return 0;
}


void *version_02(void *threadno){
    int buf = 30;
    int x=1;
    delay_s(9);
    do{
        x=send_mes_sync_t(&buf, 2, 5);
    }while(x==1);
    return 0;
}

void *t_driver(void *threadno){
    // criando threads 1,2,3
    pthread_t thread1, thread2, thread3;
    
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    const char *message3 = "Thread 3";
    int  iret0, iret1,iret2;
    
    iret0 = pthread_create( &thread1, NULL, version_00, (void*) message1);
    if(iret0) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret0);
        exit(EXIT_FAILURE);
    }
    
    iret1 = pthread_create( &thread2, NULL, version_01, (void*) message2);
    if(iret1) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }
    
    iret2 = pthread_create( &thread3, NULL, version_02, (void*) message3);
    if(iret2) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }

    int vetor[3];
    int x;
    int i;
    for (i=0; i<=2; i++) {
        x=alt_wait(3, chan);
        receive_t(&vetor[i], x, 5);
    }
    for (i=0; i<=2; i++) {
        printf("valor recebido eh: %d",vetor[i]);
    }
    return 0;
}


int main(int argc, const char * argv[]) {
        // criando thread drive
    pthread_t thread_drive;
    const char *driver_msg = "Thread Driver";
    int iret_driver;
    
    
    iret_driver = pthread_create( &thread_drive, NULL, t_driver, (void*) driver_msg);
    if(iret_driver) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret_driver);
        exit(EXIT_FAILURE);
    }
    
    pthread_join(thread_drive, NULL);
   // while(1){}
    exit(EXIT_SUCCESS);
}
