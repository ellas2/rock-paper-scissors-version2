/*
 * BoardImp.cpp
 *
 *  Created on: May 2, 2018
 *      Author: ellas2
 */

#include "BoardImp.h"
#include <iostream>

BoardImp::BoardImp(int rows, int cols) : m_rows(rows), m_cols(cols) {
	m_board.resize(m_rows,
			std::vector<std::shared_ptr<PiecePositionImp>>(m_cols, nullptr));
}

int BoardImp::getPlayer(const Point& pos) const {
	int row = pos.getY()-1;
	int col = pos.getX()-1;
	if (m_board[row][col] == nullptr)//location is empty
		return 0;
	return (m_board[row][col])->getPlayerNum();
}

void BoardImp::setPiecePos(const PiecePosition& piecePosToAdd, int playerNum)
{
	int row = piecePosToAdd.getPosition().getY()-1;
	int col = piecePosToAdd.getPosition().getX()-1;
	m_board[row][col] = std::move(std::make_shared<PiecePositionImp>(piecePosToAdd.getPosition(), piecePosToAdd.getPiece(), piecePosToAdd.getJokerRep(), playerNum));
}
void BoardImp::movePieceOnBoard(const Point& from, const Point& to)
{
	//std::cout << "Board - fromPointX:" << from.getX() << " fromPointY:" << from.getY() << "\n";
	//std::cout << "Board - toPointX:" << to.getX() << " toPointY:" << to.getY() << "\n";
	std::shared_ptr<PiecePositionImp> boardPiecePosFrom = getPiecePosByLocation(from);
	m_board[from.getY()-1][from.getX()-1] = nullptr;
	m_board[to.getY()-1][to.getX()-1] = boardPiecePosFrom;

	//std::cout << "Board - move in board\n";

}
void BoardImp::removePieceAtLocation(const Point& pointToRemove)
{
	int row = pointToRemove.getY()-1;
	int col = pointToRemove.getX()-1;
	m_board[row][col] = nullptr;
}


std::shared_ptr<PiecePositionImp> BoardImp::getPiecePosByLocation(const Point& location)
{
	int row = location.getY()-1;
	int col = location.getX()-1;
	return m_board[row][col];
}
