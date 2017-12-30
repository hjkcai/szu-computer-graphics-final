#ifndef __CG_RENDER_H__
#define __CG_RENDER_H__

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "camera.h"
#include "model.h"
#include "shader.h"

class Renderer {
private:
  GLbitfield paramsOfClearing = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

public:
  Renderer ();

  void setClearParams (const GLbitfield &params);

  void clear () const;
  void drawModel (const Model *model, const Camera *camera, Shader *shader, const glm::mat4 &transform = glm::mat4(1)) const;
};

#endif
