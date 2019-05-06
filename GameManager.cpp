/*
 * GameManager.cpp
 *
 *  Created on: May 7, 2018
 *      Author: ellas2
 */

#include "GameManager.h"

GameManager::GameManager(int gameType, int rows, int cols) : m_boardImp (BoardImp(rows,cols))
{
	switch(gameType)
	{
		case AUTO_VS_FILE:
			m_firstPlayerAlg = std::make_shared<AutoPlayerAlgorithm>();
			m_secondPlayerAlg = std::make_shared<FilePlayerAlgorithm>();
			break;
		case FILE_VS_AUTO:
			m_firstPlayerAlg = std::make_shared<FilePlayerAlgorithm>();
			m_secondPlayerAlg = std::make_shared<AutoPlayerAlgorithm>();
			break;
		case AUTO_VS_AUTO:
			m_firstPlayerAlg = std::make_shared<AutoPlayerAlgorithm>();
			m_secondPlayerAlg = std::make_shared<AutoPlayerAlgorithm>();
			break;
		case FILE_VS_FILE:
			m_firstPlayerAlg = std::make_shared<FilePlayerAlgorithm>();
			m_secondPlayerAlg = std::make_shared<FilePlayerAlgorithm>();
			break;
	}
	m_currentPlayerAlg = m_firstPlayerAlg;
}
void GameManager::manageGame()
{
	if (GetPiecePositioning() == false)
		return;
	/*int res = player1PieceCount.jokers + player1PieceCount.papers +
			player1PieceCount.scissors + player1PieceCount.rocks - player1PieceCount.jokerBombs;
	std::cout << "first: " << res << endl;
	int res2 = player2PieceCount.jokers + player2PieceCount.papers +
			player2PieceCount.scissors + player2PieceCount.rocks - player2PieceCount.jokerBombs;
	std::cout << "second: " << res2 << endl;*/
	loadAndExecuteMoves();
}

bool GameManager::GetPiecePositioning()
{
	string reason = "";
	int winner = -1;
	std::vector<unique_ptr<FightInfo>> fights;
	//get first user piece positioning
	std::vector<unique_ptr<PiecePosition>> firstPlayerVec;
	m_firstPlayerAlg->getInitialPositions(PLAYER_ONE, firstPlayerVec);
	RPSMessage player1PosMsg = UpdateBoard(firstPlayerVec, PLAYER_ONE, fights);
	//cout << "here - after first user pos" << endl;
	if (player1PosMsg == RPSWrongInputLine || isBadPositioning(player1PosMsg) || player1PosMsg == RPSPlayer1NotEnoughFlags)
	{
		printMSG(player1PosMsg);
		winner = PLAYER_TWO;
		reason = "Bad positioning for player 1";//TODO: change me!
	}
	//get second user piece positioning
	std::vector<unique_ptr<PiecePosition>> secondPlayerVec;
	m_secondPlayerAlg->getInitialPositions(PLAYER_TWO, secondPlayerVec);
	RPSMessage player2PosMsg = UpdateBoard(secondPlayerVec, PLAYER_TWO, fights);
	//cout << "here - after second user pos" << endl;
	if (player2PosMsg == RPSWrongInputLine || isBadPositioning(player2PosMsg) || player2PosMsg == RPSPlayer2NotEnoughFlags)
	{
		printMSG(player2PosMsg);
		if (winner == PLAYER_TWO)
		{
			winner = NO_PLAYER;
			reason = "A tie - Bad positioning for both players";//TODO: change me!
			printGameOutputToFile(reason, winner, true);
			return false;
		}
		winner = PLAYER_ONE;
		reason = "Bad positioning for player 2";//TODO: change me!
		printGameOutputToFile(reason, winner, true);
		return false;
	}
	if (winner == PLAYER_TWO)
	{
		printGameOutputToFile(reason, winner, true);
		return false;
	}
	//cout << "here - before check winner" << endl;
	RPSMessage winnerMsg = checkWinner();
	if (winnerMsg != RPSSuccess)
	{
		printWinnerToFileByMsg(winnerMsg);
		return false;
	}
	//cout << "here - after check winner" << endl;
	m_firstPlayerAlg->notifyOnInitialBoard(m_boardImp, fights);
	m_secondPlayerAlg->notifyOnInitialBoard(m_boardImp, fights);
	//cout << "here - end" << endl;
	return true;
}

