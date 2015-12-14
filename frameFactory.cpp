#include "frameFactory.h"
#include "extractSurface.h"
#include "ioManager.h"
#include "vector2f.h"
#include "SDL/SDL_rotozoom.h"

FrameFactory::FrameFactory() : 
  gdata( Gamedata::getInstance() ), 
  surfaces(),
  multiSurfaces(),
  frames(),
  multiFrames()
{}

FrameFactory::~FrameFactory() {
	for(std::map<std::string,Frame*>::iterator i = frames.begin(); i != frames.end(); i++) { if((*i).second) delete (*i).second; }
	for(std::map<std::string,std::vector<Frame*> >::iterator j = multiFrames.begin(); j != multiFrames.end(); j++) {
		for(std::vector<Frame*>::iterator k = j->second.begin(); k != j->second.end(); k++) { if(*k) delete (*k); }
	}
	for(std::map<std::string,SDL_Surface*>::iterator l = surfaces.begin(); l != surfaces.end(); l++) { if((*l).second) SDL_FreeSurface((*l).second); }
	for(std::map<std::string,std::vector<SDL_Surface*> >::iterator m = multiSurfaces.begin(); m != multiSurfaces.end(); m++) {
		for(std::vector<SDL_Surface*>::iterator n = m->second.begin(); n != m->second.end(); n++) { if(*n) SDL_FreeSurface(*n); }
	}
}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
  std::map<std::string,Frame*>::const_iterator posn = frames.find(name);
  if(posn!=frames.end()) { return posn->second; }
  SDL_Surface* surface = IOManager::getInstance().loadAndSet(gdata.getXmlStr(name+"/file"),gdata.getXmlBool(name+"/transparency"));
  surfaces[name] = ExtractSurface::getInstance().get(surface, gdata.getXmlInt(name+"/width"), gdata.getXmlInt(name+"/height"), gdata.getXmlInt(name+"/srcX"), gdata.getXmlInt(name+"/srcY"));
  frames[name] = new Frame(name,surfaces[name]);
  SDL_FreeSurface(surface);
  return frames[name];
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = multiFrames.find(name); 
  if ( pos != multiFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"/srcX");
  Uint16 srcY = gdata.getXmlInt(name+"/srcY");
  Uint16 width = gdata.getXmlInt(name+"/width");
  Uint16 height = gdata.getXmlInt(name+"/height");

  SDL_Surface* surf;
  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * width + srcX;
   surf = ExtractSurface::getInstance().
               get(surface, width, height, frameX, srcY); 
    surfaces.push_back( surf );
    frames.push_back( new Frame(name, surf) );
  }
  SDL_FreeSurface(surface);
  multiSurfaces[name] = surfaces;
  multiFrames[name] = frames;
  return frames;
}
