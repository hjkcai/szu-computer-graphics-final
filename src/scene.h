#ifndef __CG_SCENE_H__
#define __CG_SCENE_H__

#include "camera.h"
#include "model.h"
#include "render.h"
#include "shader.h"

class Scene {
private:
  Camera *camera;
  Model *table;
  Shader *shader;

public:
  Scene ();
  ~Scene ();

  void render (Renderer *renderer);
};

#endif
