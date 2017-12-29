#include "app.h"
#include <GL/glew.h>
#include <iostream>

Application::Application (const std::string &title) {
  createWindow(title);
  renderer = new Renderer();
  scene = new Scene();
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
  window->create({ 600, 600 }, title.c_str(), sf::Style::Close, settings);
  window->setFramerateLimit(60);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize glew" << std::endl;
  }

  glViewport(0, 0, 600, 600);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
}

void Application::mainLoop () {
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  while (window->isOpen()) {
    renderer->clear();
    scene->render(renderer);
    window->display();
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
