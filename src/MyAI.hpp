// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"
#include<limits.h>
#include<vector>
#include<queue>
#include<map>
#include<math.h>
#include<iostream>
#include<string>
#include<string.h>
static const int maxR = 10;
static const int maxC = 10;
class PositionRC{
	public:
		int r;
		int c;
		PositionRC(int x=0, int y=0):r(x), c(y) {}
		static void setPosRC(PositionRC&pos, int x, int y){
			pos.r = x;
			pos.c = y;
		}
		void setRC(int x, int y){
			r = x;
			c = y;
		}
		void operator=(PositionRC& pos){
			r = pos.r;
			c = pos.c;
		}

		bool operator==(const PositionRC& pos) const{
			return (r==pos.r)&&(c==pos.c);
		}
		bool operator!=(const PositionRC& pos) const{
			return !(*this==pos);
		}
		bool operator<(const PositionRC& pos) const{
			return ( r*(maxC)+c < pos.r*(maxC)+pos.c );
		}
		bool operator>(const PositionRC& pos) const{
			return !(*this<pos);
		}
};
class MyAI : public Agent
{
public:
	MyAI ( void );
	
	Action getAction
	(
		bool stench,
		bool breeze,
		bool glitter,
		bool bump,
		bool scream
	);
	
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================

	
	PositionRC curPos;
	int lenR = maxR;
	int lenC = maxC;
	bool getGold = false;
	bool foundMonster = false;
	bool killedMonster = false;
	bool knowRow = false;
	bool knowCol = false;
	std::vector<Action> actHistory;
	std::vector<PositionRC> posHistory;
	std::queue<Action> curActQueue;
	int curDirection = 0; // 0 - right, 1 - down, 2 - left, 3 - up
	PositionRC lastPos;
	PositionRC targetPos;
	PositionRC originPos;
	PositionRC monsterPos;
	PositionRC shootingPos;
	bool letsShoot = false;
	Action lastAction = FORWARD;
	int stepCnt = 0;

	bool maybeMonster[maxR][maxC];
	int maybeMonsterCnt = maxR*maxC;
	bool maybePit[maxR][maxC];
	int maybePitCnt = maxR*maxC;
	bool isVisited[maxR][maxC];
	bool isSafe[maxR][maxC];
	bool targetOrigin = false;

	void responseForBump(); // revise the curPos, and set the boundary lenR lenC
	void setVisited(); // set visited(curPos) true


	void setMaybeM_scream();
	// set maybeMonster=false for the positions
	// which NOT adjacent (up,down,left,right) to the "stench" position just sensed
	// also update maybeMosnterCnt, if =1 we found the monster.
	void setMaybeM_stench();
	// Similarily, set maybeMonster=false for the positions
	// which adjacent (up,down,left,right,and itself) to the "noStench" position just sensed
	// also update maybeMosnterCnt, if =1 we found the monster.
	void setMaybeM_noStench();
	// Similarily, setMaybePit if noBreeze
	void setMaybeP_noBreeze();

	// safePos=true where maybeMonster==false && maybePit==false
	void setSafePos();

	// BFS the nearest reachable unvisited safe Pos,
	// If no such Pos Or got gold already, find the nearest path back to origin.
	void setPath();
	bool setPathNew();
	int setPathToPos(PositionRC& targetP);

	// set curActQueue from curPos to targetPos based on Parents Relation
	void setActQueue(std::map<PositionRC, PositionRC>& parentPos, PositionRC& startPos, PositionRC& targetPos);
	bool setActAdjacentPos(PositionRC& posA, PositionRC& posB, std::vector<Action>& tmpActQ, int& simuDirection);

	void updateCurPos(Action act);



	bool isSafePos(PositionRC& pos);
	bool isInRange(PositionRC& pos);
	// return whether posA is up/... to posB
	bool isUp(PositionRC& posA, PositionRC& posB);
	bool isDown(PositionRC& posA, PositionRC& posB);
	bool isLeft(PositionRC& posA, PositionRC& posB);
	bool isRight(PositionRC& posA, PositionRC& posB);

	// refresh Mosnter Count in map
	void refreshMapCnt();
	void makeSureMonsterPos();
	int findPosToShoot();

	// goShooting: only for adjacent posA,B
	bool goShootingAdj(PositionRC& posA, PositionRC& posB);
	int setPathToPosFK(PositionRC& targetP, bool updateToActQueue);
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif
