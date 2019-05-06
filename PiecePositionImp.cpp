/*
 * PiecePositionImp.cpp
 *
 *  Created on: Apr 29, 2018
 *      Author: ellas2
 */

#include "PiecePositionImp.h"

PiecePositionImp::PiecePositionImp(const Point& position, char piece, char jokerRep, int playerNum):
m_position(position.getX(), position.getY()),  m_piece(piece), m_jokerRep(jokerRep), m_playerNum(playerNum){}

char PiecePositionImp::getPiece() const
{
	return m_piece;
}
char PiecePositionImp::getJokerRep() const  // ONLY for Joker: R, P, S or B -- non-Joker may return ‘#’
{
	if (getPiece() == 'J')
		return m_jokerRep;
	else
		return JOKEREP_DEFAULT;
}
const Point& PiecePositionImp::getPosition() const
{
	return m_position;
}


const PointImp& PiecePositionImp::getPositionImp() const
{
	return m_position;
}


void PiecePositionImp::movePosition(const Point& moveTo)
{
	m_position = moveTo;
}

PiecePositionImp& PiecePositionImp::operator=(const PiecePositionImp& rhs)
{

	if (this == &rhs)
		return *this;
	m_piece = rhs.getPiece();
	m_jokerRep = rhs.getJokerRep();
	m_position = rhs.getPosition();
	m_playerNum = rhs.getPlayerNum();
	return *this;
}

void PiecePositionImp::setPiece(char newPiece)
{
	m_piece = newPiece;
}

void PiecePositionImp::setjokerRep(char newJokerRep)
{
	m_jokerRep = newJokerRep;
}

int PiecePositionImp::getPlayerNum()const
{
	return m_playerNum;
}

void PiecePositionImp::setPlayerNum(int newPlayerNum)
{
	m_playerNum = newPlayerNum;
}

PiecePositionImp::PiecePositionImp(const PiecePositionImp& piecePositionImp) : m_position(piecePositionImp.getPositionImp())
{
	m_piece = piecePositionImp.getPiece();
	m_jokerRep = piecePositionImp.getJokerRep();
	m_playerNum = piecePositionImp.getPlayerNum();
}
