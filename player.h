#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "bullets.h"
#include "drawable.h"
#include "multisprite.h"
#include "twowaymultisprite.h"

class player : public Drawable {
public:
  player(const std::string&);
  virtual ~player() {
	delete laser;
	delete mag;
	for (unsigned i = 0; i < prj.size(); ++i) {
	  delete prj[i];
	delete explosion;
	}
} 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
 }
 const player* getInst() const {
    return this;
 }
 bool collidedWith(Drawable *d) const;
 bool getBdraw() const { if(explosion) return false; return bdraw; }
 bool getLdraw() const { if(explosion) return false; return ldraw; }
 bool getSdraw() const { if(explosion) return false; return sdraw; }
 void reset();
 void explode();
 bool isFinished();
 std::vector<bullets*> prj;
 bullets* laser;
 bullets* mag;
protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  bool bdraw;
  bool ldraw;
  bool sdraw;
  int startX;
  int startY;
  int speedX;
  int speedY;
  ExplodingSprite* explosion;
  void advanceFrame(Uint32 ticks);
  
private:
  player(const player&);
  player& operator=(const player&);
};
#endif
