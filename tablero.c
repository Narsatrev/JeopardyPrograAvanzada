#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                  /*  for sleep()  */
#include <curses.h>
#include <string.h>
#include "preguntas.h"
#include "numeros.h"

int max_y = 0, max_x = 0;

void draw_categories(int y, int x) {
    int i;
    for(i=0; i<5; i++) {
//        mvaddstr(y+i, x, categ[i]);
    }
}

void draw_number(int n, int y, int x){
    int i;
    for(i=0; i<5; i++){
        mvaddstr(y+i, x, todos[n][i]);
    }
}

void draw_question(int cat, int p, int y, int x){
    mvaddstr(y, x, cat1[p]);
}

void draw_cat(int y, int x, int i){
    mvaddstr(y,x, categ[i]);
}

void draw_board(){
    int i, k, contador = 1, contador1 = 1;
    for(k = 0; k < 5; k++){
        for(i = 0; i < max_x; i++){
             mvaddstr(contador * (max_y/6),i,".");
        }
        contador++;
    }
    for(k = 0; k < 4; k++){
        for(i = 0; i < max_y; i++){
             mvaddstr(i,contador1 * (max_x/5),"|");
        }
        contador1++;
    }
    contador = 0;
    for(k = 0; k < 5; k++){
        draw_cat(3,(contador * max_x/5)+3,k);
        contador++;
    }
    for(k = 0; k < 5; k++){
    for(i=0; i<5; i++) {
        draw_number(i,((max_y/6)*(i+1))+3,((max_x/5)*(k))+5);
    }
    }
}

int main(void){
    WINDOW * mainwin;
    
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    noecho();
    curs_set(FALSE);
    start_color();

    nodelay(mainwin, TRUE);

    getmaxyx(stdscr, max_y, max_x);
    draw_board();
    
    refresh();
//    clear();
    sleep(10);
    delwin(mainwin);
    endwin();
    refresh();
    return EXIT_SUCCESS;
}