RPSMessage GameManager::UpdateBoard(std::vector<unique_ptr<PiecePosition>>& newPositionsVec, int playerNum, std::vector<unique_ptr<FightInfo>>& fights)
{
	pieceCount currPieceCount; //if tried to position more than allowed but some got eaten along the way
	for (const auto& piecePos: newPositionsVec)
	{
		//check piecePos validity
		RPSMessage msg = isValidPiecePosition(*piecePos);
		if (msg != RPSSuccess)
			return 	msg;
		else if (isTooManyPieces(*piecePos, currPieceCount))
			return RPSTooManyPiecesOfSomeSort;
		appendToPieceCount(*piecePos, currPieceCount);
		int locationPlayerNum = m_boardImp.getPlayer(piecePos->getPosition());
		if (playerNum == locationPlayerNum) //Location is taken by the same player
			return RPSTargetLocationContainsPlayerPiece;
		else if (locationPlayerNum == 0) //Location is empty - TODO: remember to change that when you move pieces!!!!!!!!
			addPiecePosAndUpdatePieceCount(*piecePos, playerNum);
		else //Location is taken by the other user - They should fight over it!
		{
			std::shared_ptr<PiecePositionImp> boardPiecePos = m_boardImp.getPiecePosByLocation(piecePos->getPosition());
			if (playerNum == PLAYER_ONE)
				executeFight(*piecePos, *boardPiecePos, fights, PLAYER_TWO);
			else //playerNum == PLAYER_TWO
				executeFight(*boardPiecePos, *piecePos, fights, PLAYER_ONE);
		}
	}

	//pieceCount* currPlayerPieceCount = (playerNum == 1)?(&player1PieceCount):(&player2PieceCount);
	if (currPieceCount.flags < F)
	{
		if (playerNum == 1)
			return RPSPlayer1NotEnoughFlags;
		else
			return RPSPlayer2NotEnoughFlags;
	}
	return RPSSuccess;
}


bool GameManager::isTooManyPieces(const PiecePosition& piecePos, pieceCount& pieceCount)
{
	char piecePosChar = piecePos.getPiece();
	bool res = false;
	switch(piecePosChar)
	{
		case 'R':
			if (pieceCount.rocks == R)
				res = true;
			break;
		case 'P':
			if (pieceCount.papers == P)
				res = true;
			break;
		case 'S':
			if (pieceCount.scissors == S)
				res = true;
			break;
		case 'B':
			if (pieceCount.bombs == B)
				res = true;
			break;
		case 'J':
			if (pieceCount.jokers == J)
				res = true;
			break;
		case 'F':
			if (pieceCount.flags == F)
				res = true;
			break;
	}
	return res;
}

//bool GameManager::isTooManyPieces(const PiecePosition& piecePos, int playerNum)
//{
//	pieceCount* currPlayerPieceCount = (playerNum == 1)?(&player1PieceCount):(&player2PieceCount);
//	char piecePosChar = piecePos.getPiece();
//	bool res = false;
//	switch(piecePosChar)
//	{
//		case 'R':
//			if (currPlayerPieceCount->rocks == R)
//				res = true;
//			break;
//		case 'P':
//			if (currPlayerPieceCount->papers == P)
//				res = true;
//			break;
//		case 'S':
//			if (currPlayerPieceCount->scissors == S)
//				res = true;
//			break;
//		case 'B':
//			if (currPlayerPieceCount->bombs == B)
//				res = true;
//			break;
//		case 'J':
//			if (currPlayerPieceCount->jokers == J)
//				res = true;
//			break;
//		case 'F':
//			if (currPlayerPieceCount->flags == F)
//				res = true;
//			break;
//	}
//	return res;
//}

