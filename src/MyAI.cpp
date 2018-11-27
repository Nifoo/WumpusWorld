// ======================================================================
// FILE:        MyAI.cpp
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

// PENG HAO
/*
 *
 * map:
 * (r,c)
 * (2,0) ...
 * (1,0) ...
 * (0,0) (0,1) (0,2)...
 *
*
 * direction:
 * 0 - right, 1 - down, 2 - left, 3 - up
 */
#include "MyAI.hpp"
std::string actionEmu[6]={
	"TURN_LEFT",
	"TURN_RIGHT",
	"FORWARD",
	"SHOOT",
	"GRAB",
	"CLIMB"
};
std::string directionEmu[4]={"right", "down", "left", "up"};


MyAI::MyAI() : Agent()
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	PositionRC::setPosRC(targetPos, lenR, lenC);
	for(int i=0; i<maxR; i++){
		for(int j=0; j<maxC; j++){
			maybeMonster[i][j] = true;
			maybePit[i][j] = true;
			isVisited[i][j] = false;
			isSafe[i][j] = false;
		}
	}
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}
	

void MyAI::updateCurPos(Action act){
	switch(act)
	{
		case FORWARD:
			switch(this->curDirection){
				case 0: this->curPos.c ++; break;
				case 1: this->curPos.r --; break;
				case 2:	this->curPos.c --; break;
				case 3: this->curPos.r ++; break;
			}
			break;
		case TURN_LEFT: curDirection = (curDirection+4-1)%4; break;
		case TURN_RIGHT: curDirection = (curDirection+1)%4; break;
		case SHOOT: /*printf("SHOOT\n");*/ letsShoot = false; break;
		default: break;
	}
}

void MyAI::refreshMapCnt(){
	maybeMonsterCnt = 0;
	for(int i=0; i<lenR; i++){
		for(int j=0; j<lenC; j++){
			if(maybeMonster[i][j]) maybeMonsterCnt++;
		}
	}
}
void MyAI::responseForBump(){
	switch(this->curDirection){
		case 0: this->curPos.c --; lenC = curPos.c+1; knowCol = true; break;
		case 1: this->curPos.r ++; break;
		case 2:	this->curPos.c ++; break;
		case 3: this->curPos.r --; lenR = curPos.r+1; knowRow = true; break;
	}
	if(knowCol && knowRow) refreshMapCnt();
}

void MyAI::setMaybeM_scream(){
	maybeMonster[monsterPos.r][monsterPos.c] = false;
	maybeMonsterCnt = 0;
	killedMonster = true;
	//printf("Scream!\n");
}
void MyAI::setVisited(){
	isVisited[curPos.r][curPos.c] = true;
}
void MyAI::setMaybeM_stench(){
	maybeMonsterCnt = 0;
	for(int i=0; i<lenR; i++){
		for(int j=0; j<lenC; j++){
			if(!(
					((abs(i-curPos.r)==1) && (j==curPos.c))
					|| ((i==curPos.r) && (abs(j-curPos.c)==1))
			)){
				if(maybeMonster[i][j]==true){
					maybeMonster[i][j]=false;
				}
			}
			if (maybeMonster[i][j]==true) maybeMonsterCnt++;
		}
	}
}
void MyAI::setMaybeM_noStench(){
	int delR[]={-1, 1, 0, 0, 0};
	int delC[]={0, 0, 0, -1, 1};
	int tmpR, tmpC;
	for(int i=0; i<sizeof(delR)/sizeof(int); i++){
		tmpR = curPos.r+delR[i];
		tmpC = curPos.c+delC[i];
		if(tmpR>=0 && tmpR<lenR && tmpC>=0 && tmpC<lenC){
			if(maybeMonster[tmpR][tmpC]==true){
				maybeMonster[tmpR][tmpC]=false;
				maybeMonsterCnt--;
			}
		}
	}
}
void MyAI::setMaybeP_noBreeze(){
	int delR[]={-1, 1, 0, 0, 0};
	int delC[]={0, 0, 0, -1, 1};
	int tmpR, tmpC;
	for(int i=0; i<sizeof(delR)/sizeof(int); i++){
		tmpR = curPos.r+delR[i];
		tmpC = curPos.c+delC[i];
		if(tmpR>=0 && tmpR<lenR && tmpC>=0 && tmpC<lenC){
			if(maybePit[tmpR][tmpC]==true){
				maybePit[tmpR][tmpC]=false;
				maybePitCnt--;
			}
		}
	}
}
void MyAI::setSafePos(){
	for(int i=0; i<lenR; i++){
		for(int j=0; j<lenC; j++){
			if((maybeMonster[i][j]==false) && (maybePit[i][j]==false)){
				isSafe[i][j]=true;
			}
		}
	}
}

