//
//  main.c
//  Tempo_Real_05
//
//  Created by Edivando Alves on 7/31/15.
//  Copyright (c) 2015 J7ss. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int channel[6] = {-1, -1, -1, -1, -1, -1};

int flagB = 0;

void send_mes_sync(int* buf, int can){
    channel[can] = *buf;
    while (channel[can] != -1) { }
    return;
}

void receive_mes(int *buf, int can){
    while (channel[can] == -1) {}
    *buf = channel[can];
    channel[can] = -1;
    return;
}

int randNumber(){
    return (rand() % 100) + 1;
}

char *decimal_to_binary(int n) {
    int c, d, count;
    char *pointer;
    
    count = 0;
    pointer = (char*)malloc(6+1);
    
    if ( pointer == NULL )
        exit(EXIT_FAILURE);
    
    for ( c = 5 ; c >= 0 ; c-- ) {
        d = n >> c;
        
        if ( d & 1 )
            *(pointer+count) = 1 + '0';
        else
            *(pointer+count) = 0 + '0';
        count++;
    }
    *(pointer+count) = '\0';
    return  pointer;
}

void *proccessA(void *threadno){
    printf("Processo A");
    int gambler[5][6] = {0};
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            gambler[i][j] = randNumber();
        }
    }
    for (int i = 0; i<100; i++) {
        printf("Entrou process A");
        int sortedValue;
        int gamblersWon = 0;
        receive_mes(&sortedValue, 1);
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                if( gambler[i][j] == sortedValue ){
                    gamblersWon = gamblersWon + pow(2, i);
                }
            }
        }
        flagB = 1;          //Iniciar processo B
        send_mes_sync(&gamblersWon, 2);
    }
    return 0;
}

void *proccessB(void *threadno){
    printf("Processo B");
    int sorted;
    for (int i =0; i<100; i++) {
        while (flagB == 1) {}
        sorted = randNumber();
        send_mes_sync(&sorted, 0);
        flagB = 0;
    }
    return 0;
}


void *proccessC(void *threadno){
    printf("Processo C");
    int sortedValue;
    int gamblerWin;
    int perctGambler[6] = {0};
    while (perctGambler[0] < 6 && perctGambler[1] < 6 && perctGambler[2] < 6 && perctGambler[3] < 6 && perctGambler[4] < 6 && perctGambler[5] < 6) {
        receive_mes(&sortedValue, 0);         // Receive number sorted by proccess B
        send_mes_sync(&sortedValue, 1);       // Send number sorted to process A
        
        receive_mes(&gamblerWin, 2);          // Receive gamblers Won from proccess A
        
//        Count gambler right
        char *binary;
        binary = decimal_to_binary(gamblerWin);
        for (int i = 5; i>=0; i--) {
            printf("%i", binary[i]);
            
            if (binary[i] == '1'){
                perctGambler[i] += 1;
                printf("Jogador %i = 1", i);
            }
        }
        free(binary);
        flagB = 1;
    }
    return 0;
}


int main(int argc, const char * argv[]) {
    printf("Main");
    srand((unsigned int)time(NULL));
    pthread_t thread1, thread2, thread3;
    
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    const char *message3 = "Thread 3";
    
    /* Create independent threads each of which will execute function */
    
    int proA = pthread_create( &thread1, NULL, proccessA, (void*) message1 );
    if(proA) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",proA);
        exit(EXIT_FAILURE);
    }
    
    int proB = pthread_create( &thread2, NULL, proccessB, (void*) message2 );
    if(proB) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",proB);
        exit(EXIT_FAILURE);
    }
    
    int proC = pthread_create( &thread3, NULL, proccessC, (void*) message3 );
    if(proC) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",proC);
        exit(EXIT_FAILURE);
    }
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    
//    while(1){}
        exit(EXIT_SUCCESS);
}
