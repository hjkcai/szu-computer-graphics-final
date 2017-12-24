#include "app.h"

Application::Application (const std::string &title) {
  ctx = new Context(title);
}

Application::~Application () {
  delete ctx;
}

void Application::mainLoop () {
  while (ctx->window->isOpen()) {
    handleWindowEvents();
  }
}

void Application::handleWindowEvents () {
  sf::Event e;
  while (ctx->window->pollEvent(e)) {
    switch (e.type) {
      case sf::Event::Closed:
        ctx->window->close();
        break;

      default:
        break;
    }
  }
}
