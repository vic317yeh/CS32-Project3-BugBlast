#include "StudentWorld.h"

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


StudentWorld::StudentWorld(){

}

StudentWorld::~StudentWorld(){
    delete m_player;
    
    while(!m_actors.empty())
        delete m_actors.back();
}


//init function
int StudentWorld::init(){
    finished=false;
    curLev=getLevel();
    int myLevel=loadLevel();
    bonus=lev.getOptionValue(optionLevelBonus);
    
    if (curLev==0&&myLevel==Level::load_fail_file_not_found)
        return GWSTATUS_NO_FIRST_LEVEL;
    
    else if (curLev!=0&&myLevel==Level::load_fail_file_not_found)
        return GWSTATUS_PLAYER_WON;
    
    else if(myLevel==Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    
    else
        return GWSTATUS_CONTINUE_GAME;
}


//load the current level file
int StudentWorld::loadLevel(){
    
    char num=48+curLev;
    
    string curLevel = "level0";
    curLevel+=num;
    curLevel+=".dat";
    
    
    
    Level::LoadResult result = lev.loadLevel(curLevel,"/Users/vic317_yeh/Desktop/Winter14/CS32/BugBlast/DerivedData/BugBlast/Build/Products/Debug");
    
    
    if (result == Level::load_fail_file_not_found )
        return Level::load_fail_file_not_found;
    
    else if(result == Level::load_fail_bad_format )
        return Level::load_fail_bad_format;
    
    
    for (int i=0;i<VIEW_WIDTH;i++){
        for (int j=0; j< VIEW_HEIGHT;j++){
            Level::MazeEntry item = lev.getContentsOf(i,j);
                if (item==Level::player)
                    m_player=new Player(IID_PLAYER,i,j,this);
                if (item==Level::perma_brick)
                    m_actors.push_back(new Perma_Brick(IID_PERMA_BRICK,i,j,this));
                if (item==Level::destroyable_brick)
                    m_actors.push_back(new Destroyable_Brick(IID_DESTROYABLE_BRICK,i,j,this));
                if (item==Level::simple_zumi)
                    m_actors.push_back(new SimpleZumi(IID_SIMPLE_ZUMI,i,j,this,lev.getOptionValue(optionTicksPerSimpleZumiMove)));
                if (item==Level::complex_zumi)
                    m_actors.push_back(new ComplexZumi(IID_COMPLEX_ZUMI,i,j,this,lev.getOptionValue(optionTicksPerComplexZumiMove)));
                if (item==Level::exit)
                    m_actors.push_back(new Exit(IID_EXIT,i,j,this));
        
        }
    }
    
    
    return Level::load_success;
}


//move function
int StudentWorld::move(){
    // Update the Game Status Line
    updateDisplayText();     
    
    m_player->doSomething();
    
    for (int i=0;i<m_actors.size();i++){
        if ( m_actors[i]->isAlive())
            m_actors[i]->doSomething();
        if (!m_player->isAlive())
            return GWSTATUS_PLAYER_DIED;
    }
    
    //remove every dead object in the array
    removeDeadObjects();
    
    //decrease level bonus score by one
    decBonus();
    
    //if the player is killed during the current tick
    if (!m_player->isAlive())
        return GWSTATUS_PLAYER_DIED;
    
    //if the player is at the exit
    if (levelFinished()){
        increaseLevel();
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

//cleanup function
void StudentWorld::cleanUp(){
    delete m_player;
    
    
    while(!m_actors.empty()){
        delete m_actors.back();
        m_actors.pop_back();
    }
}


//return whether the level is finished
bool StudentWorld::levelFinished() const{
    return finished;
}


//set the level to be complete
void StudentWorld::setLevelFinished(){
    finished=true;
}

//check if there is a brick at x and y
bool StudentWorld:: hasbrick(int x, int y){
    bool flag=false;
    
    for (int i=0; i< m_actors.size();i++){
        if(typeid(*m_actors[i])==typeid(Destroyable_Brick)||typeid(*m_actors[i])==typeid(Perma_Brick))
            if (m_actors[i]->getX()==x&&m_actors[i]->getY()==y)
                flag=true;}
    
    return flag;
    

}


//check if there is a destroyable brick at x and y
bool StudentWorld::hasDbrick(int x, int y){
    bool flag=false;
    
    for (int i=0; i< m_actors.size();i++){
        if(typeid(*m_actors[i])==typeid(Destroyable_Brick))
            if (m_actors[i]->getX()==x&&m_actors[i]->getY()==y)
                flag=true;}
    
    return flag;
}


//check if there is a permanent brick at x and y
bool StudentWorld::hasPbrick(int x, int y){
    bool flag=false;
    
    for (int i=0; i< m_actors.size();i++){
        if(typeid(*m_actors[i])==typeid(Perma_Brick))
            if (m_actors[i]->getX()==x&&m_actors[i]->getY()==y)
                flag=true;}
    
    return flag;
}

//increase the current level by 1
void StudentWorld::increaseLevel(){
    curLev++;
}

//push a new actor into the vector
void StudentWorld::addActor(Actor* a){
    m_actors.push_back(a);
}

//apply bug spray to each actor at this position
void StudentWorld::applyBugSprayToActorsAt(int x, int y){
    if (m_player->getX()==x && m_player->getY()==y)
        m_player->applyBugSpray();
    
    for (int i=0; i< m_actors.size();i++){
        if(m_actors[i]->getX()==x && m_actors[i]->getY()==y){
            m_actors[i]->applyBugSpray();
        }
    }
}

//remove dead objects from the array
void StudentWorld::removeDeadObjects(){
//    if (!m_player->isAlive())
//        delete m_player;
    
    for (int i=0; i< m_actors.size();i++){
        if(!m_actors[i]->isAlive()){
            delete m_actors[i];
            m_actors.erase(m_actors.begin()+i);
        }
    }
}

//decrease player's bug sprayer used by 1
void StudentWorld::playerIncreaseBugSprayer(){
    m_player->increaseBugSprayer();
}

//check if there is a bug sprayer on the current cooridnate
bool StudentWorld::hasBugSprayerAt(int x, int y){
    for (int i=0; i< m_actors.size();i++){
        if(typeid(*m_actors[i])==typeid(BugSprayer))
            return m_actors[i]->getX()==x&&m_actors[i]->getY()==y;}
    return false;
    
}

//check if there is any zumi left
bool StudentWorld::anyLiveZumi() const{
    for (int i=0; i< m_actors.size();i++){
        if(typeid(*m_actors[i])==typeid(SimpleZumi)||typeid(*m_actors[i])==typeid(ComplexZumi))
            return true;}
    return false;
}

//get player's current coordinate
void StudentWorld::getPlayerLocation(int& x, int& y) const{
    x=m_player->getX();
    y=m_player->getY();
}

// Does this location contain an actor that blocks a Zumi from being
// moved to here or a bug sprayer from being dropped here?
bool StudentWorld::zumiAndSprayersBlockedAt(int x, int y) const{
    for (int i=0; i< m_actors.size();i++){
        if(typeid(*m_actors[i])==typeid(SimpleZumi)||typeid(*m_actors[i])==typeid(ComplexZumi)||typeid(*m_actors[i])==typeid(Perma_Brick)||typeid(*m_actors[i])==typeid(Destroyable_Brick)||typeid(*m_actors[i])==typeid(BugSprayer))
            
            if (m_actors[i]->getX()==x&&m_actors[i]->getY()==y)
            return true;
    }
    
    return false;
}



//check if there is zumi at the current coordinate
bool StudentWorld::hasZumiAt(int x, int y) const{
    for (int i=0; i< m_actors.size();i++){
        if(typeid(*m_actors[i])==typeid(SimpleZumi)||typeid(*m_actors[i])==typeid(ComplexZumi))
            if (m_actors[i]->getX()==x&&m_actors[i]->getY()==y)
                return true;}
    return false;
}

//set game text
void StudentWorld::updateDisplayText() {
    int score = getScore();
    int level = getLevel();
    unsigned int bonus = getBonus();;
    int livesLeft = getLives();
    // Next, create a string from your statistics, of the form: // "Score: 00000100 Level: 09 Lives: 003 Bonus: 345"
    string s = setDisplayText(score,level,livesLeft,bonus);
    // Finally, update the display text at the top of the screen with your // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

//check number of digit in a number
int numDigits(int number)
{
    if (number==0)
        return 1;
    
    int digits = 0;
    
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

//sets the display text in the correct manner
string StudentWorld::setDisplayText(int score, int level, int livesLeft, unsigned int bonus){
    string returnMe;
    
    //keep track of how many zeros to put in the front
    int temp(0);
    
    //set text for score
    
    temp=8-numDigits(score);
    string tempString1(temp,'0');
    tempString1+=std::to_string(score);
    returnMe+="Score: "+tempString1+" Level: ";
    
    //set text for level
    temp=2-numDigits(level);
    string tempString2(temp,'0');
    tempString2+=std::to_string(level);
    returnMe+=tempString2+" Lives: ";
    
    //set text lives
    temp=3-numDigits(livesLeft);
    string tempString3(temp,'0');
    tempString3 += std::to_string(livesLeft);;
    returnMe+= tempString3 +" Bonus: ";
    
    //set bonus
    returnMe+=std::to_string(bonus);;

    return returnMe;
}

//set player dead
void StudentWorld::setPlayerDead(){
    m_player->setDead();
}

//return bonus point
int StudentWorld::getBonus() const{
    return bonus;
}


//decrease bonus score by 1
void StudentWorld::decBonus(){
    if (bonus>0)
        bonus--;
}

//set player able to penetrate wall
void StudentWorld::activatePlayerWalkThroughWalls(){
    if (!m_player->canPenetrateWall())
        m_player->switchPenetrateWall();
    m_player->setTickWall(lev.getOptionValue(optionWalkThruLifetimeTicks));
}

//increase the number of bug sprayer that can be dropped
void StudentWorld::increasePlayerSimultaneousSprayers(){
    m_player->setTickBugSprayer(lev.getOptionValue(optionBoostedSprayerLifetimeTicks));
    m_player->setMaxBugSprayer(lev.getOptionValue(optionMaxBoostedSprayers));
    
}


unsigned int StudentWorld::getProbOfGoodieOverall() const{
    return lev.getOptionValue(optionProbOfGoodieOverall);
}

unsigned int StudentWorld::getProbOfExtraLifeGoodie() const{
    return lev.getOptionValue(optionProbOfExtraLifeGoodie);
}

unsigned int StudentWorld::getProbOfWalkThruGoodie() const{
    return lev.getOptionValue(optionProbOfWalkThruGoodie);

}

unsigned int StudentWorld::getProbOfMoreSprayersGoodie() const{
    return lev.getOptionValue(optionProbOfMoreSprayersGoodie);
}


unsigned int StudentWorld::getGoodieLifetimeInTicks() const{
    return lev.getOptionValue(optionGoodieLifetimeInTicks);
}

unsigned int StudentWorld::getComplexZumiSearchDistance() const{
    return lev.getOptionValue(optionComplexZumiSearchDistance);
}


