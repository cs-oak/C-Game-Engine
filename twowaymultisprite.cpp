#include "twowaymultisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <ctime>

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;

	}
}

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :
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
  explosion(NULL),
  isDrawing(true),
  speedX(Gamedata::getInstance().getXmlInt(name+"/speedX")),
  speedY(Gamedata::getInstance().getXmlInt(name+"/speedY")),
  type(Gamedata::getInstance().getXmlInt(name+"/type"))
{ 
  srand(time(NULL));
}

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s) :
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
  explosion(s.explosion),
  isDrawing(s.isDrawing),
  speedX(s.speedX),
  speedY(s.speedY),
  type(s.type)
  { }

void TwoWayMultiSprite::draw() const { 
  if (explosion) {
    explosion->draw();
    return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void TwoWayMultiSprite::explode() { 
  if ( explosion ) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite); 
}

void TwoWayMultiSprite::update(Uint32 ticks) {
  isDrawing = false;
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  } 
  isDrawing = true;
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    Y(worldHeight - 500);
  }
  if ( Y() > worldHeight-frameHeight) {
    Y(0);
  }

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

bool TwoWayMultiSprite::collidedWith(Drawable *d) const {
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

bool TwoWayMultiSprite::followX(const Drawable *d) {
  if ( abs(d->X() - X()) < 50 && ( abs(d->Y() - Y()) < 400 )) return true;
  return false; 
}

bool TwoWayMultiSprite::followY(const Drawable *d) {
  if ( ( abs(d->Y() - Y()) == 0 ) && ( Y() < d->Y() ) ) return true;
  return false;
}
