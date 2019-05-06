/*
 * main.cpp
 *
 *  Created on: Apr 23, 2018
 *      Author: ellas2
 */

#include <cstdio>
#include <cstring>
#include <iostream>
#include <cassert>
#include "GameManager.h"

#define ROWS 10
#define COLS 10

int main(int argc, char* argv[]){
	if (argc != 2)
	{
		std::cout << "Wrong number of arguments" << endl;
		return 1;
	}
	assert (argc == 2);
	int gameType;
	if (strcmp(argv[1], "auto-vs-auto") == 0)
	{
		gameType = AUTO_VS_AUTO;
	}
	else if (strcmp(argv[1], "auto-vs-file") == 0)
	{
		gameType = AUTO_VS_FILE;
	}
	else if (strcmp(argv[1], "file-vs-file") == 0)
	{
		gameType = FILE_VS_FILE;
	}
	else if (strcmp(argv[1], "file-vs-auto") == 0)
	{
		gameType = FILE_VS_AUTO;
	}
	else
	{
		std::cout << "Wrong game format input" << endl;
		return 0;
	}
	GameManager game(gameType, ROWS, COLS);
	game.manageGame();
	return 0;
}
