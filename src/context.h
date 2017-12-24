#ifndef __CG_CONTEXT_H__
#define __CG_CONTEXT_H__

#include <string>
#include <SFML/Graphics.hpp>

class Context {
public:
  sf::RenderWindow *window;

  Context (const std::string &title);
  ~Context ();
};

#endif
