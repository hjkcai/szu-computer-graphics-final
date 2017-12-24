#include "texture.h"

#include <iostream>
#include <SFML/Graphics.hpp>

Texture::Texture (const char *file) {
  sf::Image i;
  if (!i.loadFromFile(file)) {
    std::cout << "Unable to load texture file: " << file << std::endl;
    return;
  }

  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i.getSize().x, i.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, i.getPixelsPtr());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::~Texture () {
  glDeleteTextures(1, &_id);
}
