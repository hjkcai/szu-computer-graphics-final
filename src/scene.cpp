#include "scene.h"
#include "texture.h"

Scene::Scene () {
  camera = new Camera();
  camera->eye = glm::vec3(0, 0, 4);
  camera->update();

  shader = new Shader("shaders/vshader.glsl", "shaders/fshader.glsl");

  auto tableTexture = new Texture("textures/table.png");
  table = new Model("models/table.obj", tableTexture, shader);
  table->rotationX = -90;
  table->y = -0.5;
  table->z = -1;
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
