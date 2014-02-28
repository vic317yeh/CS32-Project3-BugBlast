#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

//actor class
class Actor: public GraphObject {
public:
    Actor (int imageID, int startX, int startY,StudentWorld* ptr);
    StudentWorld* getWorld();
    virtual void doSomething()=0;
    void setDead();
    bool isAlive() const;
    virtual void applyBugSpray(){return;}

private:
    int actorID;
    StudentWorld* st_p;
    bool m_alive;
};


//player class
class Player: public Actor {
public:
    Player(int imageID, int startX, int startY,StudentWorld* ptr);
    void doSomething();
    void moveDir(int dir);
    void penetrate_moveDir(int dir);
    bool canPenetrateWall();
    void switchPenetrateWall();
    bool atExit();
    bool bugSprayerLeft();
    void applyBugSpray();
    void increaseBugSprayer();
    void setTickWall(int n);
    void setTickBugSprayer(int n);
    void setMaxBugSprayer(int n);

    
private:
    bool penetrateWall;
    int  bugSprayerUsed;
    int  maxSprayer;
    int  m_tick_wall;
    int  m_tick_bugSprayer;
};


//zumi class
class Zumi: public Actor {
public:
    Zumi(int imageID, int startX, int startY,StudentWorld* ptr, int N);
    virtual void applyBugSpray();
    int getSpeed() const;
    int getCount() const;
    void setCount(int x);
    
private:
    //how many ticks per move
    int speed;
    //count how many ticks have passed
    int count;
};


//simple zumi class
class SimpleZumi: public Zumi {
public:
    SimpleZumi(int imageID, int startX, int startY,StudentWorld* ptr, int N);
    void doSomething();
    void move();
    void applyBugSpray();

private:
    int curDir;
};


//complex zumi class
class ComplexZumi: public Zumi {
public:
    ComplexZumi(int imageID, int startX, int startY,StudentWorld* ptr, int N);
    void doSomething();
    void move();
    void applyBugSpray();
    int firstSearch(int startX,int startY, int endX, int endY);

private:
    int curDir;
};

//brick class
class Brick: public Actor {
public:
    Brick(int imageID, int startX, int startY,StudentWorld* ptr);

};

//permanent brick class
class Perma_Brick: public Brick{
public:
    Perma_Brick(int imageID, int startX, int startY,StudentWorld* ptr);
    void doSomething(){return;}

};

//destroyable brick class
class Destroyable_Brick: public Brick{
public:
    Destroyable_Brick(int imageID, int startX, int startY,StudentWorld* ptr);
    void doSomething(){return;}
    void applyBugSpray();
};

//exit class
class Exit: public Actor{
public:
    Exit(int imageID, int startX, int startY,StudentWorld* ptr);
    void doSomething();
    
private:
    bool revealed;

};

//bug sprayer class
class BugSprayer: public Actor{
public :
    BugSprayer(int imageID, int startX, int startY,StudentWorld* ptr);
    void decreaseTick();
    void doSomething();
    bool moreTicks();
    void addBugSprays();
    void expireImmediately();
    void applyBugSpray();


private:
    int m_tick;
};


//bug spray class
class BugSpray: public Actor{
public:
    BugSpray(int imageID, int startX, int startY,StudentWorld* ptr);
    void doSomething();
    void decreaseTick();
    bool moreTicks();
 
private:
    int m_tick;
    
};


//goodies class
class Goodie: public Actor{
public:
    Goodie(int imageID, int startX, int startY,StudentWorld* ptr,int N);
    void decreaseTick();
    bool moreTicks() const;
    
private:
    int m_tick;
};


//extra life goodie class
class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(int imageID, int startX, int startY,StudentWorld* ptr,int N);
    void doSomething();
};

//walk through walls goodie class
class WalkThroughWallsGoodie : public Goodie
{
public:
    WalkThroughWallsGoodie(int imageID, int startX, int startY,StudentWorld* ptr,int N);
    void doSomething();
};

//increase simultaneous sprayer class
class IncreaseSimultaneousSprayersGoodie : public Goodie
{
public:
    IncreaseSimultaneousSprayersGoodie(int imageID, int startX, int startY,StudentWorld* ptr,int N);
    void doSomething();
};


#endif // ACTOR_H_

