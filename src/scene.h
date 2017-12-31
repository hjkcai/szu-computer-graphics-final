#ifndef __CG_SCENE_H__
#define __CG_SCENE_H__

#include <vector>
#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#include "camera.h"
#include "model.h"
#include "render.h"
#include "shader.h"

class Renderer;

// 光照参数
struct LightingOptions {
  glm::vec3 position = { 4, 4, 4 };         // 光照位置
  glm::vec3 color = { 1, 1, 1 };            // 光照颜色
  float power = 50;                         // 光照强度
};

// 基础场景类
class Scene {
protected:
  Camera *camera;
  LightingOptions *light;
  std::vector<ModelGroup*> modelGroups;

public:
  Scene ();
  ~Scene ();

  Camera* getCamera () const { return camera; }
  LightingOptions* getLightingOptions () const { return light; }
  std::vector<ModelGroup*> getModelGroups () const { return modelGroups; }

  virtual void tick (const sf::RenderWindow *window) = 0;
};

#endif
