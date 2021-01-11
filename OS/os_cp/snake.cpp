// snake.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

//змейка для двух пользователей: первый клиент становиться серевером, второй подключается к первому.
//игра заканчивается, как только будут съедены все 25 яблок, побеждает тот, у кого счет больше.

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "zmq.h"
#include <assert.h>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <cstring>


int height = 25;
int width = 100;

int gameover = 0, counter, gameover2 = 0, counter2, conn;
int status = 0;
int lflag = 0, rflag = 0, uflag = 0, dflag = 0;
int lflag2 = 0, rflag2 = 0, uflag2 = 0, dflag2 = 0;
short fcount;
void *context;
void *serverSocket;

using namespace std;

class Snake {

    int x, y, fx, fy, x2, y2;
    char playername[50], playername2[50];;

    struct node {
        int nx, ny;
        struct node *next;
        struct node *back;
    };
    struct node *head = NULL;
    struct node *head2 = NULL;

    void gotoxy(int x, int y) {
        COORD pos = { x,y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    }

    void nameandscore() {
        textcolour(6);
        gotoxy(101, 2);
        cout << playername << "'s SCORE = " << counter * 100;
         gotoxy(101, 4);
         cout << playername2 << "'s SCORE = " << counter2 * 100;
         gotoxy(101, 6);
         cout << "Remained Fruit :";
         gotoxy(117, 6);
         cout << "  ";
         gotoxy(117, 6);
         cout << fcount;
    }

    void textcolour(int k) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, k);
    }

