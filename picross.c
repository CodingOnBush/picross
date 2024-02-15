#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include<locale.h>


int L,C;
void LANCEMENT_JEU();


/**Initialisation de ncurses**/
void ncurses_initialiser() {
    initscr();	        /* Démarre le mode ncurses */
    cbreak();	        /* Pour les saisies clavier (desac. mise en buffer) */
    noecho();             /* Désactive l'affichage des caractères saisis */
    keypad(stdscr, TRUE);	/* Active les touches spécifiques */
    refresh();            /* Met a jour l'affichage */
    curs_set(FALSE);      /* Masque le curseur */
}


/**Initialisation des couleurs**/
void ncurses_couleurs() {
    /* Vérification du support de la couleur */
    if(has_colors() == FALSE) {
        endwin();
        fprintf(stderr, "Le terminal ne supporte pas les couleurs.\n");
        exit(EXIT_FAILURE);
    }
    /* Activation des couleurs */
    start_color();
    //init_color(COLOR_BLACK, 1000, 1000, 1000);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
}

/**Initialisation de la souris**/
void ncurses_souris() {
    if(!mousemask(ALL_MOUSE_EVENTS, NULL)) {
    endwin();
    fprintf(stderr, "Erreur lors de l'initialisation de la souris.\n");
    exit(EXIT_FAILURE);
    }
}


/**Pour récupérer les coordonnées (x,y) du clic de la souris**/
int click_souris(){
    MEVENT event ;
    int ch;

    while((ch = getch()) != KEY_F(1)){
        switch(ch){
            case KEY_F(2): /*Pour quitter la boucle*/
            return 1;
            case KEY_MOUSE:
            if(getmouse(&event) == OK){
                C = event.x;
                L = event.y;
                if(event.bstate & BUTTON1_CLICKED){
                    return 0;
                }
            }
        }
    }
    return 0;
}