void GameManager::appendToPieceCount(const PiecePosition& piecePos, pieceCount& pieceCount)
{
	char piecePosChar = piecePos.getPiece();
	switch(piecePosChar)
			{
				case 'R':
					(pieceCount.rocks)++;
					break;
				case 'P':
					(pieceCount.papers)++;
					break;
				case 'S':
					(pieceCount.scissors)++;
					break;
				case 'B':
					(pieceCount.bombs)++;
					break;
				case 'J':
					(pieceCount.jokers)++;
					if (piecePos.getJokerRep() == 'B')
						pieceCount.jokerBombs++;
					break;
				case 'F':
					(pieceCount.flags)++;
					break;
			}
}

void GameManager::printWinnerToFileByMsg(RPSMessage msg)
{
	int winner;
	string reason;
	if (msg == RPSPlayer1WonNoOpponentFlags)
	{
		reason = "All flags of the opponent are captured";
		winner = 1;
	} else if (msg == RPSPlayer1WonNoOpponentMovingPieces){
		reason = "All moving PIECEs of the opponent are eaten";
		winner = 1;
	} else if (msg == RPSPlayer2WonNoOpponentFlags){
		reason = "All flags of the opponent are captured";
		winner = 2;
	} else if (msg == RPSPlayer2WonNoOpponentMovingPieces){
		reason = "All moving PIECEs of the opponent are eaten";
		winner = 2;
	} else if (msg == RPSTieNoFlags){
		reason = "A tie - all flags are eaten by both players in the position files";
		winner = 0;
	}
	else if (msg == RPSTieNoMovingPieces){
		reason = "A tie - all flags are eaten by both players in the position files";
		winner = 0;
	}
	printGameOutputToFile(reason , winner, false);
}

char GameManager::getFightChar(const PiecePosition& piecePos)
{
	char piece = piecePos.getPiece();
	char jokerRep = piecePos.getJokerRep();
	if (piece != 'J')
		return piece;
	else//piece == 'J'
		return jokerRep;
}

void GameManager::executeFight(const PiecePosition& firstPlayerPiecePos, const PiecePosition& secondPlayerPiecePos, std::vector<unique_ptr<FightInfo>>& fights, int playerNumOnBoard)
{
	char firstPlayerFightChar = getFightChar(firstPlayerPiecePos);
	char secondPlayerFightChar = getFightChar(secondPlayerPiecePos);
	int winner;
	if (secondPlayerFightChar == firstPlayerFightChar)
	{
		winner = NO_PLAYER_WINS_FIGHT;
		if (playerNumOnBoard == PLAYER_ONE)
			removePiecePosAndUpdatePieceCount(firstPlayerPiecePos, PLAYER_ONE);
		else
			removePiecePosAndUpdatePieceCount(secondPlayerPiecePos, PLAYER_TWO);
	}
	else if (firstPlayerFightChar == 'F' ||
			(firstPlayerFightChar == 'R' && secondPlayerFightChar == 'P') ||
			(firstPlayerFightChar == 'P' && secondPlayerFightChar == 'S') ||
			(firstPlayerFightChar == 'S' && secondPlayerFightChar == 'R'))
	{
		winner = PLAYER_TWO_WINS_FIGHT;
		if (playerNumOnBoard == PLAYER_ONE)
		{
			removePiecePosAndUpdatePieceCount(firstPlayerPiecePos, PLAYER_ONE);
			addPiecePosAndUpdatePieceCount(secondPlayerPiecePos, PLAYER_TWO);
		}
	}
	else if (secondPlayerFightChar == 'B')
	{
		winner = PLAYER_TWO_WINS_FIGHT;
		if (playerNumOnBoard == PLAYER_ONE)
			removePiecePosAndUpdatePieceCount(firstPlayerPiecePos, PLAYER_ONE);
		else
			removePiecePosAndUpdatePieceCount(secondPlayerPiecePos, PLAYER_TWO);
	}

	else if (firstPlayerFightChar == 'B')
	{
		winner = PLAYER_ONE_WINS_FIGHT;
		if (playerNumOnBoard == PLAYER_ONE)
			removePiecePosAndUpdatePieceCount(firstPlayerPiecePos, PLAYER_ONE);
		else
			removePiecePosAndUpdatePieceCount(secondPlayerPiecePos, PLAYER_TWO);
	}
	else
	{
		winner = PLAYER_ONE_WINS_FIGHT;
		if (playerNumOnBoard == PLAYER_TWO)
		{
			removePiecePosAndUpdatePieceCount(secondPlayerPiecePos, PLAYER_TWO);
			addPiecePosAndUpdatePieceCount(firstPlayerPiecePos, PLAYER_ONE);
		}
	}
	PointImp fightLocation = firstPlayerPiecePos.getPosition();
	std::unique_ptr<FightInfoImp> fightPtr = std::make_unique<FightInfoImp>(fightLocation, firstPlayerFightChar, secondPlayerFightChar, winner);
	fights.push_back(std::move(fightPtr));
}