public:
    void window() {
        textcolour(1);
        for (int i = 0; i <= width; i++){
            gotoxy(i, 0);
            cout << "#";
            gotoxy(i, height);
            cout << "#";
        }
        for (int i = 0; i <= height; i++){
            gotoxy(0, i);
            cout << "#";
            gotoxy(width, i);
            cout << "#";
        }
    }

    void setup(){   //начальные данные
        resetflag2();
        gameover2 = 0;
        counter2 = 0;
        fcount = 25;
        head2 = new node;
        head2->nx = width / 2 + 5;
        head2->ny = height / 2 + 5;
        head2->next = NULL;
        head2->back = NULL;
        x2 = width / 2 + 5;
        y2 = height / 2 + 5;

        counter = 0;
        gameover = 0;
        window();
        resetflag();
        nameandscore();
        head = new node;
        head->nx = width / 2;
        head->ny = height / 2;
        head->next = NULL;
        head->back = NULL;
        x = width / 2;
        y = height / 2;
    label1:
        fx = rand() % width;
        if (fx == 0 || fx == width)
            goto label1;
    label2:
        fy = rand() % height;
        if (fy == 0 || fy == height)
            goto label2;
    }



    void drawlist(struct node *h, int k){ //прорисовка змей
        textcolour(k);
        struct node *ptr;
        ptr = h;
        while (ptr != NULL){
            gotoxy(ptr->nx, ptr->ny);
            cout << "o";
            ptr = ptr->next;
        }
    }

    void destroylist(struct node *h) {
        struct node *ptr;
        ptr = h;
        while (ptr != NULL){
            gotoxy(ptr->nx, ptr->ny);
            cout << " ";
            ptr = ptr->next;
        }
    }

    void draw(){    //прорисовка змей и еды
        drawlist(head, 2);
        drawlist(head2, 5);
        gotoxy(fx, fy); 
        textcolour(4);
        cout << "@";
        Sleep(70);
        destroylist(head);
        destroylist(head2);
    }

    void resetflag(){
        uflag = 0;
        dflag = 0;
        lflag = 0;
        rflag = 0;
    }

    void resetflag2(){
        uflag2 = 0;
        dflag2 = 0;
        lflag2 = 0;
        rflag2 = 0;
    }

    void play(){
        int h;
        char ch;
        if (_kbhit()){
            ch = _getch();
            h = ch;
            switch (h){
            case 72:if (dflag != 1) { resetflag(); uflag = 1; }
                    break;
            case 80:if (uflag != 1) { resetflag(); dflag = 1; }
                    break;
            case 75:if (rflag != 1) { resetflag(); lflag = 1; }
                    break;
            case 77:if (lflag != 1) { resetflag(); rflag = 1; }
                    break;
            default:break;
            }
        }
    }

    void connection1() {
        textcolour(6);
        gotoxy(width / 2 - 18, height / 2 - 3);
        cout << "WELCOME TO SNAKE GAME MULTIPLAYER MODE ";
        textcolour(8);
        gotoxy(width / 2 - 18, height / 2);
        cout << "You can you can connect to another player or start your own game";
        gotoxy(width / 2 - 18, height / 2 + 2);
        cout << "Enter connect or create new game (1 or 2): ";
        cin >> conn;
        string ip;
        
        //string good;
        if (conn == 1) {    //подключиться
                gotoxy(width / 2 - 13, height / 2 + 4);
                cout << "Enter player's IP: ";
                cin >> ip;

                context = zmq_ctx_new();
                serverSocket = zmq_socket(context, ZMQ_REQ);
                string addr = "tcp://" + ip + ":4040";
                int rc = zmq_connect(serverSocket, addr.c_str());
                //assert(rc == 0);

                // WAITING PLAYERS
                //good = "ok";
                status = 1;
                //zmq_send(serverSocket, &status, sizeof(int), 0);
                //zmq_recv(serverSocket, &good, sizeof(good), 0);
                zmq_msg_t zmqMessage;
                zmq_msg_init_size(&zmqMessage, sizeof(int));
                memcpy(zmq_msg_data(&zmqMessage), &status, sizeof(int));
                int send = zmq_msg_send(&zmqMessage, serverSocket, 0);
                zmq_msg_close(&zmqMessage);

                zmq_msg_t reply;
                zmq_msg_init(&reply);
                zmq_msg_recv(&reply, serverSocket, 0);
                zmq_msg_close(&reply);

        }
        else if (conn == 2) {   //создать игру
                int Timeout = 2000;
                context = zmq_ctx_new();
                serverSocket = zmq_socket(context, ZMQ_REP);
                
                zmq_setsockopt(serverSocket, ZMQ_LINGER, &Timeout, sizeof(Timeout));
                zmq_bind(serverSocket, "tcp://*:4040");

                // WAITING PLAYERS
                gotoxy(width / 2 - 13, height / 2 + 4);
                cout << "Wait....";

                zmq_msg_t stat;
                zmq_msg_init(&stat);
                zmq_msg_recv(&stat, serverSocket, 0);
                int *m = (int *)zmq_msg_data(&stat);
                zmq_msg_close(&stat);

                zmq_msg_t reply;
                zmq_msg_init_size(&reply, strlen("ok") + 1);
                memcpy(zmq_msg_data(&reply), "ok\0", 3);
                zmq_msg_send(&reply, serverSocket, 0);
                zmq_msg_close(&reply);
                //zmq_recv(serverSocket, &status, sizeof(int), 0);
                //zmq_send(serverSocket, &good, sizeof(good), 0);
        }
         else { cout << "Enter correct number"; }    
        system("cls");
    }

    void welcome(){
        connection1();
        textcolour(8);
        gotoxy(width / 2 - 15, height / 2);
        cout << "Enter Player Name : ";
        gotoxy(width / 2 + 7, height / 2);
        cin >> playername;

        char* ptr_playername;

        if (status == 1) {  //клиент
            zmq_msg_t zmqMessage;
            zmq_msg_init_size(&zmqMessage, sizeof(playername));
            memcpy(zmq_msg_data(&zmqMessage), &playername, sizeof(playername));
            int send = zmq_msg_send(&zmqMessage, serverSocket, 0);
            zmq_msg_close(&zmqMessage);

            zmq_msg_t reply;
            zmq_msg_init(&reply);
            zmq_msg_recv(&reply, serverSocket, 0);
            ptr_playername = (char *)zmq_msg_data(&reply);
            zmq_msg_close(&reply);
        }
        else {
            zmq_msg_t stat;
            zmq_msg_init(&stat);
            zmq_msg_recv(&stat, serverSocket, 0);
            ptr_playername = (char *)zmq_msg_data(&stat);

            zmq_msg_close(&stat);

            zmq_msg_t reply;
            zmq_msg_init_size(&reply, sizeof(playername));
            memcpy(zmq_msg_data(&reply), &playername, sizeof(playername));
            zmq_msg_send(&reply, serverSocket, 0);
            zmq_msg_close(&reply);

        }
        //memcpy(&playername2, &ptr_playername, sizeof(ptr_playername));
        strcpy(playername2, ptr_playername);
        system("cls");
    }

    char end(){
        char c;
        gotoxy(width / 2 - 5, height / 2 - 4);
        cout << "GAME OVER \n";
        textcolour(1);
        gotoxy(width / 2 - 15, height / 2 - 2);
        cout << playername << " You Scored : " << counter * 100;
        gotoxy(width / 2 - 15, height / 2);
        cout << playername2 << " You Scored : " << counter2 * 100;
        textcolour(4);
        if (gameover != 0) {
                gotoxy(width / 2 - 15, height / 2 + 2);
                cout << playername << " has lost !";
        }
         else{
                gotoxy(width / 2 - 15, height / 2 + 2);
                cout << playername2 << " has lost !";
         }
         if (fcount == 0){
                textcolour(4);
                gotoxy(width / 2 - 15, height / 2 + 2);
                if (counter > counter2){
                    cout << playername << " has WON !";
                }
                else{
                    cout << playername2 << " has WON !";
                }
          }
        textcolour(6);
        gotoxy(width / 2 - 15, height / 2 + 4);
        cout << "Want To Play Again ? (Y/N) : ";
        cin >> c;
        system("cls");
        return c;

    }

    struct xy {
        int x;
        int y;
    };

    xy coords_node1;
    xy coords_node2;
    xy *c_node2;

    void run(){
        if (uflag == 1)
            y--;
        else if (dflag == 1)
            y++;
        else if (lflag == 1)
            x--;
        else if (rflag == 1)
            x++;

        coords_node1.x = x;
        coords_node1.y = y;

        /*if (uflag2 == 1)
            y2--;
        else if (dflag2 == 1)
            y2++;
        else if (lflag2 == 1)
            x2--;
        else if (rflag2 == 1)
            x2++;
            */

        if (status == 1) {  //клиент
            zmq_msg_t zmqMessage;
            zmq_msg_init_size(&zmqMessage, sizeof(xy));
            memcpy(zmq_msg_data(&zmqMessage), &coords_node1, sizeof(xy));
            int send = zmq_msg_send(&zmqMessage, serverSocket, 0);
            zmq_msg_close(&zmqMessage);

            zmq_msg_t reply;
            zmq_msg_init(&reply);
            zmq_msg_recv(&reply, serverSocket, 0);
            c_node2 = (xy *)zmq_msg_data(&reply);
            zmq_msg_close(&reply);  
        }
        else {
            zmq_msg_t stat;
            zmq_msg_init(&stat);
            zmq_msg_recv(&stat, serverSocket, 0);
            c_node2 = (xy *)zmq_msg_data(&stat);

            zmq_msg_close(&stat);

            zmq_msg_t reply;
            zmq_msg_init_size(&reply, sizeof(xy));
            memcpy(zmq_msg_data(&reply), &coords_node1, sizeof(xy));
            zmq_msg_send(&reply, serverSocket, 0);
            zmq_msg_close(&reply);
        
        }
        x2 = c_node2->x;
        y2 = c_node2->y;
    }

    void dolist(struct node *h, int pp, int qq) {
        struct node *ptr, *prev;
        ptr = h;
        prev = h;

        while (ptr->next != NULL) {
            prev = ptr;
            ptr = ptr->next;
        }
        while (prev != h){
            ptr->nx = prev->nx;
            ptr->ny = prev->ny;
            prev = prev->back;
            ptr = ptr->back;
        }
        ptr->nx = prev->nx;
        ptr->ny = prev->ny;
        prev->nx = pp;
        prev->ny = qq;
    }

    void drawagain() {
        if (x == width) {
            x = 1;
        }
        if (x == 0) {
            x = width - 1;
        }
        if (y == 0) {
            y = height - 1;
        }
        if (y == height) {
            y = 1;
        }
    }


    void drawagain2() {
        if (x2 == width) {
            x2 = 1;
        }
        if (x2 == 0) {
            x2 = width - 1;
        }
        if (y2 == 0) {
            y2 = height - 1;
        }
        if (y2 == height) {
            y2 = 1;
        }
    }

    void generatefruit() {
    label1:
        fx = rand() % width;
        if (fx == 0 || fx == width)
            goto label1;
    label2:
        fy = rand() % height;
        if (fy == 0 || fy == height)
            goto label2;
    }

    void checkfcount() {
        if (fcount == 0) {
            gameover = 1;
            gameover2 = 1;
        }
    }

    void checkup() {
        drawagain();
        if (x == fx && y == fy){
            fcount--;
            checkfcount();
            struct node *t, *ptr, *prev;
            t = new node;
            t->next = NULL;
            t->back = NULL;
            ptr = head;
            prev = head;
            while (ptr->next != NULL){
                ptr = ptr->next;
            }
            ptr->next = t;
            t->back = ptr;
            generatefruit();
            counter++;
            nameandscore();
        }
        dolist(head, x, y);
    }

    void checkup2(){
        drawagain2();
        if (x2 == fx && y2 == fy){
            fcount--;
            checkfcount();
            struct node *t, *ptr, *prev;
            t = new node;
            t->next = NULL;
            t->back = NULL;
            ptr = head2;
            prev = head2;
            while (ptr->next != NULL) {
                ptr = ptr->next;
            }
            ptr->next = t;
            t->back = ptr;
            generatefruit();
            counter2++;
            nameandscore();
        }
        dolist(head2, x2, y2);
    }

    void game(){
        char ch;
        welcome();
        do {
            setup();
            window();
            while (gameover != 1 && gameover2 != 1){
                draw();
                play();
                run();
                checkup();
                checkup2();
            }ch = end();
        } while (ch == 'y' || ch == 'Y');
    }

};
int main(){

    Snake s;
     s.game();

    system("exit");
}

