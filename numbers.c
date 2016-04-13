#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                  /*  for sleep()  */
#include <curses.h>
#include <string.h>
#include "preguntas.h"
#include "numbers.h"

int max_y = 0, max_x = 0;

void draw_number(int n, int y, int x) {
    int i;

    for(i=0; i<7; i++) {
        mvaddstr(y+i, x, todos[n][i]);
    }
}


int main(void) {
    WINDOW * mainwin;
    int i, conteo, aleatorio;

    srand(getpid());

    /*  Initialize ncurses  */
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    noecho();
    curs_set(FALSE);

    start_color();                    /*  Initialize colours  */
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_MAGENTA);

    //halfdelay(15);
    nodelay(mainwin, TRUE);

	while(1) {

		for (i=0; i<5; i++) {
            getmaxyx(stdscr, max_y, max_x);
            clear();
            aleatorio=rand()%5;
            color_set(1, NULL);
            draw_number(i, max_y/2-6+rand()%5-5, max_x/2-30+rand()%5-5);
            color_set(2, NULL);
            draw_number(aleatorio, max_y/2-6+rand()%5-5, max_x/2+10+rand()%5-5);
            refresh();
            if (aleatorio == i) {
                int v;
                halfdelay(20);
                v = getch();
                printf("V: %c I: %d",v,i);
                // clear();
                color_set(0, NULL);
                if (v == ERR) {
                    mvaddstr(max_y/2, max_x/2, "No presiono en tiempo");
                } else {
                    if (v-'0' == i) {
                        mvaddstr(max_y/2, max_x/2, "Le atino al numero");
                    } else {
                        mvaddstr(max_y/2, max_x/2, "No le atino al numero");
                    }
                }
                refresh();
                sleep(2);
                flushinp();
                nodelay(mainwin, TRUE);
            } else {
                //usleep(500000);
                sleep(1);
                flushinp();
            }

            //usleep(500000);
		}

	} 

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}
