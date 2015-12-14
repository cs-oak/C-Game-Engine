#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"

void player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    		if (velocityX() > 0) currentFrame = 2;
		if (velocityX() < 0) currentFrame = 0;
		if (velocityX() == 0) currentFrame = 1;
		timeSinceLastFrame = 0;
	}
}

player::player(const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(0,Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  prj(),
  laser(new bullets("laser")),
  mag(new bullets("sheild")),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  
  bdraw(false),
  ldraw(false),
  sdraw(false),
  startX(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")),
  startY(Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
  speedX(Gamedata::getInstance().getXmlInt(name+"/speedX")),
  speedY(Gamedata::getInstance().getXmlInt(name+"/speedY")),
  explosion(NULL)
{ 
  prj.reserve(3);
  for(int i=0;i<3;i++) {
	prj.push_back(new bullets("bullets"));
	prj[i]->X(X() + frameWidth/3 - 3);
	prj[i]->Y(Y() - frameHeight/2 + i*frameHeight);
	prj[i]->velocityX(velocityX());
	prj[i]->velocityY(-600);
  }
  laser->setVelocity(this->getVelocity());
  laser->X(X() - frameWidth/3 + 5);
  laser->Y(Y() - 10*frameHeight);
  mag->setVelocity(this->getVelocity());
  mag->X(X() - frameWidth/3 - 5);
  mag->X(Y() - frameHeight/3 + 6);
}


void player::draw() const {
  if (explosion) {
    explosion->draw();
    return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
  if(sdraw) mag->draw();
  if(ldraw) laser->draw();
  if(bdraw) 
	for(int i=0;i<3;i++) if(prj[i]->Y() < Y() - frameHeight/2) prj[i]->draw();
}

void player::explode() { 
  if ( explosion ) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite); 
}

void player::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    reset();
    }
    return;
  }   
  Uint8 *keys = SDL_GetKeyState(NULL);
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  for(int i=0;i<3;i++) {
	if(!bdraw) prj[i]->Y(Y() - frameHeight/2 + i*frameHeight);
	prj[i]->X(X() + frameWidth/3 - 3);
	//bullets[i]->velocityY(-600);
	if(abs(prj[i]->Y() - Y()) > 350) prj[i]->Y(Y() - frameHeight/2 + i*frameHeight);
	prj[i]->update(ticks);
  }
  setPosition(getPosition() + incr);
  laser->X(this->X() - frameWidth/3 + 5);
  laser->Y(this->Y()-10*frameHeight);
  mag->X(this->X() - frameWidth/3 -5);
  mag->Y(this->Y() - frameHeight/3 + 6);

  if ((keys[SDLK_d])&&(X()<=worldWidth-frameWidth)) {
	velocityX(speedX);
  }
  else if ((keys[SDLK_a])&&(X()>=0)) {
	velocityX(-speedX);
  }
  else {
  	if((velocityX()>10||velocityX()<-10) && X()>0 && X()<worldWidth-frameWidth) velocityX(velocityX()*0.88);
	else velocityX(0);
  }


  if ((keys[SDLK_w])&&(Y()>0)) {
	velocityY(speedY-100);
  }
  else if ((keys[SDLK_s])&&(Y()<=worldHeight-frameHeight)) {
	velocityY(speedY+100);
  }
  else velocityY(speedY);

  if(keys[SDLK_v]) {
	ldraw = true;
  }
  else ldraw = false;

  if(keys[SDLK_n]) {
	sdraw = true;
  }
  else sdraw = false;

  if(keys[SDLK_SPACE]) {
	bdraw = true;
  }
  else bdraw = false;
}

bool player::collidedWith(Drawable *d) const {
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

void player::reset() {
  X(startX);
  Y(startY);
}

bool::player::isFinished() {
  if ( Y() <= 100 ) return true;
  return false;
}
