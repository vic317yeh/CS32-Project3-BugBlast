#include "Actor.h"
#include "StudentWorld.h"
#include <string>
#include <queue>

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor class
Actor::Actor(int imageID, int startX, int startY,StudentWorld* ptr):GraphObject(imageID,startX,startY){
    actorID=imageID;
    st_p=ptr;
    m_alive=true;
}

void Actor::setDead(){
    m_alive=false;
}

StudentWorld* Actor::getWorld(){
    return st_p;
}

bool Actor::isAlive() const{
    return m_alive;
}



//player class
Player::Player(int imageID, int startX, int startY,StudentWorld* ptr):Actor(imageID,startX,startY,ptr){
    setVisible(true);
    penetrateWall=false;
    bugSprayerUsed=0;
    maxSprayer=2;
    m_tick_wall=0;
    m_tick_bugSprayer=0;
}

void Player::increaseBugSprayer(){
    if (bugSprayerUsed>0)
        bugSprayerUsed--;
}


bool Player::canPenetrateWall(){
    return penetrateWall;
}

bool Player::bugSprayerLeft(){
    return bugSprayerUsed<maxSprayer;
}

void Player::switchPenetrateWall(){
    if (penetrateWall==true)
        penetrateWall=false;
    else penetrateWall=true;
}

//set the number of tick player can have while being able to walk through the wall
void Player::setTickWall(int n){
    m_tick_wall=n;
}

//set the number of tick player can have while having the ability to drop move bug sprayers
void Player::setTickBugSprayer(int n){
    m_tick_bugSprayer=n;
}

//set the maximum number of the bug sprayers that can be drop simultaneouly by the player
void Player::setMaxBugSprayer(int n){
    maxSprayer=n;
}

//move direction function while the player can move through the wall
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
            if (bugSprayerLeft()&&(!(getWorld()->zumiAndSprayersBlockedAt(getX(),getY())))){
                    bugSprayerUsed++;
                    getWorld()->addActor(new BugSprayer(IID_BUGSPRAYER,getX(),getY(),getWorld()));}
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
                if (bugSprayerLeft()&&(!(getWorld()->zumiAndSprayersBlockedAt(getX(),getY())))){
                        bugSprayerUsed++;
                        getWorld()->addActor(new BugSprayer(IID_BUGSPRAYER,getX(),getY(),getWorld()));}
                 break;
        }
}


void Player::applyBugSpray(){
    getWorld()->playSound(SOUND_PLAYER_DIE);
    getWorld()->decLives();
    setDead();
}

