#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor class
Actor::Actor(int imageID, int startX, int startY,StudentWorld* ptr):GraphObject(imageID,startX,startY){
    actorID=imageID;
    st_p=ptr;
    
}

StudentWorld* Actor::getWorld(){
    return st_p;
}

//player class
Player::Player(int imageID, int startX, int startY,StudentWorld* ptr):Actor(imageID,startX,startY,ptr){
    setVisible(true);
    alive=true;
    penetrateWall=false;
}

bool Player::canPenetrateWall(){
    return penetrateWall;
}

void Player::switchPenetrateWall(){
    if (penetrateWall==true)
        penetrateWall=false;
    else penetrateWall=true;
}

bool Player::isAlive(){
    return alive;
}

void Player::setDead(){
    alive=false;
}

void Player::penetrate_moveDir(int dir){
    switch (dir) {
        case KEY_PRESS_UP:
            if (!getWorld()->hasPbrick(getX(),getY()+1))
                moveTo(getX(),getY()+1);
            break;
        case KEY_PRESS_DOWN:
            if (!getWorld()->hasPbrick(getX(),getY()-1))
                moveTo(getX(),getY()-1);
            break;
        case KEY_PRESS_LEFT:
            if (!getWorld()->hasPbrick(getX()-1,getY()))
                moveTo(getX()-1,getY());
            break;
        case KEY_PRESS_RIGHT:
            if (!getWorld()->hasPbrick(getX()+1,getY()))
                moveTo(getX()+1,getY());
            break;
        case KEY_PRESS_SPACE:
            break;
            
    }
}

void Player::moveDir(int dir){
    
        // user hit a key this tick!
        switch (dir) {
            case KEY_PRESS_UP:
                if (!getWorld()->hasDbrick(getX(),getY()+1)&&!getWorld()->hasPbrick(getX(),getY()+1))
                    moveTo(getX(),getY()+1);
                break;
            case KEY_PRESS_DOWN:
                if (!getWorld()->hasDbrick(getX(),getY()-1)&&!getWorld()->hasPbrick(getX(),getY()-1))
                moveTo(getX(),getY()-1);
                break;
            case KEY_PRESS_LEFT:
                if (!getWorld()->hasDbrick(getX()-1,getY())&&!getWorld()->hasPbrick(getX()-1,getY()))
                moveTo(getX()-1,getY());
                break;
            case KEY_PRESS_RIGHT:
                if (!getWorld()->hasDbrick(getX()+1,getY())&&!getWorld()->hasPbrick(getX()+1,getY()))
                moveTo(getX()+1,getY());
               break;
            case KEY_PRESS_SPACE:
                 break;
            
        }
}



void Player::doSomething(){
    
    if (!isAlive()){
        getWorld()->playSound(SOUND_PLAYER_DIE);
        return;
    }
    
    
    int dir;
    
    
    //check if the player can wall through wall to decide which move function to use
    if (canPenetrateWall()){
        if (getWorld()->getKey(dir)){
            penetrate_moveDir(dir);
        }
    }
    else {
        if (getWorld()->getKey(dir)){
            moveDir(dir);
        }
    }
    

    
    
    
}



//brick class
Brick::Brick(int imageID, int startX, int startY,StudentWorld* ptr):Actor(imageID,startX,startY,ptr){
    setVisible(true);
}


Perma_Brick::Perma_Brick(int imageID, int startX, int startY,StudentWorld* ptr):Brick(imageID,startX,startY,ptr){
    setVisible(true);
}

Destroyable_Brick::Destroyable_Brick(int imageID, int startX, int startY,StudentWorld* ptr):Brick(imageID,startX,startY,ptr){
    setVisible(true);
}

Zumi::Zumi(int imageID, int startX, int startY,StudentWorld* ptr):Actor(imageID,startX,startY,ptr){
    setVisible(true);
}


SimpleZumi::SimpleZumi(int imageID, int startX, int startY,StudentWorld* ptr):Zumi(imageID,startX,startY,ptr){
    setVisible(true);
}

ComplexZumi::ComplexZumi(int imageID, int startX, int startY,StudentWorld* ptr):Zumi(imageID, startX, startY,ptr){
    setVisible(true);
}


//exit class
Exit::Exit(int imageID, int startX, int startY,StudentWorld* ptr):Actor(imageID,startX,startY,ptr){
    setVisible(true);
}

    
