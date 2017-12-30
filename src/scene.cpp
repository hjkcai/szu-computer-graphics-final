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
}

TableScene::TableScene () {
  // 读入模型和纹理，并设置其初始参数
  table = new Model("models/table.obj", "textures/table.png");
  table->rotationX = -90;
  table->y = 0.6736;
  table->update();

  ground = new Model("models/ground.obj", "textures/ground.jpg");
}

TableScene::~TableScene () {
  delete table;
  delete ground;
}

std::vector<ModelDescription> TableScene::render () const {
  std::vector<ModelDescription> result;
  result.push_back(table->render());
  result.push_back(table->render(glm::translate(glm::vec3(2, 0, 0))));
  result.push_back(ground->render());
  return result;
}

void TableScene::onKeydown (const sf::Event::KeyEvent &e) {
  switch (e.code) {
    case sf::Keyboard::X:
      light->position.x += e.shift ? 0.1 : -0.1;
      break;

    case sf::Keyboard::Y:
      light->position.y += e.shift ? 0.1 : -0.1;
      break;

    case sf::Keyboard::Z:
      light->position.z += e.shift ? 0.1 : -0.1;
      break;

    default:
      break;
  }

  // std::cout << light->position.x << '\t' << light->position.y << '\t' << light->position.z << std::endl;
}