void Player::doSomething(){
    if (!isAlive()){
        return;
    }
    
    //check if at the same location as zombie
    if (getWorld()->hasZumiAt(getX(),getY())){
        getWorld()->playSound(SOUND_PLAYER_DIE);
        getWorld()->decLives();
        setDead();
    }
    
    //check if is at the same location as brick while the ability to penetrate wall expires
    if (getWorld()->hasbrick(getX(), getY())&&!canPenetrateWall()){
        getWorld()->playSound(SOUND_PLAYER_DIE);
        getWorld()->decLives();
        setDead();
    }

    
    //if the player can penetrate the wall
    if (canPenetrateWall()){
        //decrease tick by one
        m_tick_wall--;
        
        if (m_tick_wall<=0)
            switchPenetrateWall();
    }
    
    
    //if at the state of dropping more bug sprayers
    if (m_tick_bugSprayer>0){
        m_tick_bugSprayer--;
        
        if (m_tick_bugSprayer<=0)
            setMaxBugSprayer(2);
    }
    
    //variable for saving user input
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


void Destroyable_Brick::applyBugSpray(){
    setDead();
}


//zumi class
Zumi::Zumi(int imageID, int startX, int startY,StudentWorld* ptr, int N):Actor(imageID,startX,startY,ptr){
    setVisible(true);
    speed=N;
    count=0;
}

void Zumi::applyBugSpray(){
    setDead();
    getWorld()->playSound(SOUND_ENEMY_DIE);
}

int Zumi::getSpeed() const{
    return speed;
}

int Zumi::getCount() const{
    return count;
}

void Zumi::setCount(int x){
    count=x;
}

//simple zumi class
SimpleZumi::SimpleZumi(int imageID, int startX, int startY,StudentWorld* ptr, int N):Zumi(imageID,startX,startY,ptr, N){
    setVisible(true);
    srand (time(NULL));
    curDir=rand()%4;
}

void SimpleZumi::applyBugSpray(){
    Zumi::applyBugSpray();
    getWorld()->playSound(SOUND_ENEMY_DIE);
    getWorld()->increaseScore(100);
    srand (time(NULL));
    int num=rand()%100;
    int chance=getWorld()->getProbOfGoodieOverall();
    
    if (num<chance){
        
        //chance for dropping an extra life goodie
        int num2=rand()%100;
        
        int chance1=getWorld()->getProbOfExtraLifeGoodie();
        int chance2=getWorld()->getProbOfMoreSprayersGoodie();
        
        if (num2<chance1)
            getWorld()->addActor(new ExtraLifeGoodie(IID_EXTRA_LIFE_GOODIE, getX(), getY(), getWorld(), getWorld()->getGoodieLifetimeInTicks()));
        
        //chance for dropping an increase simultaneous sprayers goodie
        else if (num2>(100-chance2))
                getWorld()->addActor(new IncreaseSimultaneousSprayersGoodie(IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, getX(), getY(), getWorld(), getWorld()->getGoodieLifetimeInTicks()));
        
        //chance for dropping a walk through wall goodie
        else {
            getWorld()->addActor(new WalkThroughWallsGoodie(IID_WALK_THRU_GOODIE, getX(), getY(), getWorld(), getWorld()->getGoodieLifetimeInTicks()));}
    }
    
}

void SimpleZumi::move(){
    
    switch(curDir){
        //up
        case 0:
            if (!getWorld()->zumiAndSprayersBlockedAt(getX(),getY()+1)){
                moveTo(getX(),getY()+1);
            }
        
            else curDir=rand()%4;
            
            break;
        //down
        case 1:
            if (!getWorld()->zumiAndSprayersBlockedAt(getX(),getY()-1)){
                moveTo(getX(),getY()-1);
            }
            
            else curDir=rand()%4;
                
                break;
        
        //left
        case 2:
            if (!getWorld()->zumiAndSprayersBlockedAt(getX()-1,getY())){
                moveTo(getX()-1,getY());
            }
            
            else curDir=rand()%4;
                
            break;
        
        
        //right
        case 3:
            if (!getWorld()->zumiAndSprayersBlockedAt(getX()+1,getY())){
                moveTo(getX()+1,getY());
            }
            
            else curDir=rand()%4;
                
            break;
            
        default:break;
    }
}

void SimpleZumi::doSomething(){
    if (!isAlive())
        return;
    
    //check if player is at the same location
    int x(-1);
    int y(-1);
    getWorld()->getPlayerLocation(x, y);
    if (x==getX()&&y==getY()){
        getWorld()->setPlayerDead();
    }
    
    if (getCount()<getSpeed())
        setCount(getCount()+1);
        
    else {
        setCount(0);
        move();
    }
    
}


//complex zumi class
ComplexZumi::ComplexZumi(int imageID, int startX, int startY,StudentWorld* ptr, int N):Zumi(imageID, startX, startY,ptr, N){
    setVisible(true);
    srand (time(NULL));
    curDir=rand()%4;
}


void ComplexZumi::applyBugSpray(){
    Zumi::applyBugSpray();
    getWorld()->playSound(SOUND_ENEMY_DIE);
    getWorld()->increaseScore(500);
    srand (time(NULL));
    int num=rand()%100;
    int chance=getWorld()->getProbOfGoodieOverall();
    
    if (num<chance){
        
        //chance for dropping an extra life goodie
        int num2=rand()%100;
        
        int chance1=getWorld()->getProbOfExtraLifeGoodie();
        int chance2=getWorld()->getProbOfMoreSprayersGoodie();
        
        if (num2<chance1)
            getWorld()->addActor(new ExtraLifeGoodie(IID_EXTRA_LIFE_GOODIE, getX(), getY(), getWorld(), getWorld()->getGoodieLifetimeInTicks()));
        
        //chance for dropping an increase simultaneous sprayers goodie
        else if (num2>(100-chance2))
            getWorld()->addActor(new IncreaseSimultaneousSprayersGoodie(IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, getX(), getY(), getWorld(), getWorld()->getGoodieLifetimeInTicks()));
        
        //chance for dropping a walk through wall goodie
        else {
            getWorld()->addActor(new WalkThroughWallsGoodie(IID_WALK_THRU_GOODIE, getX(), getY(), getWorld(), getWorld()->getGoodieLifetimeInTicks()));}
    }
    
}

void ComplexZumi::doSomething(){
    if (!isAlive())
        return;

    //check if player is at the same location
    int x(-1);
    int y(-1);
    getWorld()->getPlayerLocation(x, y);
    if (x==getX()&&y==getY()){
        getWorld()->setPlayerDead();
    }
    
    
    if (getCount()<getSpeed())
        setCount(getCount()+1);
    
    else {
        
        int x(-1);
        int y(-1);
        getWorld()->getPlayerLocation(x, y);
        
        //get the horizontal and vertical distances
        int horD=abs(x-getX());
        int VerD=abs(y-getY());
        int value=getWorld()->getComplexZumiSearchDistance();
        
        //check if the player is within smell distance
        if (horD<=value&&VerD<=value){
            if (getDir()!=-1)
                curDir=getDir();

        }
        
        setCount(0);
        move();
    }
}

//data structure for queue
class Coord
{
public:
    Coord(int xx, int yy , int  direction) : m_x(xx), m_y(yy) ,dir(direction) {}
    int x() const { return m_x; }
    int y() const { return m_y; }
    int direction(){ return dir;}
private:
    int m_x;
    int m_y;
    int dir;
};


//first serach algorithm
int ComplexZumi::getDir()
{
    char maze[15][15];
    for(int x=0;x<15;x++){
        for (int y=0;y<15;y++){
            { if (getWorld()->zumiAndSprayersBlockedAt(x,y))
            {
                maze[14-y][x]='.';
            }
            else{
                maze[14-y][x]=' ';
            }
            }
        }
    }
    
    queue<Coord> q;
    int x=getX();
    int y=getY();
    if(maze[14-y][x+1]!='.')
        q.push(Coord(x+1,y,3));
    if (maze[14-y][x-1]!='.')
        q.push(Coord(x-1,y,2));
    if (maze[13-y][x]!='.')
        q.push(Coord(x,y+1,0));
    if (maze[15-y][x]!='.')
        q.push(Coord(x,y-1,1));
    
    while(!q.empty())
    {
        Coord b=q.front();
        q.pop();
        int x=b.x();
        int y=b.y();
        int dir=b.direction();
        
        //check if the player is at the same location
        int x1(-1);
        int y1(-1);
        getWorld()->getPlayerLocation(x1,y1);
        if (x1==x&&y1==y)
            return dir;
        
        maze[14-y][x]='.';
        if(maze[14-y][x+1]!='.')
            q.push(Coord(x+1,y,dir));
        if (maze[14-y][x-1]!='.')
            q.push(Coord(x-1,y,dir));
        if (maze[13-y][x]!='.')
            q.push(Coord(x,y+1,dir));
        if (maze[15-y][x]!='.')
            q.push(Coord(x,y-1,dir));
    }
    
    
    return -1;
}


void ComplexZumi::move(){
    switch(curDir){
            //up
        case 0:
            if (!getWorld()->zumiAndSprayersBlockedAt(getX(),getY()+1)){
                moveTo(getX(),getY()+1);
            }
            else curDir=rand()%4;
            break;
            //down
        case 1:
            if (!getWorld()->zumiAndSprayersBlockedAt(getX(),getY()-1)){
                moveTo(getX(),getY()-1);
            }
            else curDir=rand()%4;
            break;
            
            //left
        case 2:
            if (!getWorld()->zumiAndSprayersBlockedAt(getX()-1,getY())){
                moveTo(getX()-1,getY());
            }
            else curDir=rand()%4;
            break;
            
            
            //right
        case 3:
            if (!getWorld()->zumiAndSprayersBlockedAt(getX()+1,getY())){
                moveTo(getX()+1,getY());
            }
            
            else curDir=rand()%4;
            break;
            
        default:break;
    }

}



//exit class
Exit::Exit(int imageID, int startX, int startY,StudentWorld* ptr):Actor(imageID,startX,startY,ptr){
    revealed=false;
}

void Exit::doSomething(){
    if(!getWorld()->anyLiveZumi()){
        
        //check if the exit has been revealed
        if (!revealed){
            setVisible(true);
            getWorld()->playSound(SOUND_REVEAL_EXIT);
            revealed=true;
        }
        
        //create local variables to get the player's location
        int x(-1);
        int y(-1);
        getWorld()->getPlayerLocation(x, y);
        if (x==getX()&&y==getY()){
            getWorld()->setLevelFinished();
            getWorld()->playSound(SOUND_FINISHED_LEVEL);
            
            
            //give bonus point
            getWorld()->increaseScore(getWorld()->getBonus());
        }
    }
}



//bugsprayer class

//constructor
BugSprayer::BugSprayer(int imageID, int startX, int startY,StudentWorld* ptr):Actor(imageID,startX,startY,ptr){
    setVisible(true);
    m_tick=40;
}

//decrease the tick by 1
void BugSprayer::decreaseTick(){
    m_tick--;
}

//check if the bugsprayer is ready to explode
bool BugSprayer::moreTicks(){
    return m_tick>0;
}

//add bug spray objects
void BugSprayer::addBugSprays(){
    for (int i=1; i<=2; i++){
        //creates no bug spray if just killed a zumi
        if (i==2&&getWorld()->hasZumiAt(getX()-1, getY()))
            break;
            
        if (!getWorld()->hasPbrick(getX()-i, getY()))
            getWorld()->addActor(new BugSpray(IID_BUGSPRAY ,getX()-i,getY(),getWorld()));
        if (getWorld()->hasbrick(getX()-i, getY()))
            break;
    }
    
    for (int i=1; i<=2; i++){
        //creates no bug spray if just killed a zumi
        if (i==2&&getWorld()->hasZumiAt(getX()+1, getY()))
            break;
        
        if (!getWorld()->hasPbrick(getX()+i, getY()))
            getWorld()->addActor(new BugSpray(IID_BUGSPRAY ,getX()+i,getY(),getWorld()));
        if (getWorld()->hasbrick(getX()+i, getY()))
            break;
    }
    
    for (int i=1; i<=2; i++){
        //creates no bug spray if just killed a zumi
        if (i==2&&getWorld()->hasZumiAt(getX(), getY()-1))
            break;
        
        if (!getWorld()->hasPbrick(getX(), getY()-i))
            getWorld()->addActor(new BugSpray(IID_BUGSPRAY ,getX(),getY()-i,getWorld()));
        if (getWorld()->hasbrick(getX(), getY()-i))
            break;
    }
    
    for (int i=1; i<=2; i++){
        //creates no bug spray if just killed a zumi
        if (i==2&&getWorld()->hasZumiAt(getX(), getY()+1))
            break;
        
        if (!getWorld()->hasPbrick(getX(), getY()+i))
            getWorld()->addActor(new BugSpray(IID_BUGSPRAY ,getX(),getY()+i,getWorld()));
        if (getWorld()->hasbrick(getX(), getY()+i))
            break;
    }
    

}


//explode the bug sprayer and decrease the player's bug sprayer used by one
void BugSprayer::doSomething(){
    if (!isAlive())
        return;
    
    decreaseTick();
    if (!moreTicks()){
        setDead();
        getWorld()->playerIncreaseBugSprayer();
        addBugSprays();
        getWorld()->playSound(SOUND_SPRAY);
    }
}

void BugSprayer::expireImmediately(){
    m_tick=0;
}

void BugSprayer::applyBugSpray(){
    expireImmediately();
    doSomething();
    setDead();
}



//bugspray class
    
//constructor
BugSpray::BugSpray(int imageID, int startX, int startY,StudentWorld* ptr):Actor(imageID,startX,startY,ptr){
        setVisible(true);
        m_tick=3;
}

void BugSpray::doSomething(){
    if (!isAlive())
        return;
    
    decreaseTick();
    if (!moreTicks())
        setDead();
    
    else {
        getWorld()->applyBugSprayToActorsAt(getX(), getY());
    }
    
}

void BugSpray::decreaseTick(){
    m_tick--;
}

//check if the bugspray has ticks left
bool BugSpray::moreTicks(){
    return m_tick>0;
}


//goodies
Goodie::Goodie(int imageID, int startX, int startY,StudentWorld* ptr,int N):Actor(imageID,startX,startY,ptr){
    setVisible(true);
    m_tick=N;
}

//decrease tick by 1
void Goodie::decreaseTick(){
    if (m_tick>0)
        m_tick--;
}

//check if there are more ticks left
bool Goodie::moreTicks() const{
    return m_tick>0;
}


//extra life goodie
ExtraLifeGoodie::ExtraLifeGoodie(int imageID, int startX, int startY,StudentWorld* ptr,int N):Goodie(imageID,startX,startY,ptr,N){
}

void ExtraLifeGoodie::doSomething(){
    if (!isAlive())
        return;
    
    decreaseTick();
    
    if (!moreTicks()){
        setDead();
    }
    
    //if there are more ticks left
    else {
        int x(-1);
        int y(-1);
        getWorld()->getPlayerLocation(x, y);
        //check if the player is at the same location
        if (getX()==x&&getY()==y){
            getWorld()->incLives();
            getWorld()->increaseScore(1000);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            setDead();
        }
    }
}

//walk through walls goodie
WalkThroughWallsGoodie::WalkThroughWallsGoodie(int imageID, int startX, int startY,StudentWorld* ptr,int N):Goodie(imageID,startX,startY,ptr,N){
}


void WalkThroughWallsGoodie::doSomething(){
    if (!isAlive())
        return;
    
    decreaseTick();
    
    if (!moreTicks()){
        setDead();
    }

    else {
        int x(-1);
        int y(-1);
        getWorld()->getPlayerLocation(x, y);
        //check if the player is at the same location
        if (getX()==x&&getY()==y){
            getWorld()->activatePlayerWalkThroughWalls();
            getWorld()->increaseScore(1000);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            setDead();
        }
    }
}

//increase stimultaneous goodie
IncreaseSimultaneousSprayersGoodie::IncreaseSimultaneousSprayersGoodie (int imageID, int startX, int startY,StudentWorld* ptr,int N):Goodie(imageID,startX,startY,ptr,N){
}

void IncreaseSimultaneousSprayersGoodie::doSomething(){
    if (!isAlive())
        return;
    
    decreaseTick();
    
    if (!moreTicks()){
        setDead();
    }
    
    else {
        int x(-1);
        int y(-1);
        getWorld()->getPlayerLocation(x, y);
        //check if the player is at the same location
        if (getX()==x&&getY()==y){
            getWorld()->increasePlayerSimultaneousSprayers();
            getWorld()->increaseScore(1000);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            setDead();
        }
    }
}

    
