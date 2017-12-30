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

public:
  Scene ();
  ~Scene ();

  Camera* getCamera () const { return camera; }
  LightingOptions* getLightingOptions () const { return light; }

  virtual std::vector<ModelGroup*> getModelGroups () const = 0;
  void onKeydown (const sf::Event::KeyEvent &e) {};
};

class TableScene : public Scene {
private:
  ModelGroup *table, *anotherTable;
  ModelGroup *ground;

public:
  TableScene ();
  ~TableScene ();

  std::vector<ModelGroup*> getModelGroups () const;
  void onKeydown (const sf::Event::KeyEvent &e);
};

#endif
