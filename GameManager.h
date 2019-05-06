/*
 * GameManager.h
 *
 *  Created on: May 7, 2018
 *      Author: ellas2
 */

#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_
#define OUT_FILE_PATH "./rps.output"
#define AUTO_VS_FILE 1
#define FILE_VS_AUTO 2
#define AUTO_VS_AUTO 3
#define FILE_VS_FILE 4
#define PLAYER_ONE 1
#define PLAYER_TWO 2
#define NO_PLAYER 0
#define PLAYER_ONE_WINS_FIGHT 1
#define PLAYER_TWO_WINS_FIGHT 2
#define NO_PLAYER_WINS_FIGHT 0
#include <memory>
#include <string>
#include <string.h>
#include <iostream>
#include "FilePlayerAlgorithm.h"
#include "AutoPlayerAlgorithm.h"
#include "BoardImp.h"
#define TAKEN_SAME 1
#define TAKEN_OTHER 2
#define EMPTY 3

using namespace std;

typedef struct {
	int rocks = 0;
	int papers = 0;
	int scissors = 0;
	int bombs = 0;
	int jokers = 0;
	int flags = 0;
	int jokerBombs = 0;
} pieceCount;


typedef enum rsp_game_message{
	RPSFileFailure,
	RPSWrongPiece,
	RPSNotAJokerPiece,
	RPSTooManyPiecesOfSomeSort,
	RPSNotEnoughFlags,
	RPSLocationIsTaken,
	RPSTargetLocationContainsPlayerPiece,
	RPSTooManyJokers,
	RPSNoFlags,
	RPSWrongInputLine,
	RPSWrongDestCoords,
	RPSWrongSourceCoords,
	RPSSourceLocationDoesntContainsPlayerPiece,
	RPSLocationIsEmpty,
	RPSInvalidPieceMove,
	RPSPieceCannotMove,
	RPSBothPlayersNotEnoughFlags,
	RPSPlayer1NotEnoughFlags,
	RPSPlayer2NotEnoughFlags,
	RPSTieNoFlags,
	RPSTieNoMovingPieces,
	RPSPlayer1WonNoOpponentFlags,
	RPSPlayer1WonNoOpponentMovingPieces,
	RPSPlayer2WonNoOpponentFlags,
	RPSPlayer2WonNoOpponentMovingPieces,
	RPSWrongMoveLinePlayer1,
	RPSWrongMoveLinePlayer2,
	RPSSuccess
} RPSMessage;

void printMSG(RPSMessage msg);
bool isBadPositioning(RPSMessage msg);
bool isWinnerRelated(RPSMessage msg);

class GameManager {
public:
	GameManager(int gameType, int rows, int cols);
	bool GetPiecePositioning();
	//Checks if the point is in range and the piece and jokerRep are as expected
	RPSMessage isValidPiecePosition(const PiecePosition& piecePos);
	//
	//bool isTooManyPieces(const PiecePosition& piecePos, int playerNum);
	RPSMessage UpdateBoard(std::vector<unique_ptr<PiecePosition>>& newPositionsVec, int playerNum, std::vector<unique_ptr<FightInfo>>& fights);
	void addPiecePosAndUpdatePieceCount(const PiecePosition& piecePos, int playerNum);
	void executeFight(const PiecePosition& firstPlayerPiecePos, const PiecePosition& secondPlayerPiecePos, std::vector<unique_ptr<FightInfo>>& fights, int playerNumOnBoard);
	void removePiecePosAndUpdatePieceCount(const PiecePosition& piecePos, int playerNum);
	char getFightChar(const PiecePosition& piecePos);
	RPSMessage checkWinner();
	void switchCurrentPlayer();
	bool hasMovingPieces(pieceCount player){return (player.jokers + player.papers +
			player.scissors + player.rocks - player.jokerBombs) != 0 ? true : false;};
	RPSMessage loadAndExecuteMoves();
	RPSMessage loadAndExecuteJokerChange();
	RPSMessage loadAndExecuteSingleMove();
	void printBoardToFile(ostream& fout);
	void printGameOutputToFile(string reason, int winner, bool isBadPositioning);
	void executeSingleFight(const PiecePosition& firstPlayerPiecePos, const PiecePosition& secondPlayerPiecePos);
	void manageGame();
	void printWinnerToFileByMsg(RPSMessage msg);
	void appendToPieceCount(const PiecePosition& piecePos, pieceCount& pieceCount);
	bool isTooManyPieces(const PiecePosition& piecePos, pieceCount& pieceCount);
	~GameManager() = default;

private:
	int counter = 0;
	std::shared_ptr<PlayerAlgorithm> m_firstPlayerAlg;
	std::shared_ptr<PlayerAlgorithm> m_secondPlayerAlg;
	std::shared_ptr<PlayerAlgorithm> m_currentPlayerAlg;
	BoardImp m_boardImp;
	pieceCount player1PieceCount;
	pieceCount player2PieceCount;
};

#endif /* GAMEMANAGER_H_ */
