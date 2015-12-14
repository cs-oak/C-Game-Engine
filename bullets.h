#ifndef BULLETS__H
#define BULLETS__H
#include <string>
#include <vector>
#include "drawable.h"

class bullets : public Drawable {
public:
  bullets(const std::string&);
  bullets(const bullets&);
  virtual ~bullets() {  } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  bool collidedWith(Drawable*) const;
  int getType() const { return type; }
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
  int speedX;
  int speedY;
  int type;
  void advanceFrame(Uint32 ticks);
private:
  bullets& operator=(const bullets&);
};
#endif
