#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
#include "explodingSprite.h"

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  virtual ~TwoWayMultiSprite() { delete explosion; } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  bool collidedWith(Drawable*) const;
  void explode();
  bool isDrw() const { return isDrawing; }
  bool followX(const Drawable*);
  bool followY(const Drawable*);
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
  ExplodingSprite* explosion;
  bool isDrawing;
  int speedX;
  int speedY;
  int type;
  void advanceFrame(Uint32 ticks);
private:
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
};
#endif
