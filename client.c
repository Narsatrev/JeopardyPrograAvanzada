#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                  /*  for sleep()  */
#include <curses.h>
#include <string.h>
#include "numbers.h"
#include "ipc.h"
#include "preguntas.h"

#define WITH_CURSES

int max_y = 0, max_x = 0;
int game_id;

extern int *max_game_id, *n;

void draw_number(int n, int y, int x) {
    int i;

    for(i=0; i<11; i++) {
        mvaddstr(y+i, x, todos[n][i]);
    }
}


int main(void) {
    WINDOW * mainwin;
  //  numbers * nu;
    preguntas * pr;
    pr = initmem();

//    nu = initmem();
    initsem();
    initmsgq();

//    Wait(PLAYER);
        game_id = get_game_id();
	printf("llego\n");
        printf("game_id=[%d]\n", game_id);
  //  Signal(PLAYER);

    /*  Initialize ncurses  */
#ifdef WITH_CURSES
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    noecho();
    curs_set(FALSE);

    start_color();                    /*  Initialize colours  */
    init_pair(1, COLOR_WHITE,   COLOR_BLUE);
    init_pair(2, COLOR_WHITE,   COLOR_MAGENTA);

    //halfdelay(15);
    nodelay(mainwin, TRUE);
#endif

	while(1) {
#ifdef WITH_CURSES
        getmaxyx(stdscr, max_y, max_x);
        clear();
#endif

        barrierWaitPhase1();

#ifdef WITH_CURSES
        color_set(1, NULL);
//        draw_number(nu->serial, max_y/2-6+rand()%5-5, max_x/2-30+rand()%5-5);
	mvaddstr(max_y/2-6+rand()%5-5, max_x/2-30+rand()%5-5, categorias[pr->serial][1]);
        color_set(2, NULL);
        //draw_number(nu->random, max_y/2-6+rand()%5-5, max_x/2+10+rand()%5-5);
        // refresh();
#else
        printf("Status: [%d]-[%d] max_g_id:[%d], n:[%d]\n", pr->serial, pr->random,
                        *max_game_id, *n);
#endif

#ifdef WITH_CURSES
        if (pr->random == pr->serial) {
            int v;
            halfdelay(30);
            v = getch(); 
            clear();
            color_set(0, NULL);
            if (v == ERR) {
                mvaddstr(max_y/2, max_x/2, "Demasiado tarde :(");
            } else {
                if (v-'0' == pr->serial) {
                    manotazo(game_id);
                    mvaddstr(max_y/2, max_x/2, "Enviando, ojalá seas el primero!");
                } else {
                    mvaddstr(max_y/2, max_x/2, "Error en el número :X");
                }
            }
        }
        flushinp();
        refresh();
        nodelay(mainwin, TRUE);
#endif

        barrierWaitPhase2();
	}

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}
