#include "scene.h"
#include "texture.h"

#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
  camera->up = glm::vec3(0, 1, 0);
  camera->eye = glm::vec3(4, 4, 4);
  camera->update();

  // 初始化 Shader
  shader = new Shader("shaders/vshader.glsl", "shaders/fshader.glsl");

  light.power = 100;
  light.position = { -2.5, 10, 2.5 };
}

Scene::~Scene () {
  delete table->texture;
  delete table;
  delete shader;
  delete camera;
}

void Scene::render (Renderer *renderer) {
  // 设置光照参数
  shader->setLightColor(light.color);
  shader->setLightPower(light.power);
  shader->setLightPosition(light.position);

  // 绘制模型
  shader->setLightingEnabled(true);
  renderer->drawModel(table, camera, shader);
  renderer->drawModel(table, camera, shader, glm::translate(glm::vec3(2, 0, 0)));

  // 绘制地面
  // shader->setLightingEnabled(false);
  renderer->drawModel(ground, camera, shader);
}

void Scene::onKeydown (const sf::Event::KeyEvent &e) {
  switch (e.code) {
    // case sf::Keyboard::X:
    //   camera->eye.x += e.shift ? 0.1 : -0.1;
    //   break;

    // case sf::Keyboard::Y:
    //   camera->eye.y += e.shift ? 0.1 : -0.1;
    //   break;

    // case sf::Keyboard::Z:
    //   camera->eye.z += e.shift ? 0.1 : -0.1;
    //   break;

    case sf::Keyboard::X:
      light.position.x += e.shift ? 0.1 : -0.1;
      break;

    case sf::Keyboard::Y:
      light.position.y += e.shift ? 0.1 : -0.1;
      break;

    case sf::Keyboard::Z:
      light.position.z += e.shift ? 0.1 : -0.1;
      break;

    default:
      break;
  }

  std::cout << light.position.x << '\t' << light.position.y << '\t' << light.position.z << std::endl;
  // camera->update();
}
