#include "scene.h"
#include "texture.h"
#include <iostream>

Scene::Scene () {
  // 设置场景背景色
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // 读入模型和纹理，并设置其初始参数
  table = new Model("models/table.obj", "textures/table.png");
  table->rotationX = -90;
  table->y = 0.6736;
  table->update();

  ground = new Model("models/ground.obj", "textures/ground.jpg");

  // 初始化相机参数
  camera = new Camera();
  camera->up = glm::vec3(0, 0.001, 0);
  camera->eye = glm::vec3(0, 4, 4);
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
  // 启用 Shader
  shader->use();

  // 设置光照参数
  shader->setLightColor(light.color);
  shader->setLightPower(light.power);
  shader->setLightPosition(light.position);

  // 绘制模型
  renderer->drawModel(table, camera, shader);
  renderer->drawModel(ground, camera, shader);
}

void Scene::onKeydown (const sf::Event::KeyEvent &e) {
  switch (e.code) {
    case sf::Keyboard::W:
      camera->up.y += 0.001;
      break;

    case sf::Keyboard::S:
      camera->up.y -= 0.001;
      break;

    case sf::Keyboard::E:
      camera->up.x += 0.001;
      break;

    case sf::Keyboard::Q:
      camera->up.x -= 0.001;
      break;

    case sf::Keyboard::D:
      camera->up.z += 0.001;
      break;

    case sf::Keyboard::A:
      camera->up.z -= 0.001;
      break;

    default:
      break;
  }

  std::cout << camera->up.x << '\t' << camera->up.y << '\t' << camera->up.z << '\t' << std::endl;
  camera->update();
}
