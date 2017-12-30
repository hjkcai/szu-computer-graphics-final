#ifndef __CG_APP_H__
#define __CG_APP_H__

#include <string>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "render.h"
#include "scene.h"

class Application {
private:
  Scene *scene = NULL;
  Renderer *renderer = NULL;
  GLuint vao;

  void createWindow (const std::string &title);
  void initGL ();

public:
  sf::RenderWindow *window;

  Application (const std::string &title);
  ~Application ();

  void run (Scene *theScene);
  void handleWindowEvents ();
};

#endif
