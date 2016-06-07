#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

enum {
	ZERO,CROSS,EMPTY,BOUNDARY
};

/*enum{
	HUMANWIN,COMPWIN,DRAW
};*/

const int ConvertTo25[] = {
	6,7,8,
	11,12,13,
	16,17,18};
enum {
	FAIL = -1,FALSE,TRUE
};
int Middle = 4;
int Corner[] = {0,2,6,8};
int Between[] = {1,3,5,7};
const char CharacterMap[] = "OX-|";
const int directions[] = {1,4,5,6};
void InitialiseBoard(int *board)
{
	int index;
	for(index = 0; index < 25; index++)
		board[index] = BOUNDARY;

	for(index = 0;index < 9; index++)
		board[ConvertTo25[index]] = EMPTY;

}
void PrintBoard(const int *board)
{
	int index = 0;
	printf("BOARD:\n");
	for(index = 0; index < 25; index ++)
	{
		if((index != 0)&&(index % 5 == 0))
			printf("\n");
		printf("%c ",CharacterMap[board[index]]);
	}
	printf("\n");
}
int GetHumanMove(const int *board)
{
	char user_input[10];
	int valid_move = 0;
	int move = -1;
	while(!valid_move)
	{
		printf("Please enter a move from 1 to 9 : ");
		gets(user_input);
		if(strlen(user_input) != 1)
		{
			fflush(stdin);
			printf("Error in strlen()\n");
			continue;
		}
		if(!isdigit(user_input[0]))
		{
			printf("Error Not number\n");
			continue;
		}
		if((move = atoi(user_input)) == 0 )
		{
			move = -1;
			printf("Error Out of Range\n");
			continue;
		}
		move--;
		if(board[ConvertTo25[move]] != EMPTY)
		{
			move = -1;
			printf("Square not available\n");
			continue;
		}
		valid_move = 1;
	}
	printf("Making move %d...\n",move+1);
	return ConvertTo25[move];
}
int GetNumForDir(const int *board,int current_square,const int Dir,const int me)
{
	int count = 0;
	while(board[current_square] == me)
	{
		count++;
		current_square += Dir;
	}
	return count;
}
int FindThreeInARow(const int *board,const int square,const int side)
{
	int DirIndex = 0;
	int threecount;	
	int Dir;
	for(DirIndex = 0; DirIndex < 4; DirIndex++)
	{
		threecount = 1;
		Dir = directions[DirIndex];
		threecount += GetNumForDir(board,square + Dir,Dir,side);
		threecount += GetNumForDir(board,square - Dir,-Dir,side);
		if(threecount == 3)
			break;
	}

	if(DirIndex < 4)
		return TRUE;
	else
		return FALSE;	
}
int GetWinningMove(int *board,int side)
{
	int ourMove = -1;
	int index;
	int FoundWinningMove = FALSE;
	for(index = 0; index < 9; index++)
	{
		// Gets an empty square,substitutes side temporarily and checks whether 3 in a row occurs.
		if(board[ConvertTo25[index]] == EMPTY)
		{
			ourMove = ConvertTo25[index];
			board[ourMove] = side;
			if(FindThreeInARow(board,ourMove,side) == TRUE)
				FoundWinningMove = TRUE;
			board[ourMove] = EMPTY;
			if(FoundWinningMove == TRUE)
				break;
			ourMove = -1;
		}
	}
	return ourMove;
}
int GetBlockingMove(int *board,int side)
{
	return GetWinningMove(board,(!side));
}
int GetNextBestMove(const int *board)
{
	if(board[ConvertTo25[Middle]] == EMPTY)
		return ConvertTo25[Middle];

	int move;
	int AvailableMoves[4];
	int NumFree = 0;
	int index;
	for(index = 0; index < 4; index++)
	{
		if(board[ConvertTo25[Corner[index]]] == EMPTY)
			AvailableMoves[NumFree++] = ConvertTo25[Corner[index]];
	}
	if(NumFree == 0)
	{
			for(index = 0; index < 4; index++)
			{
				if(board[ConvertTo25[Between[index]]] == EMPTY)
					AvailableMoves[NumFree++] = ConvertTo25[Between[index]];
			}
	}

	move = rand() % NumFree;
	return AvailableMoves[move];
}
int GetCompMove(int *board,int side)
{
	int move;
	int AvailableMoves[9];
	int NumFree = 0;
	int index;

	// If computer gets a winning move
	if((move = GetWinningMove(board,side)) != FAIL)
		return move;

	// blocking a winning move of player
	if((move = GetBlockingMove(board,side)) != FAIL)
		return move;
	// get next best move
	move = GetNextBestMove(board);
		return move;


}
int HasEmpty(const int *board)
{
	int index;
	for(index = 0; index < 9; index++)
		if(board[ConvertTo25[index]] == EMPTY)
			return 1;
	return 0;
}
void MakeMove(int *board,int square,int side)
{
	board[square] = side;
}
void run_game()
{
	int board[25];
	InitialiseBoard(board);
	PrintBoard(board);
	int GameOver = FALSE;
	int side = ZERO;
	int lastmove;

	while(!GameOver)
	{
		if(side == ZERO)
		{
			//get a move from the human,make the move on the board,change the side
			lastmove = GetHumanMove(board);
			MakeMove(board,lastmove,side);
			side = !side;
		}
		else
		{
			//get a move from the computer,make the move on the board,change the side
			lastmove = GetCompMove(board,side);
			MakeMove(board,lastmove,side);
			side = !side;
			PrintBoard(board);
		}
		// if 3 in a row exist Game is over
		if(FindThreeInARow(board,lastmove,(!side)) == TRUE)
		{
			GameOver = TRUE;
			PrintBoard(board);
			printf("Game Over\n");
			if((!side) == CROSS)
				printf("Computer Wins\n");
			else
				printf("Human Wins\n");
		}
		//if all squares filled, its a draw
		if(!HasEmpty(board))
		{
			GameOver = TRUE;
			printf("Game Over\n");
			printf("Its a DRAW\n");
		}
	}


}
int main()
{
	srand(time(NULL));
	run_game();
	return 0;
}