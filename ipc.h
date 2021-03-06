#ifndef IPC_H
#define IPC_H

#define QUEUE1 0
#define QUEUE2 1
#define MUTEX 2
#define PLAYER 4 //JEOPARDY rules

#define SEMS 4

typedef struct {
    long mtype;
    int game_id;
    int puntaje;
} msg;

typedef struct {
    int serial, random;
} numbers;

typedef struct {
    int serial, random;
} preguntas;

void *initmem();
void initsem();
void initmsgq();

void WaitSome(int n, int p);
void Wait(int n);
void SignalSome(int n, int p);
void Signal(int n);
void barrierWaitPhase1();
void barrierWaitPhase2();

int get_game_id();
void manotazo(int id);
void check_winner();
void emptyQueue();
void imprimeTablero();

#endif
