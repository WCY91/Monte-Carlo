#include <iostream>
#include <string>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
using namespace std;

typedef pair<int, int> node;    //position, score

int moveX[8] = {0, 1, 1, 1, 0, -1, -1, -1}; //æ£‹ç›¤8æ–¹å‘çš„XYåº§æ¨™è®ŠåŒ–
int moveY[8] = {-1, -1, 0, 1, 1, 1, 0, -1}; //
string playerR = "XO";  //ç©å®¶1æ˜¯é»‘Xï¼Œç©å®¶2æ˜¯ç™½O
int isMax;  //ç©å®¶
int itert[9] = {600, 700, 800, 900, 1000, 1100, 1200, 1300, 2000};  //æ¸¬è©¦ç”¨ï¼Œè¿­ä»£æ¬¡æ•¸
int itt = 0;    //æŒ‡å®šè¦è·‘å¤šå°‘æ¬¡

bool inBoard(int row, int col);
int countFlipPieces(string gameboard, int player, int grid, int dir);
bool isValidMove(string gameboard, int player, int grid);
bool passTurn(string gameboard, int player);
bool gameOver(string gameboard);
string flipPieces(string gameboard, int player, int grid);
int countColorPieces(string gameboard, int player);
int heuristicScore(string gameboard, int player);
node minimax(string gameboard, int depth, int alpha, int beta, int player); 
node randminmax(string gameboard, int depth, int player);   //ç•¶minimaxçš„çµæœæœ‰è¤‡æ•¸ä½ç½®åŒåˆ†æ™‚ï¼Œéš¨æ©Ÿé¸å–ä¸€å€‹ä½ç½®å›å‚³ã€‚
bool randplay(string gameboard, int player);        //éš¨æ©Ÿä¸‹å­åˆ°æ£‹å±€çµæŸï¼Œè‹¥ç©å®¶ç²å‹å›å‚³Trueï¼Œå¦å‰‡å›å‚³False
int Mante(string gameboard, int player);
void testManteRuntime();
int minimax_Mante(string gameboard, int player);
void test();


