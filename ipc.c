#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "ipc.h"
#include <sys/msg.h>
#include <unistd.h>

static int mem_id, *mem_ptr;
static int sem_id;
static int msgqid;

int *max_game_id, *n;

int *tablero;

/* Borrar IPC en atexit (solo en el sever) */
void clearmem() {
    shmdt(mem_ptr);
    shmctl(mem_id, IPC_RMID, NULL);
}

void clearsem() {
    semctl(sem_id, 0, IPC_RMID);
}

void clearmsgq() {
    msgctl(msgqid, IPC_RMID, NULL);
}

/* Inicializar IPC (notar cambios en server) */
void *initmem() {
    key_t key;
    int memsize;
    /* En memoria compartida quedaría:
     *
     * numbers.serial | numbers.random | max_game_id | process_count (for synch) | tablero
     */

    key = ftok(".", 1);
    /* 100 jugadores máximo meh */
    memsize = sizeof(int)*104;

    mem_id = shmget(key, memsize, IPC_CREAT | 0666);
    mem_ptr = shmat(mem_id, NULL, 0);
    max_game_id = mem_ptr + sizeof(int)*2;
    n = mem_ptr + sizeof(int)*3;
    tablero = mem_ptr + sizeof(int)*4;

    #ifdef SERVER
    atexit(clearmem);
    memset(mem_ptr, 0, memsize);
    // el server siempre tiene 0 y cuenta como participante extra
    *max_game_id = 1;
    #endif

    return mem_ptr;
}


void initsem() {
    key_t key;

    key = ftok(".", 1);

    sem_id = semget(key, SEMS, IPC_CREAT | 0666);

    #ifdef SERVER
    semctl(sem_id, QUEUE1, SETVAL, 0);
    semctl(sem_id, QUEUE2, SETVAL, 0);
    semctl(sem_id, MUTEX, SETVAL, 1);

    atexit(clearsem);
    #endif

}

void initmsgq() {
    key_t key;

    key = ftok(".", 1);

    msgqid = msgget(key, IPC_CREAT | 0666);

    #ifdef SERVER
    atexit(clearmsgq);
    #endif
}

/* Obtener un ID dentro del juego (distinto al PID del so) */
int get_game_id() {
    int game_id;

    Wait(MUTEX);
    game_id = *max_game_id;
    *max_game_id += 1;
    Signal(MUTEX);

    return game_id;
}

void manotazo(int game_id) {
    msg mano;
    int r;

    mano.mtype = 1;
    mano.game_id = game_id;
    r = msgsnd(msgqid, &mano, sizeof(int), 0);
}

void imprimeTablero() {
    int i;

    for (i=1; i<*max_game_id; i++) {
        printf("Jugador:[%d], puntos:[%d]\n", i, tablero[i]);
    }
}

void check_winner() {
    msg m;
    int r;

    sleep(1);
    r = msgrcv(msgqid, &m, sizeof(int), 1, IPC_NOWAIT);
    if (errno == ENOMSG) {
        printf("Nadie obtuvo punto\n");
        return;
    }

    tablero[m.game_id]++;
    printf("Uno más para [%d]\n", m.game_id);

    while(1) {
        msgrcv(msgqid, &m, sizeof(int), 1, IPC_NOWAIT);
        if (errno == ENOMSG) break;
    }
}

void WaitSome(int n, int p) {
    struct sembuf oper[1];

    oper[0].sem_num = n;
    oper[0].sem_op = -p;
    oper[0].sem_flg = 0;

    semop(sem_id, oper, 1);
}

void Wait(int n) {
    WaitSome(n, 1);
}

void SignalSome(int n, int p) {
    struct sembuf oper[1];

    oper[0].sem_num = n;
    oper[0].sem_op = p;
    oper[0].sem_flg = 0;

    semop(sem_id, oper, 1);
}

void Signal(int n) {
    SignalSome(n, 1);
}

void barrierWaitPhase1() {
    Wait(MUTEX);
        *n += 1;
        //printf("En barrier1 %d, n=[%d]\n", getpid(), *n);
        // Si ya están todos 
        if (*n == *max_game_id) {
            // dejarlos pasar
            //printf("Liberando1 %d\n", getpid());
            //sleep(1);
            SignalSome(QUEUE1, *max_game_id);
        }
    Signal(MUTEX);
    Wait(QUEUE1);

}

void barrierWaitPhase2() {
    Wait(MUTEX);
        *n -= 1;
        //printf("En barrier2 %d, n=[%d]\n", getpid(), *n);
        if (*n == 0) {
            //printf("Liberando2 %d\n", getpid());
            //sleep(1);
            SignalSome(QUEUE2, *max_game_id);
        }
    Signal(MUTEX);
    Wait(QUEUE2);
}