void GameManager::executeSingleFight(const PiecePosition& firstPlayerPiecePos, const PiecePosition& secondPlayerPiecePos)
{
	char firstPlayerFightChar = getFightChar(firstPlayerPiecePos);
	char secondPlayerFightChar = getFightChar(secondPlayerPiecePos);
	int PLAYER = m_currentPlayerAlg == m_firstPlayerAlg ? PLAYER_ONE : PLAYER_TWO;
	int OPP_PLAYER = m_currentPlayerAlg == m_firstPlayerAlg ? PLAYER_TWO : PLAYER_ONE;
	int winner;
	//FightInfoImp(const PointImp& pos, char player1Piece, char player2Piece, int winner = 0);
	// 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
	if (secondPlayerFightChar == 'B' || secondPlayerFightChar == firstPlayerFightChar)
	{
		if (secondPlayerFightChar == firstPlayerFightChar){
			winner = 0;
		}
		else
			winner = OPP_PLAYER;
		removePiecePosAndUpdatePieceCount(firstPlayerPiecePos, PLAYER);
		removePiecePosAndUpdatePieceCount(secondPlayerPiecePos, OPP_PLAYER);
	}
	else if (firstPlayerFightChar == 'F' ||
			(firstPlayerFightChar == 'R' && secondPlayerFightChar == 'P') ||
			(firstPlayerFightChar == 'P' && secondPlayerFightChar == 'S') ||
			(firstPlayerFightChar == 'S' && secondPlayerFightChar == 'R'))
	{
			removePiecePosAndUpdatePieceCount(firstPlayerPiecePos, PLAYER);
			winner = OPP_PLAYER;
	}
	else
	{
		std::shared_ptr<PiecePositionImp> boardPiecePosFrom = m_boardImp.getPiecePosByLocation(firstPlayerPiecePos.getPosition());
		const PointImp from = boardPiecePosFrom->getPosition();
		const PointImp to = secondPlayerPiecePos.getPosition();
		//std::cout << "---fromPointX:" << from.getX() << " fromPointY:" << from.getY() << "\n";
		//std::cout << "---toPointX:" << to.getX() << " toPointY:" << to.getY() << "\n";
		boardPiecePosFrom->movePosition(to);
		removePiecePosAndUpdatePieceCount(secondPlayerPiecePos, OPP_PLAYER);
		m_boardImp.movePieceOnBoard(from, to);
		winner = PLAYER;
	}
	FightInfoImp fightRes(secondPlayerPiecePos.getPosition(), firstPlayerFightChar, secondPlayerFightChar, winner);
	m_firstPlayerAlg->notifyFightResult(fightRes);
	m_secondPlayerAlg->notifyFightResult(fightRes);
}
void GameManager::removePiecePosAndUpdatePieceCount(const PiecePosition& piecePos, int playerNum)
{
	m_boardImp.removePieceAtLocation(piecePos.getPosition());
	pieceCount* currPlayerPieceCount = (playerNum == 1)?(&player1PieceCount):(&player2PieceCount);
	char piecePosChar = piecePos.getPiece();
	switch(piecePosChar)
		{
			case 'R':
				(currPlayerPieceCount->rocks)--;
				break;
			case 'P':
				(currPlayerPieceCount->papers)--;
				break;
			case 'S':
				(currPlayerPieceCount->scissors)--;
				break;
			case 'B':
				(currPlayerPieceCount->bombs)--;
				break;
			case 'J':
				(currPlayerPieceCount->jokers)--;
				if (piecePos.getJokerRep() == 'B')
					currPlayerPieceCount->jokerBombs--;
				break;
			case 'F':
				(currPlayerPieceCount->flags )--;
				break;
		}
}