int countFlipPieces_wcy(string gameboard, int color, int grid, int dir){
 // Convert grid to row and column index
    int row = grid/6;
//    cout <<"row" <<row << endl;
    int col = grid%6;
//    cout <<"col" <<col << endl;
    // Define the increment for each direction
    int dirs[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
    int inc[2] = {dirs[dir][0], dirs[dir][1]};
    int count = 0;
    int i = row + inc[0], j = col + inc[1];
    string players = "XO"; 
    // Check if the next piece is opponent's piece
    char opponent = players[2-color];
    char self = players[color-1];
    while (i >= 0 && i < 6 && j >= 0 && j < 6 && gameboard[i * 6 + j] == opponent) {
        count++;
        i += inc[0];
        j += inc[1];
    }
    
    // Check if the next piece is player's piece
    char player = players[color-1];
    if (i >= 0 && i < 6 && j >= 0 && j < 6 && gameboard[i * 6 + j] == player) {
        return count;
    }
    return 0;
}

bool isValidMove_wcy(string gameboard, int player, int grid){
    if(gameboard[grid] != '+')  return false;
    for(int dir = 0; dir < 8; dir++){
        if(countFlipPieces_wcy(gameboard, player, grid, dir) > 0){
            return true;
        }
    }
    return false;
}	

bool passTurn_wcy(string gameboard, int player){
	
    for(int i = 0; i < gameboard.length(); i++){
        if(gameboard[i] == '+'&&(isValidMove_wcy(gameboard,player,i))){
            return false;
        }
    }
    return true;
}

bool gameOver_wcy(string gameboard){
    return passTurn_wcy(gameboard, 1) && passTurn_wcy(gameboard,2);
}

string flipPieces_wcy(string gameboard, int color, int grid){
	string players = "XO"; 
    int row = grid/6;

    int col = grid%6;
    
    char opponent = players[2-color];
    char self = players[color-1];
    gameboard[row*6+col]=self;
    
    int dirs[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
    
    for(int k=0;k<8;k++){
    	int inc[2] = {dirs[k][0], dirs[k][1]};
    	int i = row + inc[0], j = col + inc[1];
    	int flag = 0;
    	int old_i;
    	int old_j;
    	while (i >= 0 && i < 6 && j >= 0 && j < 6 && gameboard[i * 6 + j] == opponent) {  
			old_i = i;
			old_j = j; 
        	i += inc[0];
        	j += inc[1];       
			
        	if(i >= 0 && i < 6 && j >= 0 && j < 6 && gameboard[i * 6 + j] == self) {
        		
        		flag = 1;
        		break;
			}
    	}
		if(flag == 1){
			int start_row = row;
			int start_col = col;
			while(true){
			
				gameboard[start_row*6+start_col]=self;
				start_row+= inc[0];
				start_col+= inc[1];
				if(start_row==i&&start_col==j) break;
			}
		}   	
    	
	}

    return gameboard;
}

int countColorPieces_wcy(string gameboard, int player){
    int count = 0;
    char self = (player == 2) ? 'O' : 'X';
    for(int i=0;i<36;i++){
    	if(gameboard[i]==self){
    		count++;
		}
	}
    return count;
}

int heuristicScore_wcy(string gameboard, int player){ //­pºâ³õ­±¤Wªº½Öªº´Ñ¤l¦h (³Ó²v 
    return countColorPieces_wcy(gameboard, player) - countColorPieces_wcy(gameboard, 3-player);
}

bool simulate(string gameboard, int players){ 
	int player = 3-players; //¥Ñ©ó¶i¤J¼ÒÀÀ«e·|¥ı¥Ñplayer¤U¤@¨B¦]¦¹¼ÒÀÀªº¶}ÀY¥Ñ¹ï¤â¶}©l 
    vector<int> validMoves;
    for(int i = 0; i < 36; i++){
        if(isValidMove_wcy(gameboard, player, i))    validMoves.push_back(i);
    }
    if(validMoves.size()==0){
    	return false;
	}
    while(!gameOver_wcy(gameboard)){
        for(int i = 0; i < 36; i++){
            if(isValidMove_wcy(gameboard, player, i))    validMoves.push_back(i);
        }
        if(validMoves.size() >= 1){
            gameboard = flipPieces_wcy(gameboard, player, validMoves[rand() % validMoves.size()]);
        }
        player = 3-player;
        validMoves.clear();
    }
    if(heuristicScore_wcy(gameboard, players) > heuristicScore_wcy(gameboard, 3-players)){
        return true;
    }
    else{
        return false;
    }
}
int learnHeuristics(string gameboard, int player,int simulateTimes) {
    vector<int> validMoves;
    vector<int> scores(36, 0); 
	validMoves.clear();
    for (int i = 0; i < 36; i++) {
        if (isValidMove_wcy(gameboard, player, i)) {
            validMoves.push_back(i);
        }
    }
    if (validMoves.size() == 0) {
		return -1; 
	}
	for(int i=0;i<simulateTimes;i++){
		int wins = 0;
		int choose =  rand() % validMoves.size();//ÀH¾÷¿ï¤@­Ó¦X²zªº¨B 
		string newGameboard = gameboard;
		newGameboard = flipPieces_wcy(newGameboard, player, validMoves[choose]); 
 		if (simulate(newGameboard, (player))) { //½ü¨ì¹ï¤â 
            wins++;
        }
        scores[validMoves[choose]] += wins;
	}
    // ¿ï¾Ü³Ó²v³Ì°ªªº²¾°Ê
    int maxScore = INT_MIN;
    int bestMove = -1;
    for (int i = 0; i < 36; i++) {
        if (scores[i] > maxScore && isValidMove_wcy(gameboard, player, i)) {
            maxScore = scores[i];
            bestMove = i;
        }
    }
//    gameboard = flipPieces_wcy(gameboard, player, bestMove); // §ó·s¹CÀ¸ª©­±
    return bestMove; // ªğ¦^³Ì¨Î²¾°Ê
}

int main(){
    //OO++++XOOXXX+XOXX++OXXOO+XXOX++XXXX+
    srand(time(NULL));
    //testManteRuntime();
    test();

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
    for(int i = 0; i < 36; i++){    //æ‰¾åˆ°ç•¶å‰æ£‹ç›¤æ‰€æœ‰èƒ½ä¸‹å­çš„ä½ç½®
        if(isValidMove(gameboard, player, i)){
            v.push_back(i);
        }
    }
    int wintimes = 0;
    int gowhere = 0;
    for(int i = 0; i < v.size(); i++){//æ¯å€‹èƒ½ä¸‹å­çš„ä½ç½®éƒ½è¿­ä»£næ¬¡ï¼Œç´€éŒ„æœƒè´çš„æ¬¡æ•¸
        int t = 0;
        for(int j = 0; j < itert[itt]; j++){
            if(randplay(flipPieces(gameboard, player, v[i]), player + 2 % 1)){
                t++;
            }
        }
        if(t > wintimes){//è´æœ€å¤šæ¬¡çš„æœƒè¢«è¨˜éŒ„ä¸‹ä¾†
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
    clock_t startTime = clock(); 
    while(!gameOver(gameboard)){
        if(!passTurn(gameboard, p)){
            if(p == 1){
                isMax = p;
                gameboard = flipPieces(gameboard, p, minimax_Mante(gameboard, p));
            }
            else if(p == 2){
                isMax = p;
                gameboard = flipPieces(gameboard, p, learnHeuristics(gameboard, p,2000));
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
     clock_t endTime = clock(); 
    double totalTime = double(endTime - startTime) / CLOCKS_PER_SEC; 
    cout << "Simulation time: " << totalTime << " seconds" << endl;
}
