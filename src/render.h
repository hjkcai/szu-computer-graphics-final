#ifndef __CG_RENDER_H__
#define __CG_RENDER_H__

#include <GL/glew.h>

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
  void drawModel (const Model *model, const Camera *camera, Shader *shader) const;
};

#endif
