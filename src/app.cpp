#include "app.h"
#include <GL/glew.h>

Application::Application (const std::string &title) {
  createWindow(title);
}

Application::~Application () {
  delete window;
}

void Application::createWindow (const std::string &title) {
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

void Application::mainLoop () {
  while (window->isOpen()) {
    handleWindowEvents();
  }
}

void Application::handleWindowEvents () {
  sf::Event e;
  while (window->pollEvent(e)) {
    switch (e.type) {
      case sf::Event::Closed:
        window->close();
        break;

      default:
        break;
    }
  }
}