void GameManager::addPiecePosAndUpdatePieceCount(const PiecePosition& piecePos, int playerNum)
{
	m_boardImp.setPiecePos(piecePos, playerNum);
	pieceCount* currPlayerPieceCount = (playerNum == 1)?(&player1PieceCount):(&player2PieceCount);
	char piecePosChar = piecePos.getPiece();
	switch(piecePosChar)
		{
			case 'R':
				(currPlayerPieceCount->rocks)++;
				break;
			case 'P':
				(currPlayerPieceCount->papers)++;
				break;
			case 'S':
				(currPlayerPieceCount->scissors)++;
				break;
			case 'B':
				(currPlayerPieceCount->bombs)++;
				break;
			case 'J':
				(currPlayerPieceCount->jokers)++;
				if (piecePos.getJokerRep() == 'B')
					currPlayerPieceCount->jokerBombs++;
				break;
			case 'F':
				(currPlayerPieceCount->flags )++;
				break;
		}
}

RPSMessage GameManager::isValidPiecePosition(const PiecePosition& piecePos)
{
	int col = piecePos.getPosition().getX();
	int row = piecePos.getPosition().getY();
	char c = piecePos.getPiece();
	char jokerRep = piecePos.getJokerRep();
	//std::cout << c << " " << jokerRep  << " " << col << " " << row << endl;
	if (col < 1 || col > M || row < 1 || row > N)//PiecePos Point not in range
		return RPSWrongSourceCoords;
	if (c == 'R' || c == 'P' || c == 'S' || c == 'B' || c == 'F' || c == 'J')
	{
		if (c == 'J' && (jokerRep == 'R' || jokerRep == 'P'|| jokerRep =='S' || jokerRep == 'B'))
			return RPSSuccess;
		else if (c == 'R' || c == 'P' || c == 'S' || c == 'B' || c == 'F')
			return RPSSuccess;
		else
			return RPSWrongPiece;
	}
	else
		return RPSWrongPiece;
}

