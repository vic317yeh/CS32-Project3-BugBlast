

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



int StudentWorld::init(){
    curLev=getLevel();
    int myLevel=loadLevel();
    
    
    if (curLev==0&&myLevel==Level::load_fail_file_not_found)
        return GWSTATUS_NO_FIRST_LEVEL;
    
    else if (curLev!=0&&myLevel==Level::load_fail_file_not_found)
        return GWSTATUS_PLAYER_WON;
    
    else if(myLevel==Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    
    else
        return GWSTATUS_CONTINUE_GAME;
}

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
                    m_actors.push_back(new SimpleZumi(IID_SIMPLE_ZUMI,i,j,this));
                if (item==Level::complex_zumi)
                    m_actors.push_back(new ComplexZumi(IID_COMPLEX_ZUMI,i,j,this));
                if (item==Level::exit)
                    m_actors.push_back(new Exit(IID_EXIT,i,j,this));
        
        }
    }
    
    
    return Level::load_success;
}


Actor* StudentWorld::getExit(){
    for (int i=0;i<m_actors.size();i++){
        if (typeid(*m_actors[i])==typeid(Exit))
            return m_actors[i];}
    

    return NULL;
}

int StudentWorld::move(){
    m_player->doSomething();
    
    
    
    //if the player is at the exit
    if (m_player->getX()==getExit()->getX()&&m_player->getY()==getExit()->getY()){
        increaseLevel();
        return GWSTATUS_FINISHED_LEVEL;
        
    }
    
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp(){
    decLives();
    delete m_player;
    
    
    while(!m_actors.empty()){
        delete m_actors.back();
        m_actors.pop_back();
    }

}


bool StudentWorld::hasDbrick(int x, int y){
    if (lev.getContentsOf(x, y)==Level::destroyable_brick)
        return true;
    else return false;
}

bool StudentWorld::hasPbrick(int x, int y){
    if (lev.getContentsOf(x, y)==Level::perma_brick)
        return true;
    else return false;
}

void StudentWorld::increaseLevel(){
    curLev++;
}

