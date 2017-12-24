#ifndef __CG_APP_H__
#define __CG_APP_H__

#include <string>
#include <SFML/Graphics.hpp>

#include "render.h"
#include "scene.h"

class Application {
private:
  Scene *scene;
  Renderer *renderer;
  void createWindow (const std::string &title);

public:
  sf::RenderWindow *window;

  Application (const std::string &title);
  ~Application ();

  void mainLoop ();
  void handleWindowEvents ();
};

#endif