bool MyAI::isSafePos(PositionRC& pos){
	return isSafe[pos.r][pos.c];
}
bool MyAI::isInRange(PositionRC& pos){
	int tmpR = pos.r;
	int tmpC = pos.c;
	return(tmpR>=0 && tmpR<lenR && tmpC>=0 && tmpC<lenC);
}


bool MyAI::isUp(PositionRC& posA, PositionRC& posB){
	return ((posA.r-posB.r==1)&&(posA.c==posB.c));
}
bool MyAI::isDown(PositionRC& posA, PositionRC& posB){
	return ((posA.r-posB.r==-1)&&(posA.c==posB.c));
}
bool MyAI::isLeft(PositionRC& posA, PositionRC& posB){
	return ((posA.r==posB.r)&&(posA.c-posB.c==-1));
}
bool MyAI::isRight(PositionRC& posA, PositionRC& posB){
	return ((posA.r==posB.r)&&(posA.c-posB.c==1));
}

// store the action sequence from A to B (A and B are adjacent) into tmpActQ
bool MyAI::setActAdjacentPos(PositionRC& posA, PositionRC& posB, std::vector<Action>& tmpActQ, int& simuDirection){
	//curDirection =  0 - right, 1 - down, 2 - left, 3 - up
	////****
	//std::cout<<"Direction: "<<directionEmu[simuDirection]<<std::endl;
	if(isUp(posA, posB)){
		switch(simuDirection){
		case 0:
			tmpActQ.push_back(TURN_RIGHT);
			break;
		case 1:
			break;
		case 2:
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 3:
			tmpActQ.push_back(TURN_LEFT);
			tmpActQ.push_back(TURN_LEFT);
			break;
		}
		tmpActQ.push_back(FORWARD);
		simuDirection = 1;
		return true;
	}
	else if(isDown(posA, posB)){
		switch(simuDirection){
		case 0:
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 1:
			tmpActQ.push_back(TURN_LEFT);
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 2:
			tmpActQ.push_back(TURN_RIGHT);
			break;
		case 3:
			break;
		}
		tmpActQ.push_back(FORWARD);
		simuDirection = 3;
		return true;
	}
	else if(isLeft(posA, posB)){
		switch(simuDirection){
		case 0:
			break;
		case 1:
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 2:
			tmpActQ.push_back(TURN_LEFT);
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 3:
			tmpActQ.push_back(TURN_RIGHT);
			break;
		}
		tmpActQ.push_back(FORWARD);
		simuDirection = 0;
		return true;
	}
	else if(isRight(posA, posB)){
		switch(simuDirection){
		case 0:
			tmpActQ.push_back(TURN_LEFT);
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 1:
			tmpActQ.push_back(TURN_RIGHT);
			break;
		case 2:
			break;
		case 3:
			tmpActQ.push_back(TURN_LEFT);
			break;
		}
		tmpActQ.push_back(FORWARD);
		simuDirection = 2;
		return true;
	}
	else return false;
}
void MyAI:: setActQueue(std::map<PositionRC, PositionRC>& parentPos, PositionRC& startPos, PositionRC& targetP){

	////****
	//std::cout<<"setActQueue from "<<curPos.r<<" "<<curPos.c<<" to "<<targetP.r<<" "<<targetP.c<<std::endl;
	std::vector<PositionRC> posQ;
	posQ.push_back(targetP);
	PositionRC testPos = targetP;
	while(testPos!=startPos){
		testPos = parentPos[testPos];
		posQ.push_back(testPos);
	}
	// posQ[0] is targetPos, posQ[size-1] is curPos
	int simuDirection = curDirection; // 0 - right, 1 - down, 2 - left, 3 - up
	for(int i=posQ.size()-1; i>=1; i--){
		std::vector<Action> tmpActQ;
		setActAdjacentPos(posQ[i], posQ[i-1], tmpActQ, simuDirection);
		////****
		//std::cout<<"( "<<posQ[i].r<<", "<<posQ[i].c<<" )-->( "<<posQ[i-1].r<<", "<<posQ[i-1].c<<" ): ";

		for(int j=0; j<tmpActQ.size(); j++){
			curActQueue.push(tmpActQ[j]);
			////****
			//std::cout<<actionEmu[tmpActQ[j]]<<", ";
		}
		////****
		//std::cout<<std::endl;
	}
}

