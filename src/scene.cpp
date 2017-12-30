#include "scene.h"
#include "texture.h"

#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene () {
  // 初始化相机参数
  camera = new Camera();
  camera->up = glm::vec3(0, 1, 0);
  camera->eye = glm::vec3(4, 4, 4);
  camera->update();

  // 初始化光照参数
  light = new LightingOptions();
  light->power = 100;
  light->position = { -2.5, 10, 2.5 };
}

Scene::~Scene () {
  delete camera;
  delete light;

  for (auto group : modelGroups) {
    delete group;
  }
}
