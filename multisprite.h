#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&);
  virtual ~MultiSprite() { }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  float getZoom() const;
  struct spriteCompare {
   bool operator()(MultiSprite* left, MultiSprite* right) {
	return left->getZoom() < right->getZoom();
   }
  };

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
  float zoom;
  std::string temp;
  float velx;
  float vely;

  void advanceFrame(Uint32 ticks);
};
#endif
