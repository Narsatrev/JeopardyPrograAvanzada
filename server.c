#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "ipc.h"

extern int *max_game_id, *n;

void salir(int x) {
    exit(0);
}

int main() {
    numbers *nu;
    srand(getpid());

    nu = initmem();
    initsem();
    initmsgq();
    signal(SIGINT, salir);

    while (1) {
        for(nu->serial=0; nu->serial<10; nu->serial++) {
            printf("Admitiendo nuevos jugadores...\n");
            Signal(PLAYER);
            Wait(PLAYER);
            printf("Tenemos %d jugadores...\n", *max_game_id-1);
            imprimeTablero();
            nu->random = rand()%10;
            sleep(1);
            printf("Status: [%d]-[%d] max_g_id:[%d], n:[%d]\n", nu->serial, nu->random, *max_game_id, *n);
            barrierWaitPhase1();
            if (nu->serial == nu->random) {
                if(!fork()) {
                    check_winner();
                    _exit(0);
                }
            }
            barrierWaitPhase2();
        }
    }
}
