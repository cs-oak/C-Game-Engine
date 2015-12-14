#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")*rand()%Gamedata::getInstance().getXmlInt("world/width"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")*rand()%Gamedata::getInstance().getXmlInt("world/height")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")*(rand()%3 +1)) 
           ),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  zoom(1.0/(rand()%3 + 1))
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel), 
  frame( FrameFactory::getInstance().getFrame(n) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  zoom(1.0/(rand()%3 + 1))
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel), 
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  zoom(1.0/(rand()%3 + 1))
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s), 
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  zoom(s.zoom)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  zoom = rhs.zoom;
  return *this;
}

void Sprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

/* Changed to make them wrap around instead of bounce */

  if ( Y() < 0) {
    //velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight/*-frameHeight*/) {
    Y(0);
    X(X()+160);
  }

  if ( X() < 0) {
    //velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth/*-frameWidth*/) {
    X(0);
  }  
}
