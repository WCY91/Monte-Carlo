#include <iostream>
#include <string>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
using namespace std;

typedef pair<int, int> node;    //position, score

int moveX[8] = {0, 1, 1, 1, 0, -1, -1, -1}; //棋盤8方向的XY座標變化
int moveY[8] = {-1, -1, 0, 1, 1, 1, 0, -1}; //
string playerR = "XO";  //玩家1是黑X，玩家2是白O
int isMax;  //玩家
int itert[9] = {600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400};  //測試用，迭代次數
int itt = 0;    //指定要跑多少次

bool inBoard(int row, int col);
int countFlipPieces(string gameboard, int player, int grid, int dir);
bool isValidMove(string gameboard, int player, int grid);
bool passTurn(string gameboard, int player);
bool gameOver(string gameboard);
string flipPieces(string gameboard, int player, int grid);
int countColorPieces(string gameboard, int player);
int heuristicScore(string gameboard, int player);
node minimax(string gameboard, int depth, int alpha, int beta, int player); 
node randminmax(string gameboard, int depth, int player);   //當minimax的結果有複數位置同分時，隨機選取一個位置回傳。
bool randplay(string gameboard, int player);        //隨機下子到棋局結束，若玩家獲勝回傳True，否則回傳False
int Mante(string gameboard, int player);
void testManteRuntime();
int minimax_Mante(string gameboard, int player);
void test();
void manVScom();


int main(){
    //OO++++XOOXXX+XOXX++OXXOO+XXOX++XXXX+
    srand(time(NULL));
    //testManteRuntime();
    manVScom();

}

bool inBoard(int row, int col){
    if(row > 5 || row < 0 || col > 5 || col < 0 ){
        return false;
    }
    return true;
}

int countFlipPieces(string gameboard, int player, int grid, int dir){
    int count = 0;
    int row = grid / 6 + moveY[dir], col = grid % 6 + moveX[dir], pos = row * 6 + col;
    while(inBoard(row, col)){
        if((player == 1 && gameboard[pos] == 'O') || (player == 2 && gameboard[pos] == 'X')){
            //cout << '!' << gameboard[pos] <<endl;
            count++;
        }
        else if((player == 1 && gameboard[pos] == 'X') || (player == 2 && gameboard[pos] == 'O')){
            return count;
        }
        else{
            return 0;
        }
        row = row + moveY[dir];
        col = col + moveX[dir];
        pos = row * 6 + col;
    }
    return 0;
}

bool isValidMove(string gameboard, int player, int grid){
    if(gameboard[grid] != '+')  return false;
    for(int dir = 0; dir < 8; dir++){
        if(countFlipPieces(gameboard, player, grid, dir) > 0){
            return true;
        }
    }
    return false;
}	

bool passTurn(string gameboard, int player){
    for(int i = 0; i < gameboard.length(); i++){
        if(gameboard[i] == '+'){
            if (isValidMove(gameboard, player, i)){
            return false;
        }
        }
    }
    return true;
}

bool gameOver(string gameboard){
    return passTurn(gameboard, 1) && passTurn(gameboard,2);
}

string flipPieces(string gameboard, int player, int grid){
    int i, j, cfp, r, c;
    if(!isValidMove(gameboard, player, grid)){
        return gameboard;
    }
    for(i = 0; i < 8; i++){
        cfp = countFlipPieces(gameboard, player, grid, i);
        r = grid / 6 + moveY[i];
        c = grid % 6 + moveX[i];
        for(j = 0; j < cfp; j++){
            gameboard[r * 6 + c] = playerR[player - 1];
            r += moveY[i]; 
            c += moveX[i];
        }
    }
    gameboard[grid] = playerR[player - 1];
    return gameboard;
}

int countColorPieces(string gameboard, int player){
    int count = 0;
    for(int i = 0; i < gameboard.length(); i++){
        if(gameboard[i] == playerR[player - 1]){
            count++;
        }
    }
    return count;
}

int heuristicScore(string gameboard, int player){
    return countColorPieces(gameboard, player) - countColorPieces(gameboard, (player % 2) + 1);
}

node minimax(string gameboard, int depth, int alpha, int beta, int player){
    if(depth <= 0 || gameOver(gameboard)){
        node r(0, heuristicScore(gameboard, isMax));
        return r;
    }
    if(passTurn(gameboard, player)){
        return minimax(gameboard, depth - 1, alpha, beta, player % 2 + 1);
    }
    else if(player == isMax){
        node r(0, alpha);
            for(int i = 0; i < 36; i++){
                if(isValidMove(gameboard, player, i)){
                    node t = minimax(flipPieces(gameboard, player, i), depth - 1, alpha, beta, player % 2 + 1);
                    if(alpha < t.second){
                        alpha = t.second;
                        r.first = i;
                        r.second = t.second;
                    }
                    if(beta <= alpha){
                        break;
                    }   
                }
            }
        return r;
    }
    else{
        node r(0, beta);
        for(int i = 0; i < 36; i++){
            if(isValidMove(gameboard, player, i)){
                node t = minimax(flipPieces(gameboard, player, i), depth - 1, alpha, beta, player % 2 + 1);
                if(beta > t.second){
                    beta = t.second;
                    r.first = i;
                    r.second = t.second;
                }
                if(beta <= alpha){
                    break;
                }
                //cout << i << " " << beta << " " << flipPieces(gameboard, player, i) <<endl;
            }
        }  
        return r;
    }
}

