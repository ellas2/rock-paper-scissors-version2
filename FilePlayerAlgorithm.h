/*
 * FilePlayerAlgorithm.h
 *
 *  Created on: May 7, 2018
 *      Author: ellas2
 */

#ifndef FILEPLAYERALGORITHM_H_
#define FILEPLAYERALGORITHM_H_
#include "PlayerAlgorithm.h"
#include "PiecePositionImp.h"
#include "JokerChangeImp.h"
#include "MoveImp.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>
#include <memory>
using std::unique_ptr;
using std::vector;
#define player_one_positioning_file_path "./player1.rps_board"
#define player_two_positioning_file_path "./player2.rps_board"
#define player_one_moves_file_path "./player1.rps_moves"
#define player_two_moves_file_path "./player2.rps_moves"
using namespace std;

class FilePlayerAlgorithm : public PlayerAlgorithm{
public:
	FilePlayerAlgorithm();
	void getInitialPositions(int player, vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	void notifyOnInitialBoard(const Board& b, const vector<unique_ptr<FightInfo>>& fights) override;
	void notifyOnOpponentMove(const Move& move) override; // called only on opponent’s move
	void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	unique_ptr<Move> getMove() override;
	unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
	~FilePlayerAlgorithm();

private:
	JokerChangeImp jokerChange;
	bool needToChangeJoker = false;
	bool isFileDone = false;
	ifstream fin;
};

#endif /* FILEPLAYERALGORITHM_H_ */