//BFS. target is already known/visited targetP (usually is (0,0), this function used to go back).return distance.
int MyAI:: setPathToPos(PositionRC& targetP){
	return setPathToPosFK(targetP, true);
}
int MyAI:: setPathToPosFK(PositionRC& targetP, bool updateToActQueue){
	if(!isInRange(targetP) || !isSafePos(targetP)) return INT_MAX;
	if(curPos==targetP) return 0;
	int delR[]={-1, 1, 0, 0};
	int delC[]={0, 0, -1, 1};
	int tmpR, tmpC, tmpDir;
	bool findNewPos= false;
	PositionRC frntPos, sonPos;
	int frntDir;
	std::queue<PositionRC> bfsQ;
	std::queue<int> dirQ;
	bfsQ.push(curPos);
	dirQ.push(curDirection);
	std::map<PositionRC, PositionRC> parentPos;
	bool bfsVisited[maxR][maxC][4]={0};
	bfsVisited[curPos.r][curPos.c][curDirection]=true;
	parentPos[curPos] = curPos;
	bool findTarget = false;
	int levelCnt = 0;
	//BFS find the nearest, safe, unvisited pos
	while(!bfsQ.empty() && !findTarget){
		levelCnt++;
		frntPos = bfsQ.front();
		frntDir = dirQ.front();
		bfsQ.pop();
		dirQ.pop();

		for(int i=0; i<3; i++){
			tmpR = frntPos.r;
			tmpC = frntPos.c;
			tmpDir = frntDir;

			// move forward
			if(i==0){
				switch(frntDir){
					case 0: tmpC++; break;
					case 1: tmpR--; break;
					case 2: tmpC--; break;
					case 3: tmpR++; break;
				}
			}
			// turn left
			else if(i==1){
				tmpDir = (tmpDir+3) % 4;
			}
			// turn right
			else if(i==2){
				tmpDir = (tmpDir+1) % 4;
			}

			////****
			//std::cout<<tmpR<<", "<<tmpC<<": ";
			PositionRC::setPosRC(sonPos, tmpR, tmpC);
			if(bfsVisited[tmpR][tmpC][tmpDir]==false && isInRange(sonPos)){
				bfsVisited[tmpR][tmpC][tmpDir]=true;
				if(isSafePos(sonPos)){
					////****
					//std::cout<<"isSafe ";
					if(i==0) parentPos[sonPos] = frntPos;
					if(sonPos==targetP){
						////****
						//std::cout<<"isTarget "<<std::endl;
						findTarget = true;
						break;
					}
					bfsQ.push(sonPos);
					dirQ.push(tmpDir);
				}
			}
			////****
			//std::cout<<std::endl;
		}
	}
	// from targetPos trace back, set the path, set the curActQueue.
	if(findTarget){
		////****
		//std::cout<<"Path to Target ( "<<targetP.r<<", "<<targetP.c<<" ) Found!"<<std::endl;
		if(updateToActQueue) setActQueue(parentPos, curPos, targetP);
		return levelCnt;
	}
	else{
		////****
		//std::cout<<"Path to Target ( "<<targetP.r<<", "<<targetP.c<<" ) NOT Found!"<<std::endl;
		return INT_MAX;
	}
}