bool isValidPoint(const Point& point)
{
	int col = point.getX();
	int row = point.getY();
	if (col < 1 || col > M || row < 1 || row > N)//Point not in range
		return false;
	return true;
}
RPSMessage GameManager::loadAndExecuteSingleMove()
{
	std::unique_ptr<Move> currentMove;
	currentMove = m_currentPlayerAlg->getMove();
	if (currentMove == nullptr)
		return RPSSuccess;
	const Point& fromPoint = currentMove->getFrom();
	const Point& toPoint = currentMove->getTo();
	//std::cout << "fromPointX:" << fromPoint.getX() << " fromPointY:" << fromPoint.getY() << endl;
	//std::cout << "toPointX:" << toPoint.getX() << " toPointY:" << toPoint.getY() << endl;
	if (!isValidPoint(fromPoint))
		return RPSWrongSourceCoords;
	if (!isValidPoint(toPoint))
		return RPSWrongDestCoords;
	int locationPlayerNum = m_boardImp.getPlayer(fromPoint);
	if ((locationPlayerNum == 1 && m_currentPlayerAlg == m_secondPlayerAlg) ||
			(locationPlayerNum == 2 && m_currentPlayerAlg == m_firstPlayerAlg))
		return RPSSourceLocationDoesntContainsPlayerPiece;
	else if (locationPlayerNum == 0)
	{
		return RPSLocationIsEmpty;
	}
	std::shared_ptr<PiecePositionImp> boardPiecePosFrom = m_boardImp.getPiecePosByLocation(fromPoint);
	char fromChar = getFightChar(*boardPiecePosFrom);
	if (fromChar != 'R' && fromChar != 'S' && fromChar != 'P')
		return RPSPieceCannotMove;
	locationPlayerNum = m_boardImp.getPlayer(toPoint);
	if ((locationPlayerNum == 2 && m_currentPlayerAlg == m_secondPlayerAlg) ||
			(locationPlayerNum == 1 && m_currentPlayerAlg == m_firstPlayerAlg))
		return RPSTargetLocationContainsPlayerPiece;
	else if (locationPlayerNum == 0)
	{
		if (m_currentPlayerAlg == m_firstPlayerAlg)
			m_secondPlayerAlg->notifyOnOpponentMove(MoveImp(fromPoint, toPoint));
		else
			m_firstPlayerAlg->notifyOnOpponentMove(MoveImp(fromPoint, toPoint));
		boardPiecePosFrom->movePosition(toPoint);
		m_boardImp.movePieceOnBoard(fromPoint, toPoint);
	}
	else //Location is taken by the other user - They should fight over it!
	{
		if (m_currentPlayerAlg == m_firstPlayerAlg)
			m_secondPlayerAlg->notifyOnOpponentMove(MoveImp(fromPoint, toPoint));
		else
			m_firstPlayerAlg->notifyOnOpponentMove(MoveImp(fromPoint, toPoint));
		std::shared_ptr<PiecePositionImp> boardPiecePosTo = m_boardImp.getPiecePosByLocation(toPoint);

		executeSingleFight(*boardPiecePosFrom, *boardPiecePosTo);
	}

	return RPSSuccess;
}
void GameManager::switchCurrentPlayer()
{
	m_currentPlayerAlg = m_currentPlayerAlg == m_firstPlayerAlg ? m_secondPlayerAlg : m_firstPlayerAlg;
}

