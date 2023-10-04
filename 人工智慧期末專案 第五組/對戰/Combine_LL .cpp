#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <ctime>
#define max_h 36
#define min_h -36
using namespace std;
double start, end;
int jacky_countPieces(vector<vector<int> > boardArr, int player);
int jacky_heuristic(vector<vector<int> > boardArr);
int jacky_minimax(vector<vector<int> > boardArr, int depth, int alpha, int beta, int player);
int jacky_maxPlayer, jacky_minPlayer;
string playerColor[3] = {"", "black", "white"};
void jacky_FlipPieces(vector<vector<int> >& boardArr, int player, pair<int,int> position);
void printBoard(vector<vector<int> >& boardArr);
pair<int, int> direction[] = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
vector<pair<int, int> > jacky_availableMove(vector<vector<int> > boardArr, int player);
pair<int, int> jacky_nextMove(vector<vector<int> >, int, int);
bool jacky_gameover(vector<vector<int> >);
string jacky_boardArrtoS(vector<vector<int> > boardArr);
vector<vector<int> > jacky_boardStoArr(string gameBoard);
//above is jacky's-----------------------------------------------------------------------------------------
typedef pair<int, int> node;    //position, score
int moveX[8] = {0, 1, 1, 1, 0, -1, -1, -1}; //´Ñ½L8¤è¦VªºXY®y¼ÐÅÜ¤Æ
int moveY[8] = {-1, -1, 0, 1, 1, 1, 0, -1}; //
string playerR = "XO";  //ª±®a1¬O¶ÂX¡Aª±®a2¬O¥ÕO
int isMax;  //çŽ©å®¶
int itert[9] = {600, 700, 800, 900, 1000, 1100, 1200, 1300, 2000};  //´ú¸Õ¥Î¡A­¡¥N¦¸¼Æ•¸
int itt = 0;    //«ü©w­n¶]¦h¤Ö¦¸

bool inBoard(int row, int col);
int countFlipPieces(string gameboard, int player, int grid, int dir);
bool isValidMove(string gameboard, int player, int grid);
bool passTurn(string gameboard, int player);
bool gameOver(string gameboard);
string flipPieces(string gameboard, int player, int grid);
int countColorPieces(string gameboard, int player);
int heuristicScore(string gameboard, int player);
node minimax(string gameboard, int depth, int alpha, int beta, int player); 
node randminmax(string gameboard, int depth, int player);   //·íminimaxªºµ²ªG¦³½Æ¼Æ¦ì¸m¦P¤À®É¡AÀH¾÷¿ï¨ú¤@­Ó¦ì¸m¦^¶Ç¡C
bool randplay(string gameboard, int player);        //ÀH¾÷¤U¤l¨ì´Ñ§½µ²§ô¡A­Yª±®aÀò³Ó¦^¶ÇTrue¡A§_«h¦^¶ÇFalse
int Mante(string gameboard, int player);
void testManteRuntime();
int minimax_Mante(string gameboard, int player);
void test();

//above is Mr. Lin's-----------------------------------------------------------------------------------------


int main()
{
	srand(time(NULL));
	string board = "++++++++++++++XO++++OX++++++++++++++", gameboard = "++++++++++++++OX++++XO++++++++++++++";
	int player=1, tmp, depth, breakVal=0;
	double playerTimeSpent[3] = {0,0,0};
	clock_t start, stop;
	vector<pair<int, int> > ablemove;
	vector<vector<int> > boardArr(6);
	pair<int, int> MoveInt, endPair = make_pair(-1,-1);
	for(int j=0; j<board.length(); j++)
	{
		if(board[j]=='X')
		tmp=1;
		else if(board[j]=='O')
		tmp=2;
		else
		tmp=0;
		boardArr[j/6].push_back(tmp);
	}
	
	int p = 1, lin=1, isMax=1, jacky=2;//who's player black | alter p along with isMax
    itt = 4;
	cout<<"input depth:"; cin>>depth;
	cout<<"itt times:"; cin>>itt;
    
    
	while(true)
	{
		if(breakVal==1)
			break;
			
		start = clock();
		if(player == 1)
		{
			if(gameOver(gameboard))
			{
				cout<<"gameover from Lin"<<endl;
				breakVal=1;
				break;
			}
			if(!passTurn(gameboard, p)){
			gameboard = flipPieces(gameboard, p, minimax_Mante(gameboard, p));
			boardArr = jacky_boardStoArr(gameboard);
			} 
		}
		else if(player == 2)
		{ 
			if(jacky_gameover(boardArr))
			{
				cout<<"gameover from jacky"<<endl;
//				breakVal = 1;
//				break;
			}
            MoveInt = jacky_nextMove(boardArr, player, depth);
			if(MoveInt!=endPair)
			{
				jacky_FlipPieces(boardArr, player, MoveInt);
				gameboard = jacky_boardArrtoS(boardArr);
			}
        }
        stop = clock();
        playerTimeSpent[player] += double(stop - start) / CLOCKS_PER_SEC;
		player = 3-player;//3-1=2 3-2=1
		printBoard(boardArr);
	}
	
	
	printBoard(boardArr);
	if( (jacky_countPieces(boardArr, 1)-jacky_countPieces(boardArr, 2)) > 0)
	
		cout<<"black wins"<<endl;
	else
		cout<<"white wins"<<endl;
	cout<<"black wins"<<(jacky_countPieces(boardArr, 1)-jacky_countPieces(boardArr, 2))<<"pieces"<<endl;
	cout<<"black spent="<<playerTimeSpent[1]<<endl;
	cout<<"white spent="<<playerTimeSpent[2]<<endl;
	return 0;
}

