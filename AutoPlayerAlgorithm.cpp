/*
 * AutoPlayerAlgorithm.cpp
 *
 *  Created on: Apr 28, 2018
 *      Author: yevkirich
 */
#include <algorithm>
#include "AutoPlayerAlgorithm.h"
#include <time.h>
#include <iostream>
#include <memory>

void reducePieceCount(piecesCount& count, char charToReduce)
{
	switch (charToReduce)
	{
		case 'R':
			count.rocks--;
			break;
		case 'S':
			count.scissors--;
			break;
		case 'P':
			count.papers--;
			break;
		case 'B':
			count.bombs--;
			break;
		case 'F':
			count.flags--;
			break;
	}
}

int findIndexInVector(vector<PiecePositionImp>& pieces, const Point& point)
{
	for (unsigned int i = 0; i < pieces.size(); i++)
	{
		if (pieces[i].getPositionImp() == point)
		{
			return i;
		}
	}
	return -1;
}
int findIndexInInitialOpp(vector<PointImp>& pieces, const Point& point)
{
	for (unsigned int i = 0; i < pieces.size(); i++)
	{
		if (pieces[i] == point)
		{
			return i;
		}
	}
	return -1;
}
void AutoPlayerAlgorithm::removePieceLoc(vector<PiecePositionImp>& pieces, const Point& point, bool onlyIfBomb)
{
	int j = findIndexInVector(pieces, point);
	//std::cout << "in remove pieces any way\n";
	if (j == -1)
		return;
	char returned =  pieces[j].getPiece();
	if (onlyIfBomb)
	{
		if (returned != 'B')
		{
			j = -1;
		}

	}
	if (j != -1)
	{
		//std::cout << "in remove pieces\n";
		pieces.erase(pieces.begin() + j);
		&pieces == &myPieces ? reducePieceCount(myPiecesCount, returned) : reducePieceCount(opponentPieces, returned);
	}


}
void changePieceLoc(vector<PiecePositionImp>& pieces, const Point& point, const Point& newPoint)
{
	int index = findIndexInVector(pieces, point);
	if (index != -1)
		pieces[index].movePosition(newPoint);
}

bool AutoPlayerAlgorithm::posIsTaken(const Point& point)
{
	return (findIndexInVector(myPieces, point) != -1) ? true : false;
}

void AutoPlayerAlgorithm::getInitialPositions(int player, vector<unique_ptr<PiecePosition>>& vectorToFill)
{
	playerNum = player;
	oppPlayerNum = playerNum == 1 ? 2 : 1;
	int row;
	int col;
	//srand (time(NULL));
	vector<std::pair<char, int>> piece;
	piece.push_back(std::pair<char,int>('R',R));
	piece.push_back(std::pair<char,int>('P',P));
	piece.push_back(std::pair<char,int>('S',S));
	piece.push_back(std::pair<char,int>('B',B));
	piece.push_back(std::pair<char,int>('J',J));
	piece.push_back(std::pair<char,int>('F',F));

	for (const std::pair<char,int>& p : piece)
	{
		for (int i = 0; i < p.second; i++)
		{
			row = rand() % 10 + 1;
			col = rand() % 10 + 1;
			PointImp point(col,row);
			while(posIsTaken(point))
			{
				row = rand() % 10 + 1;
				col = rand() % 10 + 1;
				point.setX(col);
				point.setY(row);
			}
			//std::cout << p.first << " " << col << " " << row << "\n";
			std::unique_ptr<PiecePosition> ppImp = std::make_unique<PiecePositionImp>(point, p.first, 'S', playerNum);
			myPieces.push_back(PiecePositionImp(point, p.first, 'S', playerNum));
			vectorToFill.push_back(std::move(ppImp));

		}
	}

}

void AutoPlayerAlgorithm::removePointFromOppInitial(const Point& point)
{
	int index = findIndexInInitialOpp(opponentInitialPositions, point);
	if (index != -1)
		opponentInitialPositions.erase(opponentInitialPositions.begin()+ index);
	//opponentInitialPositions.erase(std::find(opponentInitialPositions.begin(), opponentInitialPositions.end(), point));
}

