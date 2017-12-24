#include "render.h"

Renderer::Renderer () {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

void Renderer::setClearParams (const GLbitfield &params) {
  paramsOfClearing = params;
}

void Renderer::clear () const {
  glClear(paramsOfClearing);
}
