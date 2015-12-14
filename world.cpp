#include <iostream>
#include "world.h"
#include "frameFactory.h"

World::World(const std::string& name, int fact) : 
  io( IOManager::getInstance() ),
  frame( FrameFactory::getInstance().getFrame(name) ),
  factor(fact),
    frameWidth( frame->getHeight() ),
    worldWidth( Gamedata::getInstance().getXmlInt("world/height") ),
    viewX(0.0), viewY(0.0), 
    view(Viewport::getInstance()) 
{}

void World::update() {
  viewX = view.X();
  viewY = static_cast<int>(view.Y() / factor) % frameWidth;
}

void World::draw() const { 
  frame->draw(viewX, viewY, 0, 0); 
  frame->draw(viewX, 0, 0, frameWidth-viewY); 
}

