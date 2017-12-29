#ifndef __CG_SCENE_H__
#define __CG_SCENE_H__

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#include "camera.h"
#include "model.h"
#include "render.h"
#include "shader.h"

class Scene {
private:
  Camera *camera;
  Shader *shader;

  Model *table;
  Model *ground;

  struct {
    glm::vec3 position = { 4, 4, 4 };         // 光照位置
    glm::vec3 color = { 1, 1, 1 };            // 光照颜色
    float power = 50;                         // 光照强度
  } light;

public:
  Scene ();
  ~Scene ();

  void render (Renderer *renderer);
  void onKeydown (const sf::Event::KeyEvent &e);
};

#endif
