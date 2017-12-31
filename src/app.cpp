#include "app.h"
#include <GL/glew.h>
#include <iostream>

Application::Application (const std::string &title) {
  createWindow(title);
  initGL();

  renderer = new Renderer(window->getSize().x, window->getSize().y);
}

Application::~Application () {
  delete window;
}

void Application::createWindow (const std::string &title) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 2;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  settings.depthBits = 24;
  settings.stencilBits = 8;

  window = new sf::RenderWindow();
  window->create(sf::VideoMode(800, 600), title.c_str(), sf::Style::Close, settings);
  window->setVerticalSyncEnabled(true);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize glew" << std::endl;
  }
}

void Application::initGL () {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
}

void Application::run (Scene *theScene) {
  scene = theScene;

  while (window->isOpen()) {
    renderer->render(scene);
    window->display();
    handleWindowEvents();

    scene->tick(window);
  }
}

void Application::handleWindowEvents () {
  sf::Event e;
  while (window->pollEvent(e)) {
    /* wait here */
  }
}
