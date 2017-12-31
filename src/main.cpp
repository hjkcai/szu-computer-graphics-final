#include "app.h"
#include <iostream>
#include <algorithm>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Application *app;

// 汽车模型
class Car : public ModelGroup {
private:
  // 左前轮和右前轮与原点的位置差
  float dxl = 1.71324, dyl = -1.10560, dzl = -1.03662;
  float dxr = 1.73005, dyr = -1.10560, dzr = 1.11338;

  // 左前轮和右前轮的旋转角度
  float r = 0;

protected:
  void update () {
    ModelGroup::update();

    // 左前轮
    for (int i = 9; i < 12; i++) {
      auto cylinderPart = models[i];
      auto transform =
        glm::translate(glm::vec3(-dxl, -dyl, -dzl)) *
        glm::rotate(glm::radians(-r), glm::vec3(0, 1, 0)) *
        glm::translate(glm::vec3(dxl, dyl, dzl));

      cylinderPart->setTransform(transform);
    }

    // 右前轮
    for (int i = 0; i < 3; i++) {
      auto cylinderPart = models[i];
      auto transform =
        glm::translate(glm::vec3(-dxr, -dyr, -dzr)) *
        glm::rotate(glm::radians(-r), glm::vec3(0, 1, 0)) *
        glm::translate(glm::vec3(dxr, dyr, dzr));

      cylinderPart->setTransform(transform);
    }
  }

public:
  Car () : ModelGroup("models/car.obj") {
    for (auto model : models) {
      model->setY(0.54);
      model->setRotationY(90);
    }
  }

  float getWheelRotation () const { return r; }
  void setWheelRotation (const float &value) { r = value; update(); }
};

class MyScene : public Scene {
private:
  std::vector<ModelGroup*> trees;
  std::vector<ModelGroup*> grasses;
  ModelGroup *ground;
  Car* car;
  glm::vec3 carPos = glm::vec3(0, 0, -18);

  void setModelPos (ModelGroup *group, const float &x, const float &y, const float &z) {
    for (auto model : group->getModels()) {
      model->setX(x);
      model->setY(y);
      model->setZ(z);
    }
  }

public:
  MyScene () {
    // 设置背景色
    glClearColor(0.793f, 1.0f, 0.942f, 1.0f);

    // 设置光照和相机参数
    light->position = { 6, 20, -25 };
    light->power = 1000;

    camera->at = { 0, -8, 0 };
    camera->eye = { 0, 6, -26 };
    camera->update();

    // 设置模型和场景
    initModels();
    initScene();
  }

  // 读入模型和纹理，并设置其初始参数
  void initModels () {
    auto tree1 = new ModelGroup("models/tree-1.obj");
    setModelPos(tree1, 0, 1.895607, 0);
    trees.push_back(tree1);

    auto tree2 = new ModelGroup("models/tree-2.obj");
    setModelPos(tree2, 0, 1, 0);
    trees.push_back(tree2);

    auto grass1 = new ModelGroup("models/wild-grasses-1.obj");
    setModelPos(grass1, -0.62, -0.035, -0.514);
    grasses.push_back(grass1);

    auto grass2 = new ModelGroup("models/wild-grasses-2.obj");
    setModelPos(grass2, 0.51, 0.107, 0.369);
    grasses.push_back(grass2);

    // auto grass3 = new ModelGroup("models/wild-grasses-3.obj");
    // setModelPos(grass3, -0.553, 0.081, 0.47);
    // grasses.push_back(grass3);

    auto grass4 = new ModelGroup("models/wild-grasses-4.obj");
    setModelPos(grass4, 0.515, -0.045, -0.503);
    grasses.push_back(grass4);

    car = new Car();

    ground = new ModelGroup("models/ground.obj");
    ground->getModels()[0]->setTexture("models/ground.jpg");
  }

  glm::vec3 generatePoint () {
    auto x = (rand() % 3200 - 1600) / 100.0f;
    x = x + (x < 0 ? -4 : 4);

    auto z = (rand() % 4000 - 2000) / 100.0f;
    return glm::vec3(x, 0, z);
  }

  // 生成场景
  void initScene () {
    // 生成草地和树林
    srand(time(NULL));
    for (int i = 0; i < 50; i++) {
      auto tree = trees[i % trees.size()]->clone();
      tree->setPosition(generatePoint());
      modelGroups.push_back(tree);
    }

    for (int i = 0; i < 1000; i++) {
      auto grass = grasses[i % grasses.size()]->clone();
      grass->setPosition(generatePoint());
      grass->setScale(1.5);
      modelGroups.push_back(grass);
    }

    car->setScale(0.5);
    car->setZ(-18);
    modelGroups.push_back(car);

    // 加入地面
    modelGroups.push_back(ground);
  }

  void tick () {
    if (!app->window->hasFocus()) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      auto direction = car->getRotationY();
      auto r = car->getWheelRotation();

      float directionDeltaFactor;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        directionDeltaFactor = 1;
      } else {
        directionDeltaFactor = -1;
      }

      auto sinDir = glm::sin(glm::radians(direction));
      auto cosDir = glm::cos(glm::radians(direction));

      auto dx = 0.1 * directionDeltaFactor * sinDir;
      auto dz = 0.1 * directionDeltaFactor * cosDir;

      carPos.x += dx;
      carPos.z += dz;

      camera->eye = glm::vec3(-8 * sinDir, 6, -8 * cosDir) + carPos;
      camera->at = glm::vec3(26 * sinDir, -14, 26 * cosDir) + camera->eye;

      car->setRotationY(car->getRotationY() - r * 0.1 * directionDeltaFactor);
      car->setPosition(carPos);
      camera->update();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      car->setWheelRotation(std::max(-45.0f, car->getWheelRotation() - 1));
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      car->setWheelRotation(std::min(45.0f, car->getWheelRotation() + 1));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      exit(0);
    }
  }
};

int main () {
  app = new Application("2015150317_final");

  MyScene *scene = new MyScene();
  app->run(scene);
}
