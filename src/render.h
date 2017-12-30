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
  SceneShader *shader;
  GLbitfield paramsOfClearing = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

  // 阴影绘制参数
  DepthShader *depthShader;
  GLuint depthBuffer, depthTexture;
  const GLuint DEPTH_TEXTURE_SIZE = 1024;

  glm::mat4 depthViewMatrix, depthProjectionMatrix;
  glm::mat4 depthBiasMatrix = glm::mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
  );

  void initShadow ();

  void renderShadow (const Scene *scene);
  void renderScene (const Scene *scene);
  void drawModel (const Model *model, BasicShader *shader, const glm::mat4 &M, const glm::mat4 &V, const glm::mat4 &P);

public:
  Renderer ();

  void clear () const;
  void setClearParams (const GLbitfield &params);

  void render (const Scene *scene);
};

#endif
