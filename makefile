COMP = g++-5.3.0
#COMP = g++
#put all your object files here
OBJS =  main.o PointImp.o PiecePositionImp.o BoardImp.o AutoPlayerAlgorithm.o FilePlayerAlgorithm.o FightInfoImp.o MoveImp.o JokerChangeImp.o GameManager.o
#The executabel filename DON'T CHANGE
EXEC = ex2
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG



$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies

PointImp.o: PointImp.cpp Point.h PointImp.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
PiecePositionImp.o: PiecePositionImp.cpp PiecePositionImp.h PointImp.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
BoardImp.o: BoardImp.cpp PiecePositionImp.h BoardImp.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
MoveImp.o: MoveImp.cpp Move.h PointImp.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp	
FightInfoImp.o: FightInfoImp.cpp FightInfoImp.h PointImp.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
JokerChangeImp.o: JokerChangeImp.cpp JokerChangeImp.h PointImp.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
FilePlayerAlgorithm.o: FilePlayerAlgorithm.cpp FilePlayerAlgorithm.h PlayerAlgorithm.h MoveImp.h JokerChangeImp.h PiecePositionImp.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
AutoPlayerAlgorithm.o: AutoPlayerAlgorithm.cpp AutoPlayerAlgorithm.h PlayerAlgorithm.h MoveImp.h JokerChangeImp.h PiecePositionImp.h FightInfoImp.h Board.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
GameManager.o: GameManager.cpp GameManager.h AutoPlayerAlgorithm.h FilePlayerAlgorithm.h BoardImp.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
main.o: main.cpp GameManager.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

clean:
	rm -f $(OBJS) $(EXEC)
