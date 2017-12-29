#include "scene.h"
#include "texture.h"

Scene::Scene () {
  // 设置场景背景色
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // 读入模型和纹理，并设置其初始参数
  table = new Model("models/table.obj", "textures/table.png");
  table->rotationX = -90;
  table->update();

  // 初始化相机参数
  camera = new Camera();
  camera->eye = glm::vec3(0, 0, 4);
  camera->update();

  // 初始化 Shader
  shader = new Shader("shaders/vshader.glsl", "shaders/fshader.glsl");
}

Scene::~Scene () {
  delete table->texture;
  delete table;
  delete shader;
  delete camera;
}

void Scene::render (Renderer *renderer) {
  // 绘制模型
  shader->use();
  renderer->drawModel(table, camera, shader);

  // 增加一个简单的旋转动画
  table->rotationX += 0.01f;
  table->update();
}
