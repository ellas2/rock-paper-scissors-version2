/*
 * FilePlayerAlgorithm.cpp
 *
 *  Created on: May 7, 2018
 *      Author: ellas2
 */

#include "FilePlayerAlgorithm.h"


FilePlayerAlgorithm::FilePlayerAlgorithm()
{

}
FilePlayerAlgorithm::~FilePlayerAlgorithm()
{
	if (!isFileDone)
		fin.close();
}
bool isNumber(string str)
{
	int length = str.length();
	for (int i = 0; i < length; i++)
	{
		if (!isdigit(str.at(i)))
		{
			return false;
		}
	}
	return true;
}
void insertFalseElement(int player, vector<unique_ptr<PiecePosition>>& vectorToFill)
{
	std::unique_ptr<PiecePositionImp> ppImp = std::make_unique<PiecePositionImp>(PointImp(-1, -1), 'E', 'E', player);
	vectorToFill.push_back(std::move(ppImp));
}


void FilePlayerAlgorithm::getInitialPositions(int player, vector<unique_ptr<PiecePosition>>& vectorToFill)
{
	string currentString;
	char piece;
	ifstream finPos;
	int X, Y;
	if (player == 1)
	{
		fin = ifstream(player_one_moves_file_path);
		finPos = ifstream(player_one_positioning_file_path);
	}
	else
	{
		fin = ifstream(player_two_moves_file_path);
		finPos = ifstream(player_two_positioning_file_path);
	}

	if (!finPos || !fin)
	{
		insertFalseElement(player, vectorToFill);
		return;
	}

	while (getline(finPos, currentString))
	{
		if (currentString.find_first_not_of("\r\n\t") == string::npos)
			continue;
		istringstream iss(currentString);
		vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());
		int length = results.size();
		if (length < 3 || length > 4)
		{
			insertFalseElement(player, vectorToFill);
			finPos.close();
			return;
		}
		//format is: J <X> <Y> <PIECE_CHAR>
		if (length == 4)
		{
			if (results[3].length() != 1)
			{
				insertFalseElement(player, vectorToFill);
				finPos.close();
				return;
			}
			if (results[0].compare("J") != 0)
			{
				insertFalseElement(player, vectorToFill);
				finPos.close();
				return;
			}
			//checking if piece/X/Y are invalid is done in addInitalPiece
			piece = results[3][0];
			if (isNumber(results[1]))
				X = stoi(results[1]);
			else
			{
				insertFalseElement(player, vectorToFill);
				finPos.close();
				return;
			}
			if (isNumber(results[2]))
				Y = stoi(results[2]);
			else
			{
				insertFalseElement(player, vectorToFill);
				finPos.close();
				return;
			}
			std::unique_ptr<PiecePosition> ppImp = std::make_unique<PiecePositionImp>(PointImp(X, Y), 'J', piece, player);
			vectorToFill.push_back(std::move(ppImp));
			//fin.close();
			//printMSG(msg);
			//return;
		}
		//format is: <PIECE_CHAR> <X> <Y>
		else
		{
			//checking if piece/X/Y are invalid is done in addInitalPiece
			if (results[0].length() != 1)
			{
				insertFalseElement(player, vectorToFill);
				finPos.close();
				return;
			}
			piece = results[0][0];
			if (isNumber(results[1]))
				X = stoi(results[1]);
			else
			{
				insertFalseElement(player, vectorToFill);
				finPos.close();
				return;
			}
			if (isNumber(results[2]))
				Y = stoi(results[2]);
			else
			{
				insertFalseElement(player, vectorToFill);
				finPos.close();
				return;
			}
			//cout << X << " " << Y << piece << endl;
			std::unique_ptr<PiecePosition> ppImp = std::make_unique<PiecePositionImp>(PointImp(X, Y), piece, 'P', player);
			vectorToFill.push_back(std::move(ppImp));
			//fin.close();
			//printMSG(msg);
			//return;
		}
	}
	finPos.close();
	return;
}
unique_ptr<Move> FilePlayerAlgorithm::getMove()
{
	if (isFileDone == true)
		return nullptr;
	string currString;
	char jokerChar;
	int fromX, fromY, toX, toY, jokerX, jokerY;
	//read move line
	getline(fin, currString);
	while (fin && currString.find_first_not_of("\n\r\t") == string::npos)
		getline(fin, currString);
	if (!fin)
	{
		isFileDone = true;
		fin.close();
		return nullptr;
	}

	else
	{
		istringstream iss(currString);
		vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());
		//move fomatt - <FROM_X> <FROM_Y> <TO_X> <TO_Y>
		int length = results.size();
		if (length != 4 && results.size() != 8){
			return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));
		} else if (results.size() == 4){
			//move format - <FROM_X> <FROM_Y> <TO_X> <TO_Y>
			if (!isNumber(results[0]))
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));

			else
				fromX = stoi(results[0]);

			if (!isNumber(results[1]))
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));
			else
				fromY = stoi(results[1]);

			if (!isNumber(results[2]))
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));

			else
				toX = stoi(results[2]);

			if (!isNumber(results[3]))
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));

			else
				toY = stoi(results[3]);

			return std::move(make_unique<MoveImp>(PointImp(fromX, fromY), PointImp(toX,toY)));
		} else if (length == 8){
			//move + joker format - <FROM_X> <FROM_Y> <TO_X> <TO_Y> J: <Joker_X> <Joker_Y> <NEW_REP>
			if (!isNumber(results[0]))
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));

			else
				fromX = stoi(results[0]);

			if (!isNumber(results[1]))
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));
			else
				fromY = stoi(results[1]);

			if (!isNumber(results[2]))
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));

			else
				toX = stoi(results[2]);

			if (!isNumber(results[3]))
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));

			else
				toY = stoi(results[3]);

			if (results[4].compare("J:") != 0){
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));
			}
			if (!isNumber(results[5]))
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));
			else
				jokerX = stoi(results[5]);

			if (!isNumber(results[6]))
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));

			else
				jokerY = stoi(results[6]);
			if (results[7].length() != 1){
				return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));
			}
			jokerChar = results[7][0];

			jokerChange = JokerChangeImp(PointImp(jokerX, jokerY), jokerChar);
			needToChangeJoker = true;
			return std::move(make_unique<MoveImp>(PointImp(fromX, fromY), PointImp(toX,toY)));
		}
		else{
			return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));
		}

	}
	return std::move(make_unique<MoveImp>(PointImp(-1,-1), PointImp(-1,-1)));
}
unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange()
{
	if (needToChangeJoker)
	{
		std::unique_ptr<JokerChange> jkChange = std::make_unique<JokerChangeImp>(jokerChange);
		needToChangeJoker = false;
		return (std::move(jkChange));
	}
	else
		return nullptr;
}
void FilePlayerAlgorithm::notifyOnInitialBoard(const Board& b, const vector<unique_ptr<FightInfo>>& fights)
{
	if (b.getPlayer(PointImp(1,1)) < 0 || fights.size() < 1)
		return;
	return;
}
void FilePlayerAlgorithm::notifyOnOpponentMove(const Move& move)
{
	if (move.getTo().getX() < 1)
		return;
	return;
}
void FilePlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo)
{
	if (fightInfo.getWinner() < 0)
		return;
	return;
}