//BFS. target is the nearest reachable invisited safe Pos
bool MyAI:: setPathNew(){

	// 0 - right, 1 - down, 2 - left, 3 - up
	int delR[]={-1, 1, 0, 0};
	int delC[]={0, 0, -1, 1};
	int tmpR, tmpC, tmpDir;
	bool findNewPos= false;
	PositionRC frntPos, sonPos;
	int frntDir;
	std::queue<PositionRC> bfsQ;
	std::queue<int> dirQ;
	bfsQ.push(curPos);
	dirQ.push(curDirection);
	std::map<PositionRC, PositionRC> parentPos;
	bool bfsVisited[maxR][maxC][4]={0};
	bfsVisited[curPos.r][curPos.c][curDirection]=true;
	parentPos[curPos] = curPos;
	//BFS find the nearest, safe, unvisited pos
	while(!bfsQ.empty() && !findNewPos){
		frntPos = bfsQ.front();
		frntDir = dirQ.front();
		////****
		//std::cout<<"curPos: "<<frntPos.r<<", "<<frntPos.c<<std::endl;
		bfsQ.pop();
		dirQ.pop();
		for(int i=0; i<3; i++){
			tmpR = frntPos.r;
			tmpC = frntPos.c;
			tmpDir = frntDir;

			// move forward
			if(i==0){
				switch(frntDir){
					case 0: tmpC++; break;
					case 1: tmpR--; break;
					case 2: tmpC--; break;
					case 3: tmpR++; break;
				}
			}
			// turn left
			else if(i==1){
				tmpDir = (tmpDir+3) % 4;
			}
			// turn right
			else if(i==2){
				tmpDir = (tmpDir+1) % 4;
			}

			////****
			//std::cout<<tmpR<<", "<<tmpC<<": ";
			PositionRC::setPosRC(sonPos,tmpR, tmpC);
			if(bfsVisited[tmpR][tmpC][tmpDir]==false && isInRange(sonPos)){
				bfsVisited[tmpR][tmpC][tmpDir]=true;
				////****
				//std::cout<<"inRange ";
				if(isSafePos(sonPos)){
					////****
					//std::cout<<"isSafe ";
					if(i==0) parentPos[sonPos] = frntPos;
					if(!isVisited[tmpR][tmpC]){
						////****
						//std::cout<<"unVisited "<<std::endl;
						findNewPos = true;
						targetPos = sonPos;
						break;
					}
					bfsQ.push(sonPos);
					dirQ.push(tmpDir);
				}
			}
			////****
			//std::cout<<std::endl;
		}
	}
	// from targetPos trace back, set the path, set the curActQueue.
	if(findNewPos){
		////****
		//std::cout<<"new Pos Found!"<<std::endl;
		setActQueue(parentPos, curPos, targetPos);
		return true;
	}
	else{
		////****
		//std::cout<<"new Pos NOT Found!"<<std::endl;
		return false;
	}
}

