#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor: public GraphObject {
public:
    Actor (int imageID, int startX, int startY,StudentWorld* ptr);
    StudentWorld* getWorld();
    
private:
    int actorID;
    StudentWorld* st_p;
};


class Player: public Actor {
public:
    Player(int imageID, int startX, int startY,StudentWorld* ptr);
    void doSomething();
    bool isAlive();
    void setDead();
    void moveDir(int dir);
    void penetrate_moveDir(int dir);
    bool canPenetrateWall();
    void switchPenetrateWall();
    bool atExit();
    
private:
    bool penetrateWall;
    bool alive;
    
};

class Zumi: public Actor {
public:
    Zumi(int imageID, int startX, int startY,StudentWorld* ptr);
};


class SimpleZumi: public Zumi {
public:
    SimpleZumi(int imageID, int startX, int startY,StudentWorld* ptr);

};

class ComplexZumi: public Zumi {
public:
    ComplexZumi(int imageID, int startX, int startY,StudentWorld* ptr);
    
};

class Brick: public Actor {
public:
    Brick(int imageID, int startX, int startY,StudentWorld* ptr);

};

class Perma_Brick: public Brick{
public:
    Perma_Brick(int imageID, int startX, int startY,StudentWorld* ptr);
};


class Destroyable_Brick: public Brick{
public:
    Destroyable_Brick(int imageID, int startX, int startY,StudentWorld* ptr);
};


class Exit: public Actor{
public:
    Exit(int imageID, int startX, int startY,StudentWorld* ptr);
    
};

#endif // ACTOR_H_


