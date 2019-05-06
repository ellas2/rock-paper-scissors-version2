/*
 * FightInfoImp.cpp
 *
 *  Created on: May 2, 2018
 *      Author: ellas2
 */

#include "FightInfoImp.h"

FightInfoImp::FightInfoImp(const PointImp& pos, char player1Piece, char player2Piece, int winner) :
m_pos(pos), m_player1Piece(player1Piece), m_player2Piece(player2Piece), m_winner(winner){}

const Point& FightInfoImp::getPosition() const
{
	return m_pos;
}
char FightInfoImp::getPiece(int player) const // R, P, S, B or F (but NOT J)
{
	if (player == 1)
		return m_player1Piece;
	else // player == 2
		return m_player2Piece;
}
int FightInfoImp::getWinner() const // 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
{
	return m_winner;
}


void FightInfoImp::setWinner(int newWinner)
{
	m_winner = newWinner;
}

void FightInfoImp::setPiece(char piece, int player)
{
	if (player == 1)
		m_player1Piece = piece;
	else //player == 2
		m_player2Piece = piece;
}