void MyAI::makeSureMonsterPos(){
	for(int i=0; i<lenR; i++){
		for(int j=0; j<lenC; j++){
			if(maybeMonster[i][j]){
				monsterPos.r = i; monsterPos.c = j; break;
			}
		}
	}
}
int MyAI::findPosToShoot(){
	int stpMin = INT_MAX, tmpStp = INT_MAX;
	int tmpR = monsterPos.r;
	int tmpC = monsterPos.c;
	bool canReach = false;
	PositionRC tmpPos;

	tmpPos.r = tmpR-1; tmpPos.c = tmpC;
	tmpStp = setPathToPosFK(tmpPos, false);
	if(tmpStp<stpMin){
		stpMin = tmpStp;
		shootingPos = tmpPos;
		canReach = true;
	}

	tmpPos.r = tmpR+1; tmpPos.c = tmpC;
	tmpStp = setPathToPosFK(tmpPos, false);
	if(tmpStp<stpMin){
		stpMin = tmpStp;
		shootingPos = tmpPos;
		canReach = true;
	}

	tmpPos.r = tmpR; tmpPos.c = tmpC-1;
	tmpStp = setPathToPosFK(tmpPos, false);
	if(tmpStp<stpMin){
		stpMin = tmpStp;
		shootingPos = tmpPos;
		canReach = true;
	}

	tmpPos.r = tmpR; tmpPos.c = tmpC+1;
	tmpStp = setPathToPosFK(tmpPos, false);
	if(tmpStp<stpMin){
		stpMin = tmpStp;
		shootingPos = tmpPos;
		canReach = true;
	}
	return stpMin;
}
bool MyAI::goShootingAdj(PositionRC& posA, PositionRC& posB){
	//curDirection =  0 - right, 1 - down, 2 - left, 3 - up
	////****
	//std::cout<<"Direction: "<<directionEmu[simuDirection]<<std::endl;
	bool shot;
	int simuDirection = curDirection;
	std::vector<Action> tmpActQ;

	if(isUp(posA, posB)){
		switch(simuDirection){
		case 0:
			tmpActQ.push_back(TURN_RIGHT);
			break;
		case 1:
			break;
		case 2:
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 3:
			tmpActQ.push_back(TURN_LEFT);
			tmpActQ.push_back(TURN_LEFT);
			break;
		}
		tmpActQ.push_back(SHOOT);
		simuDirection = 1;
		shot = true;
	}
	else if(isDown(posA, posB)){
		switch(simuDirection){
		case 0:
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 1:
			tmpActQ.push_back(TURN_LEFT);
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 2:
			tmpActQ.push_back(TURN_RIGHT);
			break;
		case 3:
			break;
		}
		tmpActQ.push_back(SHOOT);
		simuDirection = 3;
		shot = true;
	}
	else if(isLeft(posA, posB)){
		switch(simuDirection){
		case 0:
			break;
		case 1:
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 2:
			tmpActQ.push_back(TURN_LEFT);
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 3:
			tmpActQ.push_back(TURN_RIGHT);
			break;
		}
		tmpActQ.push_back(SHOOT);
		simuDirection = 0;
		shot = true;
	}
	else if(isRight(posA, posB)){
		switch(simuDirection){
		case 0:
			tmpActQ.push_back(TURN_LEFT);
			tmpActQ.push_back(TURN_LEFT);
			break;
		case 1:
			tmpActQ.push_back(TURN_RIGHT);
			break;
		case 2:
			break;
		case 3:
			tmpActQ.push_back(TURN_LEFT);
			break;
		}
		tmpActQ.push_back(SHOOT);
		simuDirection = 2;
		shot = true;
	}
	if(shot){
		for(int j=0; j<tmpActQ.size(); j++)	curActQueue.push(tmpActQ[j]);
		return true;
	}
	else return false;
}
void MyAI::setPath(){
	if(getGold){
		//printf("got Gold!\n");
		PositionRC::setPosRC(targetPos, 0, 0);
		setPathToPos(targetPos);
	}
	else{
		if(letsShoot && curPos==shootingPos && maybeMonsterCnt==1 && !killedMonster){
			//printf("goShooting!\n");
			goShootingAdj(curPos, monsterPos);
		}
		else{
			bool findNewPosPath = setPathNew();
			if(findNewPosPath==false){
				//printf("%d\n", maybeMonsterCnt);
				if(maybeMonsterCnt == 1){
					makeSureMonsterPos();
					int stpMin = findPosToShoot();
					if( stpMin == INT_MAX){
						PositionRC::setPosRC(targetPos, 0, 0);
						setPathToPos(targetPos);
					}
					else{
						PositionRC::setPosRC(targetPos, shootingPos.r, shootingPos.c);
						letsShoot = true;
						if(shootingPos == curPos){
							goShootingAdj(curPos, monsterPos);
						}
						else{
							setPathToPos(targetPos);
						}
					}
				}
				else{
					PositionRC::setPosRC(targetPos, 0, 0);
					setPathToPos(targetPos);
				}
			}
		}
	}
}
Agent::Action MyAI::getAction
(
	bool stench,
	bool breeze,
	bool glitter,
	bool bump,
	bool scream
)
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================

	// according to sense update maybeMonster, maybePit,

	////****
	//std::cout<<"curDirection(My): "<<directionEmu[curDirection]<<std::endl;
	if(lastAction==FORWARD){

		if(bump) responseForBump();
		setVisited();

		if(scream) setMaybeM_scream();

		if(stench) setMaybeM_stench();
		else setMaybeM_noStench();

		if(!breeze)	setMaybeP_noBreeze();

		setSafePos();
	}

	Action act;
	if(targetPos == originPos && curPos == originPos) act = CLIMB;
	else if(glitter){
		getGold = true;
		act = GRAB;
		////****
		//std::cout<<"Gold Found! : ";
	}
	else if(!curActQueue.empty()){
		act = curActQueue.front();
		curActQueue.pop();
		////****
		//std::cout<<"curActQueue NOT empty! : ";
	}
	else{
		setPath();
		if(targetPos == originPos && curPos == originPos){
			act = CLIMB;
		}
		else{
			act = curActQueue.front();
			curActQueue.pop();
			////****
			//std::cout<<"setPath() Finished! : ";
		}
	}

	////****
	//std::cout<<actionEmu[act]<<std::endl;
	//std::cout<<"==============================================="<<std::endl;


	updateCurPos(act);
	return act;


	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================


// ======================================================================
// YOUR CODE ENDS
// ======================================================================
