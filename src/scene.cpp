#include "scene.h"
#include "texture.h"

Scene::Scene () {
  // 设置场景背景色
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // 初始化相机参数
  camera = new Camera();
  camera->eye = glm::vec3(0, 0, 4);
  camera->update();

  // 初始化 Shader
  shader = new Shader("shaders/vshader.glsl", "shaders/fshader.glsl");

  // 读入模型和纹理，并设置其初始参数
  auto tableTexture = new Texture("textures/table.png");
  table = new Model("models/table.obj", tableTexture, shader);
  table->rotationX = -90;
  table->update();
}

Scene::~Scene () {
  delete table->texture;
  delete table;
  delete shader;
  delete camera;
}

void Scene::render (Renderer *renderer) {
  renderer->drawModel(camera, table);
  table->rotationX += 0.01f;
  table->update();
}
