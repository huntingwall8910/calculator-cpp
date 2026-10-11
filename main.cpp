#include <iostream>
#include <deque>
#include <random>
#include <thread>
#include <chrono>
#include <cctype>
#include <conio.h>
using namespace std; //big nono for some reason
const int sz = 20;
char game[sz][sz];
int length = 1; // player length
string playerDir = "down";
pair<int,int> head(0,0); //snake head
deque<pair<int,int>> q; //fifo queue for snake logic
bool playing = true; //for stopping behavior
pair<int,int> nxt;
pair<int,int> apple;
random_device rd; 
mt19937 gen(rd()); 
uniform_int_distribution<int> distrib(0, sz-1); 
bool won = false;
void display(){
    cout << "\x1B[H";
    for (int i = 0; i < sz; i++){
        for (int j = 0; j < sz; j++){
            if (game[i][j] == '#') cout << "\033[32m";
            else if (game[i][j] == 'A') cout << "\033[31m";
            cout << game[i][j] << " " << "\033[0m";
        }
        cout << '\n';
    }
}
void checkMove(){
    while (_kbhit()) {
        char ch = _getch();
        ch = tolower(ch);
        if (ch == 'w' && playerDir != "down")playerDir = "up";
        else if (ch == 'a' && playerDir != "right") playerDir = "left";
        else if (ch == 's' && playerDir != "up") playerDir = "down";
        else if (ch == 'd' && playerDir != "left") playerDir = "right";
    }
}
//to make code more intuitive
void die(){
    playing = false;
    return;
}
void generateApple(){
    bool insideSnake = true; //prevent apple from spawning in snake
    while (insideSnake){
        apple.first = distrib(gen);
        apple.second = distrib(gen);
        insideSnake = false;
        for (const auto& p : q){
            if (apple == p){
                insideSnake = true;
            }
        }
    }
    game[apple.first][apple.second] = 'A';
}
void loop(){
    if (length >= sz * sz){
        won = true;
        die();
        return;
    }
    checkMove();
    //simulate next move before doing
    nxt = head;
    if (playerDir == "down") nxt.first++;
    else if (playerDir == "up") nxt.first--;
    else if (playerDir == "left") nxt.second--;
    else if (playerDir == "right") nxt.second++;
    //self intersection
    for (const auto& p : q){
        if (nxt == p){
            die();
            return;
        }
    }
    if (nxt == apple){
        generateApple();
        length++;
    }
    //escape behavior
    if (nxt.first >= sz || nxt.first <= -1 || nxt.second >= sz || nxt.second <= -1){
        die();
        return;
    }
    head = nxt;
    //snake behavior
    q.push_back(head);
    game[head.first][head.second] = '#';
    while (q.size() > length){
        pair<int,int> removed = q.front();
        q.pop_front();
        game[removed.first][removed.second] = '.';
    }
    display();
}
void initGame(){
    for (int i = 0; i < sz; i++){
        for (int j = 0; j < sz; j++){
            game[i][j] = '.';
        }
    }
    q.push_back(head);
    generateApple();
}
int main(){
    cout << "\x1B[2J" << "\033[?25l";;
    initGame();
    while (playing){
        loop();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "\x1B[2J";
    if (won) cout << "You win!";
    cout << "Game over! Your length was: " << length;
    return 0;
}