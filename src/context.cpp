#include "context.h"

#include <GL/glew.h>

Context::Context (const std::string &title) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 0;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  settings.depthBits = 24;
  settings.stencilBits = 8;

  window = new sf::RenderWindow();
  window->create({ 800, 600 }, title.c_str(), sf::Style::Close, settings);
  window->setFramerateLimit(60);

  glewInit();
  glewExperimental = GL_TRUE;
  glViewport(0, 0, 800, 600);
}

Context::~Context () {
  delete window;
}
