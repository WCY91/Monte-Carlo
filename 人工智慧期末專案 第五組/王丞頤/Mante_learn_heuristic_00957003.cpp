#include <iostream>
#include <string>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

string playerR = "XO"; 

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

int heuristicScore_wcy(string gameboard, int player){ //�p������W���֪��Ѥl�h (�Ӳv 
    return countColorPieces_wcy(gameboard, player) - countColorPieces_wcy(gameboard, 3-player);
}

bool simulate(string gameboard, int players){ 
	int player = 3-players; //�ѩ�i�J�����e�|����player�U�@�B�]���������}�Y�ѹ��}�l 
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
		int choose =  rand() % validMoves.size();//�H����@�ӦX�z���B 
		string newGameboard = gameboard;
		newGameboard = flipPieces_wcy(newGameboard, player, validMoves[choose]); 
 		if (simulate(newGameboard, (player))) { //������ 
            wins++;
        }
        scores[validMoves[choose]] += wins;
	}
    // ��ܳӲv�̰�������
    int maxScore = INT_MIN;
    int bestMove = -1;
    for (int i = 0; i < 36; i++) {
        if (scores[i] > maxScore && isValidMove_wcy(gameboard, player, i)) {
            maxScore = scores[i];
            bestMove = i;
        }
    }
//    gameboard = flipPieces_wcy(gameboard, player, bestMove); // ��s�C������
    return bestMove; // ��^�̨β���
}


void ai_1000VsAi_2000() { //����1000��M����2000�� 
	string gameboard = "++++++++++++++OX++++XO++++++++++++++";
	int turn=1;  //����1000�����a���� 
	//�C�L�ثe���ѽL���A 
	cout << "��l�ѽL"<<endl;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			cout << gameboard[i * 6 + j] << " ";
		}
		cout << endl;
	} 
	int choose = 1;
	while (!gameOver_wcy(gameboard)) {
		if((turn%2) == 1){ //1000���a���^�X 
			if(!passTurn_wcy(gameboard, choose)){
		     	int nextMove = learnHeuristics(gameboard, choose,1000);
		        if (nextMove != -1) {
		            gameboard = flipPieces_wcy(gameboard, choose, nextMove);
		        } else {
		            cout << "in 1000 ai can't play" << endl;
		            break; // �S�����Ī����ʡA�����C��
		        }
		        //�C�L�ثe���ѽL���A 
		    	cout << "�g�L�q��(1000)�U���᪺�ѽL"<<endl;
				for (int i = 0; i < 6; i++) {
			        for (int j = 0; j < 6; j++) {
			            cout << gameboard[i * 6 + j] << ' ';
			        }
			        cout << endl;
			    }
			}else{
				cout << "�ثe�L�i�H�U���]�����q��(2000)�b�U�@�^�X" << endl; 
			}
		}else{ //�q�����^�X 
			if(!passTurn_wcy(gameboard, (3-choose))){
			    int nextMove = learnHeuristics(gameboard, (3-choose),2000);
		        if (nextMove != -1) {
		            gameboard = flipPieces_wcy(gameboard, (3-choose), nextMove);
		        } else {
		            cout << "in 2000 ai can't play" << endl;
		            break; // �S�����Ī����ʡA�����C��
		        }
		        //�C�L�ثe���ѽL���A 
		    	cout << "�g�L�q��(2000)�U���᪺�ѽL"<<endl;
				for (int i = 0; i < 6; i++) {
			        for (int j = 0; j < 6; j++) {
			            cout << gameboard[i * 6 + j] << ' ';
			        }
			        cout << endl;
			    }
			}else{
				cout << "�ثe�L�i�H�U���]�����q��(1000)���^�X" << endl; 
			}
		}
		turn++;
	}
	int aiPieces_1000 = countColorPieces_wcy(gameboard, choose);
	int aiPieces_2000 = countColorPieces_wcy(gameboard, (3-choose));
	if(aiPieces_1000 > aiPieces_2000){
		cout << "����1000��ӨåBĹ�F" << (aiPieces_1000 - aiPieces_2000) << "�ӴѤl" << endl; 
	}else if(aiPieces_1000 == aiPieces_2000){
		cout << "���ߪ��a�P�q����������" << endl; 
	}else{
		cout << "����2000��ӨåBĹ�F" << (aiPieces_2000-aiPieces_1000 ) << "�ӴѤl" << endl; 
	}
}

