#ifndef HUD__H
#define HUD__H
#include "ioManager.h"
#include "clock.h"
#include "aaline.h"

class hud {
public:
  static hud& getInstance();
  void toggleDraw();
  void drawHUD(SDL_Surface*, int, int);
private:
  int hud_height;
  int hud_width;
  Clock& clock;
  IOManager& io;
  bool toDraw;
  hud();
  hud(const hud&);
  hud& operator=(const hud&);
};
#endif
