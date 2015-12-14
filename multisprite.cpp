#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "SDL/SDL_rotozoom.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}




MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")*rand()%Gamedata::getInstance().getXmlInt("world/width"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")*rand()%Gamedata::getInstance().getXmlInt("world/height")), 
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
  zoom(1.0/(rand()%3 + 1)),
  temp(name),
  velx(Gamedata::getInstance().getXmlInt(name+"/speedX")),
  vely(Gamedata::getInstance().getXmlInt(name+"/speedY"))
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
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
  zoom(s.zoom),
  temp(s.temp),
  velx(s.velx),
  vely(s.vely)
  { }

void MultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  SDL_Surface* surf= zoomSurface(frames[currentFrame]->getSurface(), zoom, zoom, SMOOTHING_ON);
  Frame* xyz = new Frame(temp, surf);
  xyz->draw(x, y);
  delete xyz;
  SDL_FreeSurface(surf);
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  velocityX(velx * zoom);
  velocityY(vely * zoom);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    Y(0); X(X()+(rand()%4 + 1)*40);
  }

  if ( X() <= 0) {
    X(worldWidth-frameWidth);
  }
  if ( X() > worldWidth-frameWidth) {
    X(0);
  }  

}

float MultiSprite::getZoom() const {
  return zoom;
}
