#ifndef __CG_RENDER_H__
#define __CG_RENDER_H__

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "camera.h"
#include "model.h"
#include "scene.h"
#include "shader.h"

class Scene;
class Renderer {
private:
  GLbitfield paramsOfClearing = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  Shader *depthShader;

public:
  Renderer ();

  void clear () const;
  void setClearParams (const GLbitfield &params);

  void renderScene (Scene *scene);
  void drawModel (const Model *model, const Camera *camera, Shader *shader, const glm::mat4 &transform = glm::mat4(1)) const;
};

#endif
