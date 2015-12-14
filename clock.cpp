#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  frames(0),
  ticksSinceLastFrame(0),
  ticksAtLastFrame(0),
  sumOfTicks(0),
  started(false), 
  paused(false), 
  sloMo(false), 
  fpsLoc(Gamedata::getInstance().getXmlInt("clock/fpsLoc/x"),
         Gamedata::getInstance().getXmlInt("clock/fpsLoc/y")),
  secondsLoc(Gamedata::getInstance().getXmlInt("clock/secondsLoc/x"),
         Gamedata::getInstance().getXmlInt("clock/secondsLoc/y")),

  capFrameRate(true),
  maxFPS(Gamedata::getInstance().getXmlInt("frameCap"))
{
    start();
}

Clock::Clock(const Clock& c) :
  frames(c.frames), 
  ticksSinceLastFrame(c.ticksSinceLastFrame), 
  ticksAtLastFrame(c.ticksAtLastFrame), 
  sumOfTicks(c.sumOfTicks),
  started(c.started), 
  paused(c.paused), 
  sloMo(c.sloMo), 
  fpsLoc(c.fpsLoc),
  secondsLoc(c.secondsLoc),
  capFrameRate(c.capFrameRate),
  maxFPS(c.maxFPS)
{
  // The next line seems wrong; so I will delete it!
  //start();
}

void Clock::draw() const { 
  IOManager::getInstance().
    printMessageValueAt("fps: ", getFps(), fpsLoc[0], fpsLoc[1]);
  IOManager::getInstance().
    printMessageValueAt("seconds: ", getSeconds(), 
                                     secondsLoc[0], secondsLoc[1]);
}

void Clock::update() { 
  if ( paused ) return;
  ++frames;
  unsigned int clockTicks = SDL_GetTicks(); 
  ticksSinceLastFrame = clockTicks - ticksAtLastFrame;
  if( (isCapped()) && (ticksSinceLastFrame < (1000/maxFPS)) ) {
	SDL_Delay((float)(1000/maxFPS) - ticksSinceLastFrame);
	clockTicks = SDL_GetTicks();
  }
  ticksSinceLastFrame = clockTicks - ticksAtLastFrame;
  sumOfTicks += ticksSinceLastFrame;
  ticksAtLastFrame = clockTicks;
}

unsigned int Clock::getTicksSinceLastFrame() const {
  if (paused) return 0;
  if (sloMo) return ticksSinceLastFrame/2;
  return ticksSinceLastFrame;
}

void Clock::toggleSloMo() {
  sloMo = !sloMo;
}

int Clock::getFps() const { 
  if ( getSeconds() > 0 && frames > 0) {
	
	return (frames*1000)/sumOfTicks;
  }
  return 0;
}

void Clock::toggleCap() {
  capFrameRate = !capFrameRate;
  frames = 0;
  sumOfTicks = 0;
}

void Clock::start() { 
  started = true; 
  paused = false;  
  frames = 0;
  ticksAtLastFrame = SDL_GetTicks(); 
}

void Clock::pause() {
  if( started && !paused ) {
    paused = true;
  }
}
void Clock::unpause() {
  if( started && paused ) {
    ticksAtLastFrame = SDL_GetTicks();
    paused = false;
  }
}
