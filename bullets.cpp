#include "bullets.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <ctime>

void bullets::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;

	}
}

bullets::bullets( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    rand()%(Gamedata::getInstance().getXmlInt("world/height")-300)), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  speedX(Gamedata::getInstance().getXmlInt(name+"/speedX")),
  speedY(Gamedata::getInstance().getXmlInt(name+"/speedY")),
  type(Gamedata::getInstance().getXmlInt(name+"/type"))
{ 
  srand(time(NULL));
}

bullets::bullets(const bullets& s) :
  Drawable(s), 
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  speedX(s.speedX),
  speedY(s.speedY),
  type(s.type)
  { }

void bullets::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void bullets::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);


  if ( X() < -50) {
    velocityX( abs( velocityX() ) );
    velocityX(rand()%speedX + 150);
    if(type == 0) velocityY(rand()%speedY + 150);
    if(type == 1) velocityY(speedY);
    Y(rand()%(worldHeight -300));
  }
  if ( X() > worldWidth-frameWidth+50) {
    velocityX( -abs( velocityX() ) );
    if(type == 0) velocityY(rand()%speedY + 150);
    if(type == 1) velocityY(speedY);
    Y(rand()%(worldHeight -300));
  }  

}

bool bullets::collidedWith(Drawable *d) const {
  float left1 = X();
  float left2 = d->X();
  float right1 = left1+getFrame()->getWidth();
  float right2 = left2+d->getFrame()->getWidth();
  if ( right1 < left2 ) return false;
  if ( left1 > right2 ) return false;
  float top1 = Y();
  float top2 = d->Y();
  float bottom1 = top1+getFrame()->getHeight();
  float bottom2 = top2+d->getFrame()->getHeight();
  if ( bottom1 < top2 ) return false;
  if ( bottom2 < top1 ) return false;
  return true;
}