pair<int, int> jacky_nextMove(vector<vector<int> > boardArr, int player, int depth)
{
	vector<pair<int, int> > ablemove;
	vector<vector<int> > tmpBoardArr(6);
	vector<pair<int, int> > bestMoveInt;
	int best_hscore = min_h, tmp_hscore, opponent, randomnum;

	jacky_maxPlayer = player; 
	(player==1)? jacky_minPlayer=2 : jacky_minPlayer=1;
	(player==1) ? (opponent=2) : (opponent=1);
	ablemove = jacky_availableMove(boardArr, player);
	if(ablemove.empty())
		return make_pair(-1,-1);
		
	for(int j=0; j<ablemove.size(); j++)
	{		
//		cout<<playerColor[player]<<" move "<<"("<<ablemove[j].first<<","<<ablemove[j].second<<")"<<endl;		
		tmpBoardArr = boardArr;
		jacky_FlipPieces(tmpBoardArr, player, ablemove[j]);
		tmp_hscore = jacky_minimax(tmpBoardArr, depth-1, min_h, max_h, opponent);
		if(tmp_hscore>best_hscore)
		{
			bestMoveInt.clear();
			bestMoveInt.push_back(ablemove[j]);
			best_hscore = tmp_hscore;
		}
		else if(tmp_hscore==best_hscore)
		{
			bestMoveInt.push_back(ablemove[j]);
		}
	}
	randomnum = rand()%bestMoveInt.size();
	return bestMoveInt[randomnum];
}

