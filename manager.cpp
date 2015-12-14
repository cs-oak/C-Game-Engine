#include <iostream>
#include <string>
#include <iomanip>
#include "player.h"
#include "twowaymultisprite.h"
#include "sprite.h"
#include "multisprite.h"
#include "bullets.h"
#include "gamedata.h"
#include "hud.h"
#include "manager.h"
#include "SDL/SDL_rotozoom.h"
#include "algorithm"
#include "sound.h"

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  for (unsigned i = 0; i < deepsprites.size(); ++i) {
    delete deepsprites[i];
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world("galaxy", Gamedata::getInstance().getXmlInt("galaxy/factor") ),
  woods("woods", Gamedata::getInstance().getXmlInt("woods/factor") ),
  mountain("mountain", Gamedata::getInstance().getXmlInt("mountain/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  deepsprites(),
  currentSprite(0),
  godMode(false),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  HUD(hud::getInstance())
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  MultiSprite::spriteCompare cmp;
  atexit(SDL_Quit);
  sprites.push_back( new player("ship") );
  for (int k=0;k<10;k++) sprites.push_back( new TwoWayMultiSprite("alien") );
  for (int k=0;k<10;k++) sprites.push_back( new TwoWayMultiSprite("alien2") );
  for (int k=0;k<10;k++) sprites.push_back( new TwoWayMultiSprite("alien3") );
  for (int k=0;k<10;k++) sprites.push_back( new TwoWayMultiSprite("crystal") );
  for (int k=0;k<20;k++) deepsprites.push_back( new MultiSprite("fireballs") );
  std::sort(deepsprites.begin(), deepsprites.end(), cmp);
  viewport.setObjectToTrack(sprites[currentSprite]);
}

bool Manager::checkForCollisions() const {
  for (int k=1;k<=40;k++){
  if((static_cast <player *>(sprites[0])->collidedWith(sprites[k])) && (sprites[k]->isDrw()) && !godMode) {
	sprites[0]->explode();
	return true;
  }
  for(std::vector<bullets*>::iterator itr = static_cast <player *>(sprites[0])->prj.begin(); itr != static_cast <player *>(sprites[0])->prj.end();itr++) {
	if((*itr)->collidedWith(sprites[k]) && static_cast <player *>(sprites[0])->getBdraw() && (sprites[k]->getType() == 0))  { sprites[k]->explode(); return true; }
  }
  if(static_cast <player *>(sprites[0])->laser->collidedWith(sprites[k]) && static_cast <player *>(sprites[0])->getLdraw()) { sprites[k]->explode(); return true; }
  if(static_cast <player *>(sprites[0])->mag->collidedWith(sprites[k]) && static_cast <player *>(sprites[0])->getSdraw() && (sprites[k]->getType() == 1)) { 
	sprites[k]->velocityX(-sprites[k]->velocityX()); 
	sprites[k]->velocityY(-sprites[k]->velocityY());
	return true; 
  }
  }
  return false;
}

void Manager::draw() const {
  world.draw();
  for (unsigned i = 0; i < deepsprites.size(); ++i) {
       if(deepsprites[i]->getZoom() < 0.4)
       deepsprites[i]->draw();
  }
  woods.draw();
  for (unsigned i = 0; i < deepsprites.size(); ++i) {
       if(deepsprites[i]->getZoom() == 1.0/2)
       deepsprites[i]->draw();
  }	
  mountain.draw();
  for (unsigned i = 0; i < deepsprites.size(); ++i) {
       if(deepsprites[i]->getZoom() == 1.0)
       deepsprites[i]->draw();
  }
  for (unsigned i = 0; i < sprites.size(); ++i) {
       sprites[i]->draw();
  }

  io.printMessageAt(title, 10, 460);
  io.printMessageAt("Press R to restart", 690, 10);
  io.printMessageAt("The Ring of Xenes", 352, 10);
  if(godMode) io.printMessageAt("God Mode On", 730, 460);
  if(static_cast<player *>(sprites[0])->isFinished()) io.printMessageAt("You Win! Restarting...", 340, 300);
  checkForCollisions();
  viewport.draw();
  HUD.drawHUD(screen, 0, 0);
  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  if (currentSprite == 0) currentSprite = currentSprite+1;
  else currentSprite = currentSprite-1;
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();
  sprites[0]->update(ticks);
  for (unsigned int i = 1; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
    if(sprites[i]->followX(sprites[0]) && sprites[i]->getType() == 0){
	sprites[i]->velocityX(-sprites[i]->velocityX());
    }
    if(sprites[i]->followY(sprites[0]) && sprites[i]->getType() == 0) sprites[i]->velocityY(-sprites[i]->velocityY());
    }
  for (unsigned int i = 0; i < deepsprites.size(); ++i) {
    deepsprites[i]->update(ticks);
  }
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  world.update();
  mountain.update();
  woods.update();
  viewport.update();
  if(static_cast<player *>(sprites[0])->isFinished()) {
   for(unsigned int i=0; i<sprites.size(); ++i) sprites[i]->explode();
  }
}

void Manager::play() {
  SDLSound sound;
  SDL_Event event;
  bool done = false;
  clock.start();

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
	if (keystate[SDLK_f]) {
          clock.toggleCap();
        }
	if (keystate[SDLK_r]) {
          sprites[0]->reset();
        }
        if (keystate[SDLK_F3]) {
          clock.toggleSloMo();
        }
	if (keystate[SDLK_F1]) {
          HUD.toggleDraw();
        }
	if (keystate[SDLK_SPACE]) {
          sound[3];
        }
	if (keystate[SDLK_n]) {
          sound[2];
        }
	if (keystate[SDLK_g]) {
          godMode = !godMode;
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      }
    }
    draw();
    update();
  }
}