node randminmax(string gameboard, int depth, int player){
    vector<node> v;
    int m = INT_MIN;
    for(int i = 0; i < 36; i++){
        if(isValidMove(gameboard, player, i)){
            node tmp = minimax(flipPieces(gameboard, player, i), depth - 1, INT_MIN, INT_MAX, player % 2 + 1);
            if(tmp.second == m){
                tmp.first = i;
                v.push_back(tmp);
            }
            else if(tmp.second > m){
                m = tmp.second;
                v.clear();
                tmp.first = i;
                v.push_back(tmp);
            }
        }
    }
    return v[rand() % v.size()];
}

bool randplay(string gameboard, int player){
    vector<int> validmove;
    while(!gameOver(gameboard)){
        validmove.clear();
        for(int i = 0; i < 36; i++){
            if(isValidMove(gameboard, player, i))    validmove.push_back(i);
        }
        if(validmove.size() > 0){
            gameboard = flipPieces(gameboard, player, validmove[rand() % validmove.size()]);
            /*for(int i = 0; i < 6; i++){
                for(int j = 0; j < 6; j++){
                    cout << gameboard[i * 6 + j] << ' ';
                }
                cout << endl;
            }
            cout << "============================================\n" << endl;*/
        }
        player = player % 2 + 1;
    }
    if(heuristicScore(gameboard, isMax) > heuristicScore(gameboard, isMax % 2 + 1)){
        return true;
    }
    else{
        return false;
    }
}

int Mante(string gameboard, int player){
    vector<int> v;
    for(int i = 0; i < 36; i++){    //找到當前棋盤所有能下子的位置
        if(isValidMove(gameboard, player, i)){
            v.push_back(i);
        }
    }
    int wintimes = 0;
    int gowhere = 0;
    for(int i = 0; i < v.size(); i++){//每個能下子的位置都迭代n次，紀錄會贏的次數
        int t = 0;
        for(int j = 0; j < itert[itt]; j++){
            if(randplay(flipPieces(gameboard, player, v[i]), player + 2 % 1)){
                t++;
            }
        }
        if(t > wintimes){//贏最多次的會被記錄下來
            wintimes = t;
            gowhere = i;
        }
    }
    return v[gowhere];
}

void testManteRuntime(){
    clock_t start, stop;
    string gameboard;
    int player, depth;
    cin >> gameboard >> player;
    isMax = player;
    for(int i = 0; i < 9; i++){
        double rt = 0;
        for(int j = 0; j < 20; j++){
            start = clock();
            Mante(gameboard, player);
            stop = clock();
            rt += double(stop - start) / CLOCKS_PER_SEC;
        }
        printf("iteration %4d times : %.4lf\n", itert[i], rt / 20);
        itt++;
    }
}

int minimax_Mante(string gameboard, int player){
    int l = 0;
    for(int i = 0; i < gameboard.length(); i++) if(gameboard[i] == '+') l++;
    if(l < 16){
        return Mante(gameboard, player);
    }
    else{
        return randminmax(gameboard, 6, player).first;
    }
}

void test(){
    string gameboard = "++++++++++++++OX++++XO++++++++++++++";
    int p = 1;
    itt = 4;
    while(!gameOver(gameboard)){
        if(!passTurn(gameboard, p)){
            if(p == 1){
                isMax = p;
                gameboard = flipPieces(gameboard, p, minimax_Mante(gameboard, p));
            }
            else if(p == 2){
                isMax = p;
                gameboard = flipPieces(gameboard, p, randminmax(gameboard, 6, p).first);
            }
        }
        cout << "=================================" << endl;
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 6; j++){
                cout << gameboard[i * 6 + j] << ' ';
            }
            cout << endl;
        }
        p = p % 2 + 1;
    }
}

void manVScom(){
    int man;
    while(1){
        cout << "player enter 1(black/first), 2(white/second)" << endl;
        cin >> man;
        if(man == 1 || man == 2){
            break;
        }
    }
    string gameboard = "++++++++++++++OX++++XO++++++++++++++";
    itt = 4;
    isMax = man % 2 + 1;
    int choice = 0;
    int p = 1;
    for(int i = 0; i < 6; i++){
            for(int j = 0; j < 6; j++){
                cout << gameboard[i * 6 + j] << ' ';
            }
            cout << endl;
        }
        cout << "=================================" << endl;
    while(!gameOver(gameboard)){
        if(passTurn(gameboard, p)){
            if(p == man)    cout << "player cant flip!! you suck!!" << endl;
            else            cout << "com cant move" << endl;
        }
        else{
            if(p == man){
                cout << "Player's turn" << endl;
                while(1){
                    cout << "enter 0~35" << endl;
                    cin >> choice;
                    if(isValidMove(gameboard, man, choice)){
                        break;
                    }
                    else{
                        cout << "invalid position!" << endl;
                    }
                }
                gameboard = flipPieces(gameboard, man, choice);
            }
            else{
                cout << "Com's turm" << endl;
                gameboard = flipPieces(gameboard, isMax, minimax_Mante(gameboard, isMax));
            }
        }
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 6; j++){
                cout << gameboard[i * 6 + j] << ' ';
            }
            cout << endl;
        }
        cout << "=================================" << endl;
        p = p % 2 + 1;
    }
    if(heuristicScore(gameboard, man)){
        cout << "Player win!! Points: ";
    }
    else{
        cout << "Com win!! Points: ";
    }
    cout  << heuristicScore(gameboard, man) << endl;
}