/*Pour récupérer les données saisies par l'utilisateur sans bloquer le jeu*/
char key_pressed() {
    struct termios oldterm, newterm;
    int oldfd;
    char c, result = 0;
    tcgetattr (STDIN_FILENO, &oldterm);
    newterm = oldterm;
    newterm.c_lflag &= ~(ICANON | ECHO);
    tcsetattr (STDIN_FILENO, TCSANOW, &newterm);
    oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl (STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
    c = getchar();
    tcsetattr (STDIN_FILENO, TCSANOW, &oldterm);
    fcntl (STDIN_FILENO, F_SETFL, oldfd);
    if (c != EOF) {
        ungetc(c, stdin);
        result = getchar();
    }
    return result;
}

void start() {
    mvaddstr(3,0,"██████╗ ██╗ ██████╗██████╗  ██████╗ ███████╗███████╗\n");
    usleep(100000); refresh();
    mvaddstr(4,0,"██╔══██╗██║██╔════╝██╔══██╗██╔═══██╗██╔════╝██╔════╝\n");
    usleep(100000); refresh();
    mvaddstr(5,0,"██████╔╝██║██║     ██████╔╝██║   ██║███████╗███████╗\n");
    usleep(100000); refresh();
    mvaddstr(6,0,"██╔═══╝ ██║██║     ██╔══██╗██║   ██║╚════██║╚════██║\n");
    usleep(100000); refresh();
    mvaddstr(7,0,"██║     ██║╚██████╗██║  ██║╚██████╔╝███████║███████║\n");
    usleep(100000); refresh();
    mvaddstr(8,0,"╚═╝     ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝\n");
    refresh();
    usleep(500000);

    mvaddstr(3,0,"                                                    \n");
    mvaddstr(4,0,"                                                    \n");
    mvaddstr(5,0,"                                                    \n");
    mvaddstr(6,0,"                                                    \n");
    mvaddstr(7,0,"                                                    \n");
    mvaddstr(8,0,"                                                    \n");
    refresh();
    sleep(1);
    mvaddstr(3,0,"██████╗ ██╗ ██████╗██████╗  ██████╗ ███████╗███████╗\n");
    mvaddstr(4,0,"██╔══██╗██║██╔════╝██╔══██╗██╔═══██╗██╔════╝██╔════╝\n");
    mvaddstr(5,0,"██████╔╝██║██║     ██████╔╝██║   ██║███████╗███████╗\n");
    mvaddstr(6,0,"██╔═══╝ ██║██║     ██╔══██╗██║   ██║╚════██║╚════██║\n");
    mvaddstr(7,0,"██║     ██║╚██████╗██║  ██║╚██████╔╝███████║███████║\n");
    mvaddstr(8,0,"╚═╝     ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝\n");
    mvaddstr(10,1,"Cliquez pour commencer");
    refresh();
}

void menu() {
    clear();
    mvaddstr(3,0,"██████╗ ██╗ ██████╗██████╗  ██████╗ ███████╗███████╗\n");
    mvaddstr(4,0,"██╔══██╗██║██╔════╝██╔══██╗██╔═══██╗██╔════╝██╔════╝\n");
    mvaddstr(5,0,"██████╔╝██║██║     ██████╔╝██║   ██║███████╗███████╗\n");
    mvaddstr(6,0,"██╔═══╝ ██║██║     ██╔══██╗██║   ██║╚════██║╚════██║\n");
    mvaddstr(7,0,"██║     ██║╚██████╗██║  ██║╚██████╔╝███████║███████║\n");
    mvaddstr(8,0,"╚═╝     ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝\n");

    mvaddstr(10,3,"╔═╗┌─┐┬  ┌─┐┌─┐┌┬┐  ┌─┐  ┌┬┐┌─┐┌┬┐┌─┐ .\n");
    mvaddstr(11,3,"╚═╗├┤ │  ├┤ │   │   ├─┤  ││││ │ ││├┤  .\n");
    mvaddstr(12,3,"╚═╝└─┘┴─┘└─┘└─┘ ┴   ┴ ┴  ┴ ┴└─┘─┴┘└─┘  \n");

    mvaddstr(15,2,"  ┌──────────────┐\n");
    mvaddstr(16,2,"  │ 1 : Random   │\n");
    mvaddstr(17,2,"  └──────────────┘\n");

    mvaddstr(15,20,"  ┌──────────────┐\n");
    mvaddstr(16,20,"  │ 2 : Picture  │\n");
    mvaddstr(17,20,"  └──────────────┘\n");

    mvaddstr(15,39," ┌──────────────┐\n");
    mvaddstr(16,39," │ q : Exit     │\n");
    mvaddstr(17,39," └──────────────┘\n");
}

void exit_anim() {
    int cmpt=0;
    for (cmpt=0;cmpt<3;cmpt++) {
        mvaddstr(25,75,"EXITING.\n");
        refresh();
        usleep(500000);
        mvaddstr(25,75,"EXITING..\n");
        refresh();
        usleep(500000);
        mvaddstr(25,75,"EXITING...\n");
        refresh();
        usleep(500000);
        clear();
    }
}

void Victoire_Anim() {
    int cmpt=0;
    for(;cmpt<3;cmpt++) {
        mvaddstr(21,55,"██╗   ██╗ ██████╗ ██╗   ██╗    ██╗    ██╗██╗███╗   ██╗    ██╗\n");
        mvaddstr(22,55,"╚██╗ ██╔╝██╔═══██╗██║   ██║    ██║    ██║██║████╗  ██║    ██║\n");
        mvaddstr(23,55," ╚████╔╝ ██║   ██║██║   ██║    ██║ █╗ ██║██║██╔██╗ ██║    ██║\n");
        mvaddstr(24,55,"  ╚██╔╝  ██║   ██║██║   ██║    ██║███╗██║██║██║╚██╗██║    ╚═╝\n");
        mvaddstr(25,55,"   ██║   ╚██████╔╝╚██████╔╝    ╚███╔███╔╝██║██║ ╚████║    ██╗\n");
        mvaddstr(26,55,"   ╚═╝    ╚═════╝  ╚═════╝      ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝    ╚═╝\n");
        refresh();
        sleep(1);
        mvaddstr(21,55,"                                                             \n");
        mvaddstr(22,55,"                                                             \n");
        mvaddstr(23,55,"                                                             \n");
        mvaddstr(24,55,"                                                             \n");
        mvaddstr(25,55,"                                                             \n");
        mvaddstr(26,55,"                                                             \n");
        refresh();
        sleep(1);
    }
    mvaddstr(21,55,"██╗   ██╗ ██████╗ ██╗   ██╗    ██╗    ██╗██╗███╗   ██╗    ██╗\n");
    mvaddstr(22,55,"╚██╗ ██╔╝██╔═══██╗██║   ██║    ██║    ██║██║████╗  ██║    ██║\n");
    mvaddstr(23,55," ╚████╔╝ ██║   ██║██║   ██║    ██║ █╗ ██║██║██╔██╗ ██║    ██║\n");
    mvaddstr(24,55,"  ╚██╔╝  ██║   ██║██║   ██║    ██║███╗██║██║██║╚██╗██║    ╚═╝\n");
    mvaddstr(25,55,"   ██║   ╚██████╔╝╚██████╔╝    ╚███╔███╔╝██║██║ ╚████║    ██╗\n");
    mvaddstr(26,55,"   ╚═╝    ╚═════╝  ╚═════╝      ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝    ╚═╝\n");
    refresh();
    sleep(1);
    clear();

    LANCEMENT_JEU();
}

void anim_Debut() {
    int cmpt=0;
    for (;cmpt<3;cmpt++) {
        clear();
        mvaddstr(2,2,"Game loading, get ready.");
        refresh(); usleep(500000);
        mvaddstr(2,2,"Game loading, get ready..");
        refresh(); usleep(500000);
        mvaddstr(2,2,"Game loading, get ready...");
        refresh(); usleep(500000);
    }
    clear();
    mvaddstr(4,2,"  ┌──────────────┐\n");
    mvaddstr(5,2,"  │    START     │\n");
    mvaddstr(6,2,"  └──────────────┘\n");
}

void load_Board(int mode,int nbLin,int nbCol, int board[nbLin][nbCol]) {
    int cmpt_L=0,cmpt_C=0;
    if (mode==0) {					//Loading player board. Initialized to 0.
        for(;cmpt_L<nbLin;cmpt_L++) {
            for(cmpt_C=0;cmpt_C<nbCol;cmpt_C++) {
            board[cmpt_L][cmpt_C]=0;
            }
        }
    }
    if (mode==1) {					//Loading answer board ; Random mode
        for(;cmpt_L<nbLin;cmpt_L++) {
            for(cmpt_C=0;cmpt_C<nbCol;cmpt_C++) {
                board[cmpt_L][cmpt_C] = rand()%2;
                switch(board[cmpt_L][cmpt_C]) {
                    case 0 :
                        board[cmpt_L][cmpt_C]=1;
                        break;
                    default :
                        board[cmpt_L][cmpt_C]=0;
                        break;
                }
            }
        }
    }

}

int SelectMode(){
    for (;;) {
        int c = getch();
        if (c=='1') return 1;
        if (c=='2') return 2;
        if (c=='q') return 'q';
    }
}

int AskNumber(const char *prompt){
    addstr(prompt);
    refresh();
    char buf[100];
    int value;
    for(;;) {
        getnstr(buf,100);
        if (sscanf(buf,"%d",&value)==1 && ((value>=3 && (value<=15)))) {
            return value;
        }
        else {
            attron(COLOR_PAIR(3)); move(21,30);
            addstr("\n  Enter a value between 3 and 15");
            attron(COLOR_PAIR(1));
        }
    }
}

int AskLevel(){
    addstr("Level Selection :");
    refresh();
    char buf[100];
    int value;
    mvaddstr(22,2,"  ┌──────────────┐\n");
    mvaddstr(23,2,"  │ 1 : Rocket   │\n");
    mvaddstr(24,2,"  └──────────────┘\n");
    mvaddstr(22,20,"  ┌───────────────┐\n");
    mvaddstr(23,20,"  │ 2 : Happy Face│\n");
    mvaddstr(24,20,"  └───────────────┘\n");
    mvaddstr(22,40,"  ┌───────────────┐\n");
    mvaddstr(23,40,"  │ 3 : Best team │\n");
    mvaddstr(24,40,"  └───────────────┘\n");

    for(;;) {
        getnstr(buf,100);
        if (sscanf(buf,"%d",&value)==1 && ((value>=1 && (value<=3)))) {
            return value;
        }
    }

}

int line_Check(int lin_Check, int nb_Lin, int nb_Col, int answer_TAB[nb_Lin][nb_Col], int 	player_TAB[nb_Lin][nb_Col]) {
    int cmpt_Col=0, check=1;
    for (;cmpt_Col<nb_Col;cmpt_Col++) {
        if (answer_TAB[lin_Check][cmpt_Col]==0 && player_TAB[lin_Check][cmpt_Col]==1) check=0;
        if (answer_TAB[lin_Check][cmpt_Col]==1 && player_TAB[lin_Check][cmpt_Col]!=1) check=0;
    }
    return check;
}

int col_Check(int col_Check, int nb_Lin, int nb_Col, int answer_TAB[nb_Lin][nb_Col], int player_TAB[nb_Lin][nb_Col]) {
    int cmpt_Lin=0, check=1;
    for (;cmpt_Lin<nb_Lin;cmpt_Lin++) {
        if (answer_TAB[cmpt_Lin][col_Check]==0 && player_TAB[cmpt_Lin][col_Check]==1) check=0;
        if (answer_TAB[cmpt_Lin][col_Check]==1 && player_TAB[cmpt_Lin][col_Check]!=1) check=0;
    }
    return check;
}

int* side_Board(int val, int nb_Lin, int nb_Col, int TAB[nb_Lin][nb_Col], int mode) { //Renvoie un tableau contenant les sequences reponse d'une ligne/colonne
    int flag=0,cmpt_Board=0,cmpt_Sequence=0,size,cmpt=0;
    int* board;
    if (mode==0){
        //parcours de la ligne val
        //flag=1 signifie que la derniere case parcourue etait remplie
        size=ceil(nb_Col/2);
        board=calloc(size, sizeof(int));
        for (;cmpt<nb_Col;cmpt++) {
            if (TAB[val][cmpt]==1) {
                cmpt_Sequence++;
                flag=1;
            }
            else {
                if (flag==1) {
                    *(board+cmpt_Board)=cmpt_Sequence;
                    flag=0;
                    cmpt_Sequence=0;
                    cmpt_Board++;
                }
            }
        }
        if (flag==1) {
            *(board+cmpt_Board)=cmpt_Sequence;
            flag=0;
            cmpt_Sequence=0;
            cmpt_Board++;
        }
    }

    if (mode==1) { //Parcours de la colonne val
        size=ceil(nb_Lin/2);
        board=calloc(size, sizeof(int));
        for (;cmpt<nb_Lin;cmpt++) {
            if (TAB[cmpt][val]==1) {
                cmpt_Sequence++;
                flag=1;
            }
            else {
                if (flag==1) {
                    *(board+cmpt_Board)=cmpt_Sequence;
                    flag=0;
                    cmpt_Sequence=0;
                    cmpt_Board++;
                }
            }
        }
        if (flag==1) {
            *(board+cmpt_Board)=cmpt_Sequence;
            flag=0;
            cmpt_Sequence=0;
            cmpt_Board++;
        }
    }
    return board;
}


int** TAB_G(int nb_Lin, int nb_Col, int TAB[nb_Lin][nb_Col]) {
    int cmpt=0;
    int** TAB_G = calloc (nb_Lin, sizeof(int*));
    for (;cmpt<nb_Lin; cmpt++) {
        *(TAB_G+cmpt)= side_Board(cmpt,nb_Lin,nb_Col,TAB, 0);
    }
    return TAB_G;
}

int** TAB_D(int nb_Lin, int nb_Col, int TAB[nb_Lin][nb_Col]) {
    int cmpt=0;
    int** TAB_D = calloc (nb_Col, sizeof(int*));
    for (;cmpt<nb_Col; cmpt++) {
        *(TAB_D+cmpt)=side_Board(cmpt,nb_Lin,nb_Col,TAB, 1);
    }
    return TAB_D;
}

void AFFICHAGE_TAB_G(int y, int x, int nb_Lin, int nb_Col, int** tab) {
    int cmpt_L=0,cmpt_C=0, cmpt_TAB=0;
    move(y,x); addch(ACS_ULCORNER);
    for(cmpt_L=0;cmpt_L<nb_Lin;cmpt_L++) {
        for(cmpt_C=0;cmpt_C<nb_Col*4;cmpt_C++) {
            mvaddch(y+cmpt_L*2,x+cmpt_C+1,ACS_HLINE);
        }
        mvaddch(y+cmpt_L*2+1,x,ACS_VLINE);
        mvaddch(y+cmpt_L*2+2,x,ACS_LTEE); mvaddch(y+cmpt_L*2+2,x+nb_Col*4+1,ACS_RTEE);
    }
    for(cmpt_L=0;cmpt_L<nb_Lin;cmpt_L++) {
        move(y+cmpt_L*2+1, x+1);
        for (cmpt_TAB=0;cmpt_TAB<nb_Col;cmpt_TAB++) {
            if (tab[cmpt_L][cmpt_TAB]!=0) printw("%3d", *(*(tab+cmpt_L)+cmpt_TAB));
        }
    }
    mvaddch(y+cmpt_L*2,x,ACS_LLCORNER);
    for (cmpt_C=0;cmpt_C<nb_Col*4;cmpt_C++) addch(ACS_HLINE);
}

void AFFICHAGE_TAB_D(int y, int x, int nb_Lin, int nb_Col, int** tab) {
    int cmpt_L=0,cmpt_C=0, cmpt_TAB=0;
    move(y,x);
    addch(ACS_ULCORNER);
    for(cmpt_C=0;cmpt_C<nb_Col;cmpt_C++) {
        addch(ACS_HLINE); addch(ACS_HLINE); addch(ACS_HLINE); addch(ACS_TTEE);
    }
    for(cmpt_L=0;cmpt_L<nb_Lin*2;cmpt_L++) {
        for(cmpt_C=0;cmpt_C<=nb_Col;cmpt_C++) {
            mvaddch(y+cmpt_L+1,x+4*cmpt_C,ACS_VLINE);
        }
    }
    for(cmpt_C=0;cmpt_C<nb_Col;cmpt_C++) {
        for (cmpt_TAB=0;cmpt_TAB<nb_Lin;cmpt_TAB++) {
            if (tab[cmpt_C][cmpt_TAB]!=0) {
                move(y+cmpt_TAB+1,x+cmpt_C*4+1);
                printw("%2d", *(*(tab+cmpt_C)+cmpt_TAB));
            }
        }
    }
    mvaddch(y,x+nb_Col*4,ACS_URCORNER);
}

void update_TAB(int nb_Lin, int nb_Col, int caseX, int caseY, int tab_player[nb_Lin][nb_Col]) {
    tab_player[caseY][caseX]=(tab_player[caseY][caseX]+1)%3;
    }

    int Victoire(int nb_Lin, int nb_Col, int player_tab[nb_Lin][nb_Col], int answer_tab[nb_Lin][nb_Col]) {
    int cmpt_L=0, cmpt_C=0, victoire=1;
    for (;cmpt_L<nb_Lin;cmpt_L++) {
        if (line_Check(cmpt_L,nb_Lin,nb_Col,answer_tab,player_tab)==0) {
            victoire =0; return victoire;
        }
    }
    for (;cmpt_C<nb_Col;cmpt_C++) {
        if (col_Check(cmpt_C,nb_Lin,nb_Col,answer_tab,player_tab)==0) {
            victoire =0; return victoire;
        }
    }
    return victoire;
}

void AFFICHAGE_TAB(int y, int x, int nb_Lin, int nb_Col, int tab[nb_Lin][nb_Col]) {
    int cmpt_L=0,cmpt_C=0;
    move(y,x);

    for (cmpt_L=0;cmpt_L<nb_Lin;cmpt_L++) { //AFFICHAGE GRILLE INCOMPLET
        for (cmpt_C=0;cmpt_C<nb_Col;cmpt_C++) {
            mvaddch(y+cmpt_L*2,x+cmpt_C*4+1,ACS_HLINE); addch(ACS_HLINE); addch(ACS_HLINE); addch(ACS_PLUS);
            mvaddch(y+cmpt_L*2+1,x+cmpt_C*4,ACS_VLINE);
        }
        mvaddch(y+cmpt_L*2+1,x+nb_Col*4,ACS_VLINE);
        mvaddch(y+cmpt_L*2,x,ACS_PLUS); mvaddch(y+cmpt_L*2,x+nb_Col*4-3,ACS_HLINE); addch(ACS_HLINE); addch(ACS_HLINE); addch(ACS_RTEE);
    }
    //Ici on ajuste les depassements sur les bords
    for (cmpt_C=0;cmpt_C<nb_Col;cmpt_C++) {
        mvaddch(y,x+cmpt_C*4+1,ACS_HLINE) ; addch(ACS_HLINE); addch(ACS_HLINE); addch(ACS_PLUS);
    }
    for (cmpt_C=0;cmpt_C<nb_Col;cmpt_C++) {
        mvaddch(y+nb_Lin*2,x+cmpt_C*4+1,ACS_HLINE) ; addch(ACS_HLINE); addch(ACS_HLINE); addch(ACS_BTEE);
    }
    mvaddch(y,x,ACS_PLUS); mvaddch(y+nb_Lin*2,x,ACS_BTEE); mvaddch(y,x+nb_Col*4,ACS_RTEE); mvaddch(y+nb_Lin*2,x+nb_Col*4,ACS_LRCORNER);//Affichage coins

    //Affichage valeurs
    for (cmpt_L=0;cmpt_L<nb_Lin;cmpt_L++) {
        for (cmpt_C=0;cmpt_C<nb_Col;cmpt_C++) {
            move(y+cmpt_L*2+1,x+cmpt_C*4+1);
            if (tab[cmpt_L][cmpt_C]==1) printw("███");
            if (tab[cmpt_L][cmpt_C]==2) printw(" x");
        }
    }
}

void Jeu(int nb_Lin, int nb_Col, int Answer_board[nb_Lin][nb_Col]) {
    int Player_board[nb_Lin][nb_Col];
    load_Board(0,nb_Lin,nb_Col,Player_board);
    int** TABG = TAB_G(nb_Lin,nb_Col,Answer_board);
    int** TABD = TAB_D(nb_Lin,nb_Col,Answer_board);
    int posX= 1+(ceil(nb_Col/2)*4);
    int posY= 1+nb_Lin;
    anim_Debut();

    while (click_souris() == 0) {
    clear();

    AFFICHAGE_TAB_G(posY,1,nb_Lin, ceil(nb_Col/2), TABG);
    AFFICHAGE_TAB_D(1,posX,ceil(nb_Lin/2),nb_Col, TABD);
    AFFICHAGE_TAB(posY,posX,nb_Lin,nb_Col,Player_board);
    mvaddstr(posY+nb_Lin*2+1,posX,"┌──────────────┐\n");
    mvaddstr(posY+nb_Lin*2+2,posX,"│   Solution   │\n");
    mvaddstr(posY+nb_Lin*2+3,posX,"└──────────────┘\n");
    refresh();
    move(1,0);
    if ((L>=posY+nb_Lin*2+1 && L<=posY+nb_Lin*2+3) && ( C>=posX && C<=posX+16)) {
    clear();
    AFFICHAGE_TAB_G(posY,1,nb_Lin,ceil(nb_Col/2),TABG);
    AFFICHAGE_TAB_D(1,posX,ceil(nb_Lin/2),nb_Col,TABD);
    AFFICHAGE_TAB(posY,posX,nb_Lin,nb_Col,Answer_board);
    refresh(); break;
    }
    if (((0<=(L-posY)/2) && ((L-posY)/2<nb_Lin)) && (((C-posX)/4)>=0) && (((C-posX)/4)<nb_Col)) {
    int caseX = (C-posX)/4;
    int caseY = (L-posY)/2;

    update_TAB(nb_Lin,nb_Col,caseX,caseY,Player_board);
    clear();
    //TESTS VICTOIRE
    AFFICHAGE_TAB_G(posY,1,nb_Lin, ceil(nb_Col/2), TABG);
    AFFICHAGE_TAB_D(1,posX,ceil(nb_Lin/2),nb_Col, TABD);
    AFFICHAGE_TAB(posY,posX,nb_Lin,nb_Col,Player_board);
    mvaddstr(posY+nb_Lin*2+1,posX,"┌──────────────┐\n");
    mvaddstr(posY+nb_Lin*2+2,posX,"│   Solution   │\n");
    mvaddstr(posY+nb_Lin*2+3,posX,"└──────────────┘\n");
    refresh();
    if (Victoire(nb_Lin,nb_Col,Player_board, Answer_board) ==1) {
    Victoire_Anim();
    }
    }
    }
    if (SelectMode()=='q') {
    clear();
    exit_anim();
    endwin();
    exit(-1);
    }
}

void LANCEMENT_JEU(){
    int nb_Lin,nb_Col;
    int mode, level=0;
    int Rocket[6][9]={{0,0,0,0,1,0,0,0,0},
    {0,0,0,1,1,1,0,0,0},
    {0,0,0,1,1,1,0,0,0},
    {0,0,0,1,1,1,0,0,0},
    {0,0,1,1,1,1,1,0,0},
    {0,1,1,0,0,0,1,1,0}};
    int Smiley[7][12]={{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,1,1,0,0,1,1,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0},	{0,0,0,0,0,0,0,0,0,0,0,0},{0,0,1,0,0,0,0,0,0,1,0,0},
    {0,0,1,1,1,1,1,1,1,1,0,0},{0,0,0,0,0,0,0,0,0,0,0,0}};
    int PSG[5][22]={{1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1},
    {1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0},
    {1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1}};

    while (click_souris() == 0)	{
        menu();
        refresh();
        mode = SelectMode();
        switch(mode) {
            case 1:
                mvaddstr(16,5," ████████████ ");
                move(20,2);
                nb_Lin=AskNumber("Height (between 3 and 15) : ");
                attron(COLOR_PAIR(2));
                mvaddstr(20,30,"✓");
                attron(COLOR_PAIR(1));
                move(21,2);
                nb_Col=AskNumber("Width (between 3 and 15) : ");

                break;
            case 2:
                mvaddstr(16,24,"████████████ ");
                move(20,2);
                level=AskLevel();
                if (level==1) {
                    Jeu(6,9,Rocket);
                }
                if (level==2) {
                    Jeu(7,12,Smiley);
                }
                if (level==3) {
                    Jeu(5,22,PSG);
                }
                break;
            case 'q':
                clear();
                exit_anim();
                endwin();
                exit(-1);
        }
        if (level==0) {
            int Answer_board[nb_Lin][nb_Col];
            load_Board(mode,nb_Lin,nb_Col,Answer_board);
            Jeu(nb_Lin,nb_Col,Answer_board);
        }
    }
}



int main() {
    setlocale(LC_ALL, "");
    menu();
    srand(time(NULL));
    ncurses_initialiser(); //Initialisation de ncurses
    ncurses_souris(); //Initialisation de la souris
    ncurses_couleurs();
    refresh();
    start();
    LANCEMENT_JEU(); //Lancement du jeu
    endwin(); // Suspendre la session ncurses et restaurer le terminal

return 0;
}
