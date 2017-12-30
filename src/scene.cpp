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
  table = new ModelGroup("models/table.obj");
  table->getModels()[0]->setTexture("textures/table.png");
  table->setRotationX(-90);
  table->setY(0.6736);
  table->setZ(-2);

  anotherTable = table->clone();
  anotherTable->setX(2);
  anotherTable->setZ(0);

  ground = new ModelGroup("models/ground.obj");
  ground->getModels()[0]->setTexture("textures/ground.jpg");

  tree = new ModelGroup("models/tree.obj");
  tree->setScale(0.5);
  tree->setY(0.947804);
}

TableScene::~TableScene () {
  delete table;
  delete anotherTable;
  delete ground;
  delete tree;
}

std::vector<ModelGroup*> TableScene::getModelGroups () const {
  std::vector<ModelGroup*> result;
  result.push_back(ground);
  result.push_back(table);
  result.push_back(anotherTable);
  result.push_back(tree);
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
}
