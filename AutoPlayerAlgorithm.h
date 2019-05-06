/*
 * AutoPlayerAlgorithm.h
 *
 *  Created on: Apr 28, 2018
 *      Author: yevkirich
 */

#ifndef AUTOPLAYERALGORITHM_H_
#define AUTOPLAYERALGORITHM_H_

#include "PlayerAlgorithm.h"
#include "PointImp.h"
#include "PiecePositionImp.h"
#include "Board.h"
#include "FightInfoImp.h"
#include "MoveImp.h"
#include "JokerChangeImp.h"

using std::unique_ptr;
using std::vector;

#define M 10
#define N 10
#define R 2
#define P 5
#define S 1
#define B 2
#define J 2
#define F 1
//--------------------------

typedef struct {
	int rocks = R;
	int papers = P;
	int scissors = S;
	int bombs = B;
	int jokers = J;
	int flags = F;
} piecesCount;

class AutoPlayerAlgorithm : public PlayerAlgorithm{
public:
	//AutoPlayerAlgorithm();
	void getInitialPositions(int player, vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	void notifyOnInitialBoard(const Board& b, const vector<unique_ptr<FightInfo>>& fights) override;
	void notifyOnOpponentMove(const Move& move) override; // called only on opponent’s move
	void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	unique_ptr<Move> getMove() override;
	int getMoveScore(const PiecePositionImp& piece, const PointImp& moveTo);
	bool posIsTaken(const Point& point);
	bool legalPoint(const PointImp& point);
	unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
	void removePointFromOppInitial(const Point& point);
	void removePieceLoc(vector<PiecePositionImp>& pieces, const Point& point, bool onlyIfBomb);
	~AutoPlayerAlgorithm() override {};
private:
	vector<PiecePositionImp> myPieces;
	vector<PiecePositionImp> opponentUncovered;
	vector<PointImp> opponentInitialPositions;
	int playerNum = 0;
	int oppPlayerNum = 0;
	piecesCount opponentPieces;
	piecesCount myPiecesCount;
};



#endif /* AUTOPLAYERALGORITHM_H_ */
