//
//  main.c
//  TempoReal_03
//
//  Created by Edivando Alves on 6/26/15.
//  Copyright (c) 2015 J7ss. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>  /* Semaphore */
#include <math.h>

int ids[6] = {-1};
int flag = 0;

void funcao_ordena(){
    
    int i, j, aux;
    
    for( i=0; i<6; i++ ){
        for( j=i+1; j<6; j++ ){
            if( ids[i] > ids[j] ){
                aux = ids[i];
                ids[i] = ids[j];
                ids[j] = aux;
            }
        }
    }
    printf("\nOrdenado com sucesso!\n");
}

//int compara(){
//    for (int i = 0; i < 6; i++) {
//        for (int j = i+1; j < 5; j++) {
//            if (ids[i] == ids[j]){
//                printf("id: %d\n", ids[i]);
//                return 1;
//            }
//        }
//    }
//    return 0;
//}


void *doA(void *threadno){
    ids[0] = (int)pthread_self();
    while(1){}
}

void *doB(void *threadno){
    ids[1] = (int)pthread_self();
    while(1){}
}

void *doC(void *threadno){
    ids[2] = (int)pthread_self();
    while(1){}
}

void *doD(void *threadno){
    ids[3] = (int)pthread_self();
    while(1){}
}

void *doE(void *threadno){
    ids[4] = (int)pthread_self();
    while(1){}
}


void *doF(void *threadno){
    ids[5] = (int)pthread_self();
   
    while(ids[0] <= 0 || ids[1] <= 0 || ids[2] <= 0 || ids[3] <= 0 || ids[4] <= 0 || ids[5] <= 0 ){ }
    
    printf("ANTES \n");
    
    for (int i = 0; i<6; i++) {
        printf("ids[%d]: %d\n",i,ids[i]);
    }
    
    funcao_ordena();
    
     printf("DEPOIS \n");
    
    for (int i = 0; i<6; i++) {
        printf("ids: %d\n",ids[i]);
    }
    flag = 1;
    
    return 0;
}



int main() {
    pthread_t thread1, thread2, thread3, thread4, thread5, thread6;

    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    const char *message3 = "Thread 3";
    const char *message4 = "Thread 4";
    const char *message5 = "Thread 5";
    const char *message6 = "Thread 6";
    
   int id1, id2, id3, id4, id5, id6;
    
    id1 = pthread_create( &thread1, NULL, doA, (void*) message1);
    if(id1) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",id1);
        exit(EXIT_FAILURE);
    }
    
    id2 = pthread_create( &thread2, NULL, doB, (void*) message2);
    if(id2) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",id2);
        exit(EXIT_FAILURE);
    }
    
    id3 = pthread_create( &thread3, NULL, doC, (void*) message3);
    if(id3) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",id3);
        exit(EXIT_FAILURE);
    }
    
    id4 = pthread_create( &thread4, NULL, doD, (void*) message4);
    if(id4) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",id4);
        exit(EXIT_FAILURE);
    }
    
    id5 = pthread_create( &thread5, NULL, doE, (void*) message5);
    if(id5) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",id5);
        exit(EXIT_FAILURE);
    }
    
    id6 = pthread_create( &thread6, NULL, doF, (void*) message6);
    if(id6) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",id6);
        exit(EXIT_FAILURE);
    }
    
   while (flag==0) {}
}