int fight(char myPiece, char oppPiece)
{
	if (oppPiece == myPiece)
		return 0;
	else if (myPiece == 'F' ||
			(myPiece == 'R' && oppPiece == 'P') ||
			(myPiece == 'P' && oppPiece == 'S') ||
			(myPiece == 'S' && oppPiece == 'R') ||
			(oppPiece == 'B'))
		return -1;
	else
		return 1;
}

int AutoPlayerAlgorithm::getMoveScore(const PiecePositionImp& piece, const PointImp& moveTo)
{
	int index = findIndexInInitialOpp(opponentInitialPositions, moveTo);
	if (index != -1)
	{
		int vecSize = opponentInitialPositions.size();
		if (vecSize < 3)
			return 1000;
		else if (vecSize < 6)
			return 750;
		else
			return 500;
	}
	index = findIndexInVector(opponentUncovered, moveTo);
	if (index != -1)
	{
		char oppPiece = opponentUncovered[index].getPiece();
		char myPiece = piece.getPiece() == 'J' ? piece.getJokerRep() : piece.getPiece();
		int fightResult = fight(myPiece, oppPiece);
		if (fightResult == -1)
			return -1000;
		else if (fightResult == 0)
		{
			if (piece.getPiece() =='J')
				return -1000;
			int remainingPieces = myPieces.size() - (opponentUncovered.size() + opponentInitialPositions.size());
			if (remainingPieces < -4)
				return -750;
			else if (remainingPieces < 0)
				return -500;
			else if (remainingPieces < 4)
				return 501;
			else
				return 751;
		}
		else
			return 1001;

	}
	return (rand() % 6);
}
bool AutoPlayerAlgorithm::legalPoint(const PointImp& point)
{
	if (point.getX() <= M && point.getX() > 0 && point.getY() > 0 && point.getY() <= N && !posIsTaken(point))
		return true;
	else
		return false;
}
unique_ptr<Move> AutoPlayerAlgorithm::getMove()
{

	int maxScore = -2000;
	int currScore = 0;
	PiecePositionImp* pieceToMove;
	PointImp moveTo(0,0);
	for (PiecePositionImp& piece : myPieces)
	{
		char pieceRep = piece.getPiece() == 'J' ? piece.getJokerRep() : piece.getPiece();
		if (pieceRep == 'F' || pieceRep == 'B')
			continue;
		PointImp newPoint(piece.getPosition().getX() + 1,piece.getPosition().getY());
		if (legalPoint(newPoint) && (currScore = getMoveScore(piece, newPoint)) > maxScore)
		{
			pieceToMove = &piece;
			moveTo = newPoint;
			maxScore = currScore;
		}
		newPoint = PointImp(piece.getPosition().getX() ,piece.getPosition().getY()+1);
		if (legalPoint(newPoint) && (currScore = getMoveScore(piece, newPoint)) > maxScore)
		{
			pieceToMove = &piece;
			moveTo = newPoint;
			maxScore = currScore;
		}
		newPoint = PointImp(piece.getPosition().getX() - 1 ,piece.getPosition().getY());
		if (legalPoint(newPoint) && (currScore = getMoveScore(piece, newPoint)) > maxScore)
		{
			pieceToMove = &piece;
			moveTo = newPoint;
			maxScore = currScore;
		}
		newPoint = PointImp(piece.getPosition().getX() ,piece.getPosition().getY() - 1);
		if (legalPoint(newPoint) && (currScore = getMoveScore(piece, newPoint)) > maxScore)
		{
			pieceToMove = &piece;
			moveTo = newPoint;
			maxScore = currScore;
		}
	}
	std::unique_ptr<Move> move = std::make_unique<MoveImp>(pieceToMove->getPositionImp(), moveTo);
	pieceToMove->movePosition(moveTo);
	return std::move(move);

}
char charToWin(char oppChar)
{
	char ret = 'S';
	std::cout << oppChar << "\n";
	switch (oppChar)
	{
		case 'P':
			ret = 'S';
			break;
		case 'R':
			ret = 'P';
			break;
		case 'S':
			ret = 'R';
			break;
		default:
			ret = 'S';
	}
	return ret;
}
unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange()
{
	int index = -1;
	int fightRes;
	for (PiecePositionImp& piece : myPieces)
	{
		char pieceRep = piece.getPiece();
		if (pieceRep != 'J')
			continue;
		PointImp newPoint(piece.getPosition().getX() + 1,piece.getPosition().getY());
		if (legalPoint(newPoint) && (index = findIndexInVector(opponentUncovered, newPoint)) != -1)
		{
			char oppPiece = opponentUncovered[index].getPiece();
			fightRes = fight(piece.getJokerRep(), oppPiece);
			if (fightRes != 1)
			{
				std::unique_ptr<JokerChange> jChange = std::make_unique<JokerChangeImp>(piece.getPosition(), charToWin(oppPiece));
				return std::move(jChange);
			}

		}
		newPoint = PointImp(piece.getPosition().getX() ,piece.getPosition().getY()+1);
		if (legalPoint(newPoint) && (index = findIndexInVector(opponentUncovered, newPoint)) != -1)
		{
			char oppPiece = opponentUncovered[index].getPiece();
			fightRes = fight(piece.getJokerRep(), oppPiece);
			if (fightRes != 1)
			{
				std::unique_ptr<JokerChange> jChange = std::make_unique<JokerChangeImp>(piece.getPosition(), charToWin(oppPiece));
				return std::move(jChange);
			}

		}
		newPoint = PointImp(piece.getPosition().getX() - 1 ,piece.getPosition().getY());
		if (legalPoint(newPoint) && (index = findIndexInVector(opponentUncovered, newPoint)) != -1)
		{
			char oppPiece = opponentUncovered[index].getPiece();
			fightRes = fight(piece.getJokerRep(), oppPiece);
			if (fightRes != 1)
			{
				std::unique_ptr<JokerChange> jChange = std::make_unique<JokerChangeImp>(piece.getPosition(), charToWin(oppPiece));
				return std::move(jChange);
			}

		}
		newPoint = PointImp(piece.getPosition().getX() ,piece.getPosition().getY() - 1);
		if (legalPoint(newPoint) && (index = findIndexInVector(opponentUncovered, newPoint)) != -1)
		{
			char oppPiece = opponentUncovered[index].getPiece();
			fightRes = fight(piece.getJokerRep(), oppPiece);
			if (fightRes != 1)
			{
				std::unique_ptr<JokerChange> jChange = std::make_unique<JokerChangeImp>(piece.getPosition(), charToWin(oppPiece));
				return std::move(jChange);
			}

		}
	}
	return nullptr;
}
void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& b, const vector<unique_ptr<FightInfo>>& fights)
{
	for (const auto& fight : fights)
	{
		notifyFightResult(*fight);
	}
	//std::cout << "in notify\n";
	for (int i = 1; i <= M; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			int piece = b.getPlayer(PointImp(i,j));
			int index;
			if (piece > 0 && piece != playerNum && (index = findIndexInVector(opponentUncovered, PointImp(i,j))) == -1)
			{
				opponentInitialPositions.push_back(PointImp(i,j));
			}
		}
	}

}
void AutoPlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo)
{
	int winner = fightInfo.getWinner();
	char oppPiece = fightInfo.getPiece(oppPlayerNum);
	const Point& currPoint = fightInfo.getPosition();
	//std::cout << "winner" << winner << "\n";
	if (winner == 0) //Tie
	{
		removePieceLoc(myPieces, currPoint, false);
		removePieceLoc(opponentUncovered, currPoint, false);

	}
	else if (winner == playerNum) //Player won
	{
		removePieceLoc(myPieces, currPoint, true);
		removePieceLoc(opponentUncovered, currPoint, false);
	}
	else //Player lost
	{
		removePieceLoc(myPieces, currPoint, false);
		if (oppPiece == 'B')
			removePieceLoc(opponentUncovered, currPoint, true);
		else
		{
			int index = -1;
			if ((index = findIndexInVector(opponentUncovered, PointImp(currPoint))) == -1)
				opponentUncovered.push_back(PiecePositionImp(currPoint, oppPiece, oppPiece, oppPlayerNum));
			else
				changePieceLoc(opponentUncovered, opponentUncovered[index].getPosition(), currPoint);
		}
	}

}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move)
{
	removePointFromOppInitial(move.getFrom());
	int index = -1;
	if ((index = findIndexInVector(opponentUncovered, PointImp(move.getFrom()))) == -1)
		opponentUncovered.push_back(PiecePositionImp(move.getTo(), '@', '@', oppPlayerNum));
	else
		changePieceLoc(opponentUncovered, move.getFrom(), move.getTo());
}
