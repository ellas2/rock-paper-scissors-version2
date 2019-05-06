/*
 * BoardImp.h
 *
 *  Created on: May 2, 2018
 *      Author: ellas2
 */

#ifndef BOARDIMP_H_
#define BOARDIMP_H_
#include <iostream>
#include <vector>
#include <memory>
#include "PointImp.h"
#include "PiecePositionImp.h"
#include "Board.h"

class BoardImp : public Board {
public:
	BoardImp(int rows, int cols);
	int getPlayer(const Point& pos) const override; // 1 for player 1’s piece, 2 for 2, 0 if empty
	void setPiecePos(const PiecePosition& piecePosToAdd, int playerNum);
	void removePieceAtLocation(const Point& pointToRemove);
	void movePieceOnBoard(const Point& from, const Point& to);
	std::shared_ptr<PiecePositionImp> getPiecePosByLocation(const Point& location);
	~BoardImp() = default;
private:
	int m_rows;
	int m_cols;
	std::vector<std::vector<std::shared_ptr<PiecePositionImp>>> m_board;
};

#endif /* BOARDIMP_H_ */
