#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <string>
#include <iostream>
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <vector>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld();
    
    virtual ~StudentWorld();
    
	virtual int init();

	virtual int move();

	virtual void cleanUp();
    
    int loadLevel();
    
    bool hasDbrick(int x, int y);
    
    bool hasPbrick(int x, int y);
    
    Actor* getExit();
    
    void increaseLevel();

private:
    Level lev;
    int curLev;
    Player* m_player;
    vector<Actor*> m_actors;
};



#endif // STUDENTWORLD_H_