RPSMessage GameManager::loadAndExecuteJokerChange()
{
	std::unique_ptr<JokerChange> jChange = m_currentPlayerAlg->getJokerChange();
	if (jChange == nullptr)
		return RPSSuccess;
	//std::cout << "in joker change" << jChange->getJokerChangePosition().getX() <<
	//		jChange->getJokerChangePosition().getY()<< " new rep: " << jChange->getJokerNewRep()<<"\n";

	if (!isValidPoint(jChange->getJokerChangePosition()))
		return RPSWrongSourceCoords;
	std::shared_ptr<PiecePositionImp> boardPiecePosFrom = m_boardImp.getPiecePosByLocation(jChange->getJokerChangePosition());
	if (boardPiecePosFrom->getPiece() != 'J')
		return RPSNotAJokerPiece;
	char jokerRep = jChange->getJokerNewRep();
	if (jokerRep != 'R' && jokerRep != 'P' && jokerRep !='S' && jokerRep != 'B')
		return RPSWrongPiece;
	char pieceBefore = boardPiecePosFrom->getPiece();
	boardPiecePosFrom->setjokerRep(jokerRep);
	//std::cout << "joker changed\n";
	pieceCount* currCount = m_currentPlayerAlg == m_firstPlayerAlg ? &player1PieceCount : &player2PieceCount;
	if (pieceBefore == 'B')
		currCount->jokerBombs--;
	if (jokerRep =='B')
		currCount->jokerBombs++;
	return RPSSuccess;

}
RPSMessage GameManager::loadAndExecuteMoves(){

	RPSMessage msg = RPSSuccess;
	//bool isFirstFileDone = false;
	//bool isSecondFileDone = false;
	string reason;
	while (true)
	{
		if (counter >= 100)
		{
			printGameOutputToFile("A tie - both Moves inputs done without a winner" , 0, false);
			return RPSSuccess;
		}
		if ((msg = loadAndExecuteSingleMove()) != RPSSuccess)
		{
			if (isBadPositioning(msg))
				printMSG(msg);
			if (m_currentPlayerAlg == m_firstPlayerAlg)
			{
				reason = "Bad Move for player 1";
				printGameOutputToFile(reason , 2, false);
				return RPSSuccess;
			}
			else
			{
				reason = "Bad Move for player 2";
				printGameOutputToFile(reason , 1, false);
				return RPSSuccess;
			}

		}
		if ((msg = loadAndExecuteJokerChange()) != RPSSuccess)
		{
			if (isBadPositioning(msg))
				printMSG(msg);
			if (m_currentPlayerAlg == m_firstPlayerAlg)
			{
				reason = "Bad Move for player 1";
				printGameOutputToFile(reason , 2, false);
				return RPSSuccess;
			}
			else
			{
				reason = "Bad Move for player 2";
				printGameOutputToFile(reason , 1, false);
				return RPSSuccess;
			}
		}
		if ((msg = checkWinner()) != RPSSuccess)
		{
			//std::cout << "we are in loadAndExecuteMoves - (msg = checkWinner()) != RPSSuccess" << std::endl;
			printWinnerToFileByMsg(msg);
			return RPSSuccess;
		}
		counter++;
		//printBoardToFile(std::cout);
		switchCurrentPlayer();

	}
	return RPSSuccess;
}

RPSMessage GameManager::checkWinner(){
	//cout << "player 1 flagNum = " << player1PieceCount.flags << endl;
	//cout << "player 2 flagNum = " << player2PieceCount.flags << endl;
	if (player1PieceCount.flags == 0 && player2PieceCount.flags == 0)
		return RPSTieNoFlags;
	else if (player1PieceCount.flags == 0)
			return RPSPlayer2WonNoOpponentFlags;
	else if (player2PieceCount.flags == 0)
		return RPSPlayer1WonNoOpponentFlags;

	else if (!hasMovingPieces(player1PieceCount) && !hasMovingPieces(player2PieceCount)){
		return RPSTieNoMovingPieces;
	}
	else if (!hasMovingPieces(player1PieceCount)){
		return RPSPlayer2WonNoOpponentMovingPieces;
	}

	else if (!hasMovingPieces(player2PieceCount))
		return RPSPlayer1WonNoOpponentMovingPieces;

	return RPSSuccess;
}



bool isBadPositioning(RPSMessage msg){
	if  (msg == RPSTooManyPiecesOfSomeSort || msg == RPSNoFlags || msg == RPSTargetLocationContainsPlayerPiece ||
			msg == RPSWrongSourceCoords || msg == RPSWrongPiece || msg == RPSPieceCannotMove ||
			msg == RPSLocationIsTaken || msg == RPSLocationIsEmpty)
		return true;
	return false;
}


