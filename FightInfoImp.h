/*
 * FightInfoImp.h
 *
 *  Created on: May 2, 2018
 *      Author: ellas2
 */

#ifndef FIGHTINFOIMP_H_
#define FIGHTINFOIMP_H_

#include "FightInfo.h"
#include "PointImp.h"

class FightInfoImp : public FightInfo {
public:
	FightInfoImp(const PointImp& pos, char player1Piece, char player2Piece, int winner = 0);
	const Point& getPosition() const override;
	char getPiece(int player) const override; // R, P, S, B or F (but NOT J)
	int getWinner() const override; // 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
	void setWinner(int newWinner);
	void setPiece(char piece, int player);
	~FightInfoImp() = default;
private:
	  PointImp m_pos;
	  char m_player1Piece;
	  char m_player2Piece;
	  int m_winner;
};

#endif /* FIGHTINFOIMP_H_ */
