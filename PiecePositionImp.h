/*
 * PiecePositionImp.h
 *
 *  Created on: Apr 29, 2018
 *      Author: ellas2
 */

#ifndef PIECEPOSITIONIMP_H_
#define PIECEPOSITIONIMP_H_
#define JUNK '$'
#define JOKEREP_DEFAULT '#'
#include "PiecePosition.h"
#include "PointImp.h"


class PiecePositionImp : public PiecePosition {
public:
	PiecePositionImp(const Point& position, char piece = JUNK, char jokerRep = JOKEREP_DEFAULT, int playerNum = 0);
	char getPiece() const override; // R, P, S, B, J or F
	char getJokerRep() const override; // ONLY for Joker: R, P, S or B -- non-Joker may return ‘#’
	const Point& getPosition() const override;
	const PointImp& getPositionImp() const;
	int getPlayerNum() const;
	void setPiece(char newPiece);
	void setjokerRep(char newJokerRep);
	void movePosition(const Point& moveto);
	void setPlayerNum(int newPlayerNum);
	PiecePositionImp(const PiecePositionImp& piecePositionImp);
	PiecePositionImp& operator=(const PiecePositionImp& rhs);
	~PiecePositionImp() = default;
private:
	PointImp m_position;
	char m_piece;
	char m_jokerRep;
	int m_playerNum;
};

#endif /* PIECEPOSITIONIMP_H_ */