bool isWinnerRelated(RPSMessage msg){
	if (msg == RPSTieNoFlags || msg == RPSTieNoMovingPieces || msg == RPSPlayer1WonNoOpponentFlags || msg == RPSPlayer1WonNoOpponentMovingPieces || msg == RPSPlayer2WonNoOpponentFlags ||
	msg == RPSPlayer2WonNoOpponentMovingPieces)
		return true;
	return false;
}
void GameManager::printGameOutputToFile(string reason, int winner, bool isBadPositioning){
	ofstream fout(OUT_FILE_PATH);
	fout << "Winner: " << winner << endl;
	fout << "Reason: " << reason << endl;
	fout << endl;
	if (!isBadPositioning)
		printBoardToFile(fout);
	fout.close();
}
void GameManager::printBoardToFile(ostream& fout){
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= M ; j++)
		{
			if (m_boardImp.getPiecePosByLocation(PointImp(j, i)) == nullptr)
				fout << " ";

			else
			{
				char toPrint = m_boardImp.getPiecePosByLocation(PointImp(j, i))->getPiece();
				if (m_boardImp.getPlayer(PointImp(j, i)) == PLAYER_TWO)
					toPrint = tolower(toPrint);
				fout << toPrint;
			}
		}
		fout << endl;
	}

}
void printMSG(RPSMessage msg)
{
	if (msg == RPSPieceCannotMove)
	{
		cout << "ERROR: flags and bombs cannot move" << endl;
	}
	else if (msg == RPSTooManyPiecesOfSomeSort)
	{
		cout << "ERROR: too many pieces of some kind" << endl;
	}
	else if (msg == RPSNotEnoughFlags)
	{
		cout << "ERROR: not enough flags" << endl;
	}
	else if (msg == RPSLocationIsTaken)
	{
		cout << "ERROR: the location already contains a piece" << endl;
	}
	//else if (msg == RPSTooManyJokers)
	//{
	//	cout << "ERROR: too many jokers" << endl;
	//}
	else if (msg == RPSNoFlags)
	{
		cout << "ERROR: no flags" << endl;
	}
	else if (msg == RPSWrongDestCoords)
	{
		cout << "ERROR: bad destination coordinates" << endl;
	}
	else if (msg == RPSWrongSourceCoords)
	{
		cout << "ERROR: bad source coordinates" << endl;
	}
	else if (msg == RPSSourceLocationDoesntContainsPlayerPiece)
	{
		cout << "ERROR: source location does't contain a player piece" << endl;
	}
	else if (msg == RPSTargetLocationContainsPlayerPiece)
	{
		cout << "ERROR: target already contains same player's piece" << endl;
	}
	else if (msg == RPSLocationIsEmpty)
	{
		cout << "ERROR: source location is empty" << endl;
	}
	else if (msg == RPSInvalidPieceMove)
	{
		cout << "ERROR: incorrect piece move" << endl;

	}
	else if (msg == RPSBothPlayersNotEnoughFlags)
	{
		cout << "Both Players don't have enough flags" << endl;
	}
	else if (msg == RPSPlayer1NotEnoughFlags)
	{
		cout << "Player 1 doesn't have enough flags" << endl;
	}
	else if (msg == RPSPlayer2NotEnoughFlags)
	{
		cout << "Player 2 doesn't have enough flags" << endl;
	}
	else if (msg == RPSTieNoFlags)
	{
		cout << "It's a tie! No flags left for both players" << endl;
	}
	else if (msg == RPSTieNoMovingPieces)
	{
		cout << "Its a Tie! No moving pieces left for both players" << endl;
	}
	else if (msg == RPSPlayer1WonNoOpponentFlags)
	{
		cout << "Player 1 Wins - All opponent flags are captured! " << endl;
	}
	else if (msg == RPSPlayer1WonNoOpponentMovingPieces)
	{
		cout << "Player 1 Wins - All opponent moving pieces are eaten! " << endl;
	}
	else if (msg == RPSPlayer2WonNoOpponentFlags)
	{
		cout << "Player 2 Wins - All opponent flags are captured! " << endl;
	}
	else if (msg == RPSPlayer2WonNoOpponentMovingPieces)
	{
		cout << "Player 2 Wins - All opponent moving pieces are eaten! " << endl;
	}
	else if (msg == RPSWrongPiece)
	{
		cout << "ERROR: wrong piece" << endl;
	}
	else if (msg == RPSWrongInputLine)
	{
		cout << "ERROR: wrong input line" << endl;
	}
	else if (msg == RPSFileFailure)
	{
		cout << "ERROR: File failure" << endl;
	}
}
