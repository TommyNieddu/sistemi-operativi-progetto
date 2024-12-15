#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#define NOME 50
#define TEMPO_MAX 90 //sono i secondi, quindi un minuto e mezzo, si può modificare, in base a quanto vogliamo che duri ogni round

void menu();
void inizio(const char *username);

int main() {
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    noecho();
    cbreak();
    curs_set(0);
    menu();
    endwin();
    return 0;
}

void menu() {
    const char *opzioni[] = {"Inizia a giocare", "Esci"};
    int scelta = 0;
    int key;
    char username[NOME];
    int num_opzioni = sizeof(opzioni) / sizeof(opzioni[0]);


    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    while (1) {
        clear();

        //titolo
        attron(COLOR_PAIR(1));
        mvprintw(max_y / 2 - 3, (max_x - 33) / 2, "#################################");
        mvprintw(max_y / 2 - 2, (max_x - 33) / 2, "#       Frogger Resurrection    #");
        mvprintw(max_y / 2 - 1, (max_x - 33) / 2, "#################################");
        attroff(COLOR_PAIR(1));


        for (int i = 0; i < num_opzioni; i++) {
            if (i == scelta) {
                attron(A_REVERSE | COLOR_PAIR(2)); //per evidenziare la scelta dove ci posizioniamo con kle frecce direzionali
            }
            mvprintw(max_y / 2 + i + 1, (max_x - strlen(opzioni[i])) / 2, "%s", opzioni[i]);
            if (i == scelta) {
                attroff(A_REVERSE | COLOR_PAIR(2));
            }
        }

        key = getch();

        switch (key) {
            case KEY_UP:
                scelta = (scelta - 1 + num_opzioni) % num_opzioni;
                break;
            case KEY_DOWN:
                scelta = (scelta + 1) % num_opzioni;
                break;
            case 10: //tasto "invio" per confermare la scelta
                if (scelta == 0) {
                    clear();


                    attron(A_BOLD);
                    mvprintw(max_y / 2 - 1, (max_x - 25) / 2, "INSERISCI IL TUO USERNAME:");
                    attroff(A_BOLD);

                    echo();
                    curs_set(1);
                    int start_col = (max_x - NOME) / 2; //calcolo per trovare la posizione centrale per la stampa
                    mvprintw(max_y / 2 + 1, start_col, "");
                    mvgetnstr(max_y / 2 + 1, start_col, username, NOME - 1);
                    noecho();
                    curs_set(0);
                    username[NOME - 1] = '\0';

                    clear();

                    attron(A_BOLD);
                    mvprintw(max_y / 2 - 2, (max_x - strlen(username) - 20) / 2, "BENVENUTO, %s!", username);
                    mvprintw(max_y / 2, (max_x - 28) / 2, "IL GIOCO STA PER INIZIARE...");
                    attroff(A_BOLD);

                    refresh();
                    sleep(2);
                    inizio(username);
                } else if (scelta == 1) {
                    clear();
                    mvprintw(3, 5, "Grazie per aver giocato!");
                    getch();
                    return;
                }
                break;
            default:
                break;
        }
    }
}

void inizio(const char *username) {
    clear();

    //dimens massima schermo
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    //inizializzo la schermata di gioco
    int punteggio = 0;
    int vite = 5;
    int tempo_rimasto = TEMPO_MAX;
    int rana_x = max_x / 2 - 1;
    int rana_y = max_y - 5;

    while (vite > 0) {
        while (tempo_rimasto >= 0) {
            clear();

            //stampo il numero di vite nella riga più in alto
            attron(COLOR_PAIR(2));
            mvprintw(0, max_x - 15, "Vite: %d", vite);
            attroff(COLOR_PAIR(2));

            //stampo usernme e punteggio nella riga più in basso
            attron(COLOR_PAIR(2));
            mvprintw(max_y - 1, 0, "Username: %s", username);
            attroff(COLOR_PAIR(2));

            attron(COLOR_PAIR(1));
            mvprintw(max_y - 1, 30, "Punteggio: %d", punteggio);
            attroff(COLOR_PAIR(1));

            //timer
            int barra_lunghezza = 50 * tempo_rimasto / TEMPO_MAX; //50 sono le barre
            attron(COLOR_PAIR(4));
            mvprintw(max_y - 1, max_x - 60, "Timer: ");
            for (int i = 0; i < barra_lunghezza; i++) {
                mvprintw(max_y - 1, max_x - 52 + i, "|");
            }
            attroff(COLOR_PAIR(4));

            refresh();
            sleep(1);
            tempo_rimasto--;
        }

        //tolgo una vita quando il timer finisce
        vite--;
        tempo_rimasto = TEMPO_MAX;


        if (vite == 0) {
            clear();
            attron(COLOR_PAIR(3));
            mvprintw(max_y / 2, (max_x - 10) / 2, "GAME OVER");
            attroff(COLOR_PAIR(3));
            refresh();
            sleep(2);
            return;
        }
    }
}
