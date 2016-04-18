#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                  /*  for sleep()  */
#include <curses.h>
#include <string.h>
#include "preguntas.h"
#include "numbers.h"

int max_y = 0, max_x = 0;

void draw_categories(int y, int x) {
    int i;
    for(i=0; i<5; i++) {
        mvaddstr(y+i, x, categ[i]);
    }
}

void draw_number(int n, int y, int x){
    int i;
    for(i=0; i<7; i++){
	mvaddstr(y+i, x, todos[n][i]);    
    }
}

void draw_question(int cat, int p, int y, int x){
    switch(cat){
	case 1:
	mvaddstr(y, x, cat1[p]);
	break;
	case 2:
        mvaddstr(y, x, cat2[p]);
        break;
	case 3:
	mvaddstr(y, x, cat3[p]);
        break;
	case 4:
	mvaddstr(y, x, cat4[p]);
        break;
	case 5:
	mvaddstr(y, x, cat5[p]);
        break;
    }
}

int main(void) {
    WINDOW * mainwin;
    int i, seleccionCat, seleccionP;
    srand(getpid());

    /*  Initialize ncurses  */
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    noecho();
    curs_set(FALSE);
    start_color();

    nodelay(mainwin, TRUE);

    getmaxyx(stdscr, max_y, max_x);
    clear();
    draw_categories(max_y/2, max_x/2);
    refresh();
	
    scanf("%d", &seleccionCat);
//    if(seleccionCat == 1){
	int k;
	clear();
        for(k = 0; k < 5; k++){
		draw_number(k, (max_y/2-20)+(k*8), max_x/2-15);  
        }
	refresh();
  //  }
    scanf("%d", &seleccionP);
//    if(seleccionP == 1){
        clear();
	printf("seleccion de pregunta: %d\n", seleccionP);
        draw_question(seleccionCat, seleccionP-1, max_y/2, max_x/2);
        refresh();
  //  }
    sleep(10);

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}

