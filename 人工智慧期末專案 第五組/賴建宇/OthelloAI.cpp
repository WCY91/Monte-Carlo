#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <ctime>
#define max_h 36
#define min_h -36 //white 17 avg 29      black 12  avg 24        tie 1
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
int main()
{
	srand(time(NULL));
	string board = "++++++++++++++XO++++OX++++++++++++++";
	int currentplayer=1, tmp, depth, playerSide, validmove;
	double playerTimeSpent[3] = {0,0,0};
	clock_t start, stop;
	vector<pair<int, int> > ablemove;
	vector<vector<int> > boardArr(6);
	pair<int, int> MoveInt, endPair = make_pair(-1,-1);
	string tmpEnter;
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
	cout<<"choose your side. enter 1 for black, enter 2 for white:"; cin>>playerSide;
	cout<<"choose your opponent level:"; cin>>depth;
	while(true)
	{
		if(jacky_gameover(boardArr))
			break;
		if(jacky_availableMove(boardArr, currentplayer).empty())
		{
			if(currentplayer==playerSide)
			cout<<"you have nothing to move;"<<endl;
			else
			cout<<"opponent has nothing to move, so it's your turn again."<<endl;
			currentplayer = 3-currentplayer;//3-1=2 3-2=1
		}
		printBoard(boardArr);
		if(currentplayer==playerSide)
		{
			vector<pair<int, int> > ablemove = jacky_availableMove(boardArr, currentplayer);
			cout<<"enter your next move(enter row first then colnum):"<<endl;
			cin>>tmpEnter;
			while(true)
			{
				validmove=0;
				for(int i=0; i<ablemove.size(); i++)
				{
					if( ablemove[i] ==  make_pair( (int)(tmpEnter[0]-'A') , (int)(tmpEnter[1]-'a') ))
						validmove=1;
				}
				if(tmpEnter.length()!=2 || (tmpEnter[0]-'A')<0 ||  (tmpEnter[0]-'A')>5 || (tmpEnter[1]-'a')<0 ||  (tmpEnter[1]-'a')>5)
					cout<<"invalid enter(invalid input type), enter again:"<<endl;
				else if(validmove==0)
					cout<<"invalid enter(unavailable move), enter again:"<<endl;
				else
					break;
				cin>>tmpEnter;
			} 
			MoveInt = make_pair( (int)(tmpEnter[0]-'A') , (int)(tmpEnter[1]-'a') );
			cout<<"your move is "<<tmpEnter<<endl;
			jacky_FlipPieces(boardArr, currentplayer, MoveInt);
		}
		else
		{
			MoveInt = jacky_nextMove(boardArr, currentplayer, depth);
			jacky_FlipPieces(boardArr, currentplayer, MoveInt);
			cout<<"your opponent move is "<<(char)(MoveInt.first+'A')<<(char)(MoveInt.second+'a')<<endl;
		}

		currentplayer = 3-currentplayer;//3-1=2 3-2=1
	}
	cout<<endl<<"game over, final result is"<<endl;
	printBoard(boardArr);
	if( (jacky_countPieces(boardArr, 1)-jacky_countPieces(boardArr, 2)) > 0)
	{
		cout<<"black wins"<<endl;
		cout<<"black wins "<<(jacky_countPieces(boardArr, 1)-jacky_countPieces(boardArr, 2))<<" pieces"<<endl;
	} 
	else if((jacky_countPieces(boardArr, 1)-jacky_countPieces(boardArr, 2)) < 0)
	{
		cout<<"white wins"<<endl;
		cout<<"white wins "<<(jacky_countPieces(boardArr, 2)-jacky_countPieces(boardArr, 1))<<" pieces"<<endl;
	} 
	else
	{
		cout<<"tie"<<endl;
	}
	 	
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
	if(ablemove1.empty() && ablemove2.empty())
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

