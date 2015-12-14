#include "hud.h"
#include "gamedata.h"

hud& hud::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before HUD");
  }
  static hud hud_inst; 
  return hud_inst;
}

hud::hud():
  hud_height(Gamedata::getInstance().getXmlInt("hud/height")),
  hud_width(Gamedata::getInstance().getXmlInt("hud/width")),
  clock(Clock::getInstance()),
  io(IOManager::getInstance()),
  toDraw(false)
 {}

void hud::toggleDraw() {
  toDraw = !toDraw;
}

void hud::drawHUD(SDL_Surface* screen, int x, int y) {
  if( (SDL_GetTicks() <= 5000) || toDraw ) {
  	const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  	Draw_AALine(screen, x, y+hud_height/2, 
                      x+hud_width,y+hud_height/2, 
                      hud_height, 0xff, 0xff, 0xff, 0xff/2);
  	Draw_AALine(screen, x,y, x+hud_width,y, RED);
	clock.draw();
	io.printMessageAt("User Guide", 10, 70);
	io.printMessageAt("F1    --> Toggle HUD", 10, 90);
	io.printMessageAt("A/D   --> Move Left/Right", 10, 110);
	io.printMessageAt("W/S   --> Speed High/Low", 10, 130);
	io.printMessageAt("G     --> God Mode", 10, 150);
	io.printMessageAt("Space --> Shoot Porjectile", 10, 170);
	io.printMessageAt("V     --> Shoot Laser", 10, 190);
	io.printMessageAt("N     --> Magnetic Sheild", 10, 210);
	io.printMessageAt("Crystals can be deflected with N", 10, 240);
	io.printMessageAt("Lasers can destroy aliens & crystals", 10, 260);
	io.printMessageAt("Press P to pause", 10, 280);	
  }
}
