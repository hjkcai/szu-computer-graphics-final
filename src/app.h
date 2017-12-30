#ifndef __CG_APP_H__
#define __CG_APP_H__

#include <string>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "render.h"
#include "scene.h"

class Application {
private:
  TableScene *scene;
  Renderer *renderer;
  GLuint vao;

  void createWindow (const std::string &title);
  void initGL ();

public:
  sf::RenderWindow *window;

  Application (const std::string &title);
  ~Application ();

  void mainLoop ();
  void handleWindowEvents ();
};

#endif
