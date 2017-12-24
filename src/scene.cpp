#include "scene.h"
#include "texture.h"

Scene::Scene () {
  camera = new Camera();
  shader = new Shader("shaders/vshader.glsl", "shaders/fshader.glsl");

  auto teacupTexture = new Texture("textures/teacup.tiff");
  teacup = new Model("models/teacup.obj", teacupTexture, shader);
}

Scene::~Scene () {
  delete teacup->texture;
  delete teacup;
  delete shader;
  delete camera;
}

void render (Renderer *renderer) {
  renderer->drawModel(teacup);
}
