//
//  main.c
//  Tempo_Real_04
//
//  Created by Edivando Alves on 7/10/15.
//  Copyright (c) 2015 J7ss. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int chan[6] = {-1};

void send_mes_sync(int* buf, int can){
    chan[can] = *buf;
    while (chan[can] != -1) { }
    return;
}

void receive_mes(int *buf, int can){
    while (chan[can] == -1) {}
    *buf = chan[can];
    chan[can] = -1;
    return;
}

void *version_00(void *threadno){
    int voto = 8;
    int status;
//    printf("Versio_00");
    do{
        send_mes_sync(&voto, 0);
        receive_mes(&status, 3);
        printf("\nVersion 00 --> %d\n", status);
    }while (status == 1);
    printf("Versio_00 Finalizada");
    return 0;
}

void *version_01(void *threadno){
    int voto = 8;
    int status;
    do{
        send_mes_sync(&voto, 1);
        receive_mes(&status, 4);
        printf("\nVersion 01 --> %d\n", status);
    }while (status == 1);
    printf("Versio_01 Finalizada");
    return 0;
}


void *version_02(void *threadno){
    int voto = 10;
    int status;
    do{
        printf("aa");
        send_mes_sync(&voto, 2);
        receive_mes(&status, 5);
        printf("\nVersion 02 --> %d\n", status);
    }while (status == 1);
    printf("Versio_02 Finalizada");
    while (1) {
        
    }
    return 0;
}

void *t_driver(void *threadno){
    pthread_t thread1, thread2, thread3;
    
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    const char *message3 = "Thread 3";
    int  iret0, iret1,iret2;

    /* Create independent threads each of which will execute function */

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
    
    int voto[3] = {0};
    int certo = 1, errado = 0;
    receive_mes(&voto[0], 0);
    receive_mes(&voto[1], 1);
    receive_mes(&voto[2], 2);
    
    if(voto[0] == voto[1]){
        send_mes_sync(&certo, 3);
        send_mes_sync(&certo, 4);
        send_mes_sync(&errado, 5);
        printf("\nResultado Correto: %d \n", voto[0]);
    }else if(voto[1] == voto[2]){
        send_mes_sync(&errado, 3);
        send_mes_sync(&certo, 4);
        send_mes_sync(&certo, 5);
        printf("\nResultado Correto: %d \n", voto[1]);
    }else{
        send_mes_sync(&certo, 3);
        send_mes_sync(&errado, 4);
        send_mes_sync(&certo, 5);
        printf("\nResultado Correto: %d \n", voto[2]);
    }
    while (1) {}
    return 0;
}


int main(int argc, const char * argv[]) {
    pthread_t thread_drive;
    int iret_driver;
    const char *driver_msg = "Thread Driver";
    
    iret_driver = pthread_create( &thread_drive, NULL, t_driver, (void*) driver_msg);
    if(iret_driver) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret_driver);
        exit(EXIT_FAILURE);
    }
    
    pthread_join(thread_drive, NULL);
    while(1){}
//    exit(EXIT_SUCCESS);
}