bool jacky_gameover(vector<vector<int> > boardArr)
{
	vector<pair<int, int> > ablemove1, ablemove2;
	ablemove1 = jacky_availableMove(boardArr, 1);
	ablemove2 = jacky_availableMove(boardArr, 2);
	if(ablemove1.empty() && ablemove1.empty())
		return true;
	else
		return false;
}
int jacky_minimax(vector<vector<int> > boardArr, int depth, int alpha, int beta, int player)
{
	if(depth==0)
		return jacky_heuristic(boardArr);
	
	vector<vector<int> > tmpBoardArr;
	vector<pair<int, int> > ablemove = jacky_availableMove(boardArr, player);
	if(ablemove.empty())
	{
		int opponent;
		(player==1) ? opponent=2  : opponent=1;
		return jacky_minimax(boardArr, depth-1, alpha, beta, opponent);
	}
	
	int tmpEval;
	if(player==jacky_maxPlayer)
	{
		int maxEval = min_h;
		for(int i=0; i<ablemove.size(); i++)
		{
			tmpBoardArr = boardArr;
			jacky_FlipPieces(tmpBoardArr, player, ablemove[i]);
			tmpEval = jacky_minimax(tmpBoardArr, depth-1, alpha, beta, jacky_minPlayer);
			maxEval = max(maxEval, tmpEval);
			alpha = max(alpha, tmpEval);
			if(alpha>=beta)
				break;
		}
		return maxEval;
	}
	else
	{
		int minEval = max_h;
		for(int i=0; i<ablemove.size(); i++)
		{
			tmpBoardArr = boardArr;
			jacky_FlipPieces(tmpBoardArr, player, ablemove[i]);
			tmpEval = jacky_minimax(tmpBoardArr, depth-1, alpha, beta, jacky_maxPlayer);
			minEval = min(minEval, tmpEval);
			beta = min(beta, tmpEval);
			if(alpha>=beta)
				break;
		}
		return minEval;
	}
}
void jacky_FlipPieces(vector<vector<int> >& boardArr, int player, pair<int,int> position)
{
	int Filpable=0, FlipPieces=0, posRow=position.first, posCol=position.second, currentRow, currentCol;
	char ch;
	string newBoard;
	boardArr[posRow][posCol] = player;
	for(int dir=0; dir<8; dir++)
	{
		currentRow = posRow + direction[dir].first;
		currentCol = posCol + direction[dir].second;
		while( currentRow>=0 && currentCol>=0 && currentRow<=5 && currentCol<=5 )
		{
			if(boardArr[currentRow][currentCol]==0)
			break;
			else if(boardArr[currentRow][currentCol]==player)
			{
				while(currentRow!=posRow || currentCol!=posCol)
				{
					boardArr[currentRow][currentCol] = player;
					currentRow -=  direction[dir].first;
					currentCol -=  direction[dir].second;
				}
				break;
			}
			currentRow = currentRow + direction[dir].first;
			currentCol = currentCol + direction[dir].second;
		}
	}
}
vector<pair<int, int> > jacky_availableMove(vector<vector<int> > boardArr, int player)
{
	int skip=0, flipPieces, opponent, currentRow, currentCol;
	(player==1) ? (opponent=2) : (opponent=1);
	vector<pair<int, int> > ablemove;
	for(int row=0; row<6; row++)
	{
		for(int col=0; col<6; col++)
		{
			if(boardArr[row][col]!=0)
			continue;
			else
			{
				skip=0;
				for(int dir=0; dir<8; dir++)
				{
					flipPieces=0; 
					if(skip==1) break;
					currentRow = row + direction[dir].first;
					currentCol = col + direction[dir].second;
					while( currentRow>=0 && currentCol>=0 && currentRow<=5 && currentCol<=5 )
					{
						if(boardArr[currentRow][currentCol]==opponent)
						flipPieces++;
						else if(boardArr[currentRow][currentCol]==player)
						{
							if(flipPieces!=0)
							{
								ablemove.push_back( make_pair(row, col) );
								skip=1;
							}		
							break;
						}
						else
						break;
						currentRow +=  direction[dir].first;
						currentCol +=  direction[dir].second;
					}
				}
			}
		}
	}
	return ablemove;
}
int jacky_countPieces(vector<vector<int> > boardArr, int player)
{
	int playerPieces=0;
	for(int i=0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			if(boardArr[i][j]==player)
			playerPieces++;
		}
	}
	return playerPieces;
}
int jacky_heuristic(vector<vector<int> > boardArr)
{
	return jacky_countPieces(boardArr, jacky_maxPlayer) - jacky_countPieces(boardArr, jacky_minPlayer);
}
void printBoard(vector<vector<int> >& boardArr)
{
	SetConsoleOutputCP(CP_UTF8); 
	char ch;
	cout<<"-------------------"<<endl;
	cout<<"  a b c d e f"<<endl;
	for(int i=0; i<6; i++)
	{
		ch=i+'A'; cout<<ch<<" ";
		for(int j=0; j<6; j++)
		{
			if(boardArr[i][j]==1)
			cout<<u8"\u25CB ";
			else if(boardArr[i][j]==2)
			cout<<u8"\u25CF ";
			else
			cout<<"_ ";
		} 
		cout<<endl;
	}
	cout<<"-------------------"<<endl;
}

vector<vector<int> > jacky_boardStoArr(string gameBoard)
{
	vector<vector<int> > boardArr(6);
	int tmp;
	for(int j=0; j<gameBoard.length(); j++)
	{
		if(gameBoard[j]=='X')
		tmp=1;
		else if(gameBoard[j]=='O')
		tmp=2;
		else
		tmp=0;
		boardArr[j/6].push_back(tmp);
	}
	return boardArr;
}
string jacky_boardArrtoS(vector<vector<int> > boardArr)
{
	string gameBoard;
	char black = 'X', white =  'O' , grid = '+';
	for(int i=0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			if(boardArr[i][j]==1)
			gameBoard+=black;
			else if(boardArr[i][j]==2)
			gameBoard+=white;
			else
			gameBoard+=grid;
		} 
	}
	return gameBoard;
}

//below is Mr.Lin's-----------------------------------------------------------------------------------------------------------

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
    for(int i = 0; i < 36; i++){    //§ä¨ì·í«e´Ñ½L©Ò¦³¯à¤U¤lªº¦ì¸m
        if(isValidMove(gameboard, player, i)){
            v.push_back(i);
        }
    }
    int wintimes = 0;
    int gowhere = 0;
    for(int i = 0; i < v.size(); i++){//¨C­Ó¯à¤U¤lªº¦ì¸m³£­¡¥Nn¦¸¡A¬ö¿ý·|Ä¹ªº¦¸¼Æ
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
