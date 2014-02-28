#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <string>
#include <iostream>
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
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
    
    bool hasbrick(int x, int y);
        
    void increaseLevel();
    
    void addActor(Actor* a);
    
    void getPlayerLocation(int& x, int& y) const;
    
    void applyBugSprayToActorsAt(int x, int y);
    
    void removeDeadObjects();
    
    void playerIncreaseBugSprayer();
    
    bool hasBugSprayerAt(int x, int y);
    
    bool anyLiveZumi() const;
    
    void setLevelFinished();
    
    bool levelFinished() const;
    
    bool zumiAndSprayersBlockedAt(int x, int y) const;
    
    bool hasZumiAt(int x, int y) const;
    
    void updateDisplayText();
    
    string setDisplayText(int score, int level, int livesLeft, unsigned int bonus);
    
    int getBonus() const;
    
    void decBonus();
    
    void activatePlayerWalkThroughWalls();
    
    void increasePlayerSimultaneousSprayers();
    
    void setPlayerDead();
    
    
    // Report the value of the level data file options
    unsigned int getProbOfGoodieOverall() const;
    unsigned int getProbOfExtraLifeGoodie() const;
    unsigned int getProbOfWalkThruGoodie() const;
    unsigned int getProbOfMoreSprayersGoodie() const;
    unsigned int getGoodieLifetimeInTicks() const;
    unsigned int getComplexZumiSearchDistance() const;

    
private:
    Level lev;
    int curLev;
    bool finished;
    Player* m_player;
    vector<Actor*> m_actors;
    int bonus;
};



#endif // STUDENTWORLD_H_