void playerVsAI() {
	string gameboard = "++++++++++++++OX++++XO++++++++++++++";
	cout << "�п�ܬ��¤l�Υդl(�¤l������ 1:�¤l 2: �դl" << endl;
	int choose  = 0;
	cin >> choose;
	int flag = 0;
	if(choose == 1){
		flag = 1;
	}else{
		flag=0;
	}
	vector<int> validPlayerMoves;
	int turn=1; //���a���� 
	//�C�L�ثe���ѽL���A 
	cout << "��l�ѽL"<<endl;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			cout << gameboard[i * 6 + j] << " ";
		}
		cout << endl;
	} 
	while (!gameOver_wcy(gameboard)) {
		if((turn%2) == flag){ //���a���^�X 
			if(!passTurn_wcy(gameboard, choose)){
		    	validPlayerMoves.clear();
		    		
			    for (int i = 0; i < 36; i++) {
			        if (isValidMove_wcy(gameboard, 1, i)) {
			            validPlayerMoves.push_back(i);
			        }
			    }
			    int playerChoose;
			    cout << "�Ъ��a��ܤU�C�i�����ѨB" << endl; 
			    for(int i=0;i<validPlayerMoves.size();i++){
					cout << "choose(" << i+1 << "): " << validPlayerMoves[i] << "  "; 
				}
				cout << endl;
				cin >> playerChoose;
				gameboard = flipPieces_wcy(gameboard, choose,validPlayerMoves[playerChoose-1]);
				//�C�L�ثe���ѽL���A 
		    	cout<< "�g�L���a�U���᪺�ѽL" << endl;
				for (int i = 0; i < 6; i++) {
			        for (int j = 0; j < 6; j++) {
			            cout << gameboard[i * 6 + j] << ' ';
			        }
			        cout <<endl;
			   }	
			}else{
				cout << "���a�ثe�L�i�H�U���]�����q���b�U�@�^�X" << endl; 
			}
		}else{ //�q�����^�X 
			if(!passTurn_wcy(gameboard, (3-choose))){
			    int nextMove = learnHeuristics(gameboard, (3-choose),2000);
		        if (nextMove != -1) {
		            gameboard = flipPieces_wcy(gameboard, (3-choose), nextMove);
		        } else {
		            cout << "in ai can't play" << endl;
		            break; // �S�����Ī����ʡA�����C��
		        }
		        //�C�L�ثe���ѽL���A 
		    	cout << "�g�L�q���U���᪺�ѽL"<<endl;
				for (int i = 0; i < 6; i++) {
			        for (int j = 0; j < 6; j++) {
			            cout << gameboard[i * 6 + j] << ' ';
			        }
			        cout << endl;
			    }
			}else{
				cout << "�q�����a�ثe�L�i�H�U���]�������a���^�X" << endl; 
			}
		}
		turn++;
	}
	int playerPieces = countColorPieces_wcy(gameboard, choose);
	int aiPieces = countColorPieces_wcy(gameboard, (3-choose));
	if(playerPieces > aiPieces){
		cout << "���ߪ��a��ӨåBĹ�F" << (playerPieces - aiPieces) << "�ӴѤl" << endl; 
	}else if(playerPieces == aiPieces){
		cout << "���ߪ��a�P�q����������" << endl; 
	}else{
		cout << "�ܤ����q����ӨåBĹ�F" << (aiPieces-playerPieces ) << "�ӴѤl" <<"�Ъ��a�A���A�F"<< endl; 
	}
}



int main(){
    //OO++++XOOXXX+XOXX++OXXOO+XXOX++XXXX+
    srand(time(NULL));
   
    playerVsAI();
//	ai_1000VsAi_2000();
}




