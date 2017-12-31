#include "app.h"
#include <iostream>
#include <algorithm>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 汽车模型
class Car : public ModelGroup {
private:
  // 左前轮和右前轮与原点的位置差
  float dxl = 1.71324, dyl = -1.10560, dzl = -1.03662;
  float dxr = 1.73005, dyr = -1.10560, dzr = 1.11338;

  // 左前轮和右前轮的旋转角度
  float r = 0;

protected:
  const float friction = 0.3;             // 摩擦大小
  const float wheelBack = 3;              // 车轮回正速度
  const float maxVelocity = 20;           // 最大速度
  const float maxNegativeVelocity = -8;   // 最大倒车速度
  const float maxWheelAngle = 45;         // 车轮最大旋转角度

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

  float shift (const float &value, const float &amount) {
    if (glm::abs(value) < 0.1) return 0;
    else if (value > 0) return value - amount;
    else if (value < 0) return value + amount;
    else return value;
  }

public:
  float acceleration = 0;     // 加速度
  float velocity = 0;         // 速度

  Car () : ModelGroup("models/car.obj") {
    for (auto model : models) {
      model->setY(0.54);
      model->setRotationY(90);
    }
  }

  float getWheelRotation () const { return r; }
  void setWheelRotation (const float &value) { r = value; update(); }

  void move () {
    // 计算速度
    auto &direction = rotationY;
    auto sinDir = glm::sin(glm::radians(direction));
    auto cosDir = glm::cos(glm::radians(direction));

    velocity = shift(velocity + acceleration, friction);
    velocity = glm::clamp(velocity + acceleration, maxNegativeVelocity, maxVelocity);

    x += sinDir * velocity / 100;
    z += cosDir * velocity / 100;

    // 计算车身和车轮方向
    direction = direction - 0.05 * (velocity / maxVelocity) * r;
    r = glm::clamp(shift(r, wheelBack), -maxWheelAngle, maxWheelAngle);

    update();
  }
};

class MyScene : public Scene {
private:
  // 模型定义
  std::vector<ModelGroup*> trees;
  std::vector<ModelGroup*> grasses;
  ModelGroup *ground;
  Car* car;

  // 相机到汽车的位置差
  glm::vec3 eyeToCar = glm::vec3(-8, 6, -8);

  // 相机到视点的位置差
  glm::vec3 atToEye = glm::vec3(26, -14, 26);

  // 额外的视角旋转量
  float rotationY = 0;

public:
  MyScene () {
    // 设置背景色
    glClearColor(0.793f, 1.0f, 0.942f, 1.0f);

    initCameraAndLight();
    initModels();
    initScene();
  }

  // 设置模型组中所有模型的子模型的位置，这样便于接下来统一移动模型组
  void setModelPos (ModelGroup *group, const float &x, const float &y, const float &z) {
    for (auto model : group->getModels()) {
      model->setX(x);
      model->setY(y);
      model->setZ(z);
    }
  }

  // 随机生成一个不在路中间的点
  glm::vec3 generatePoint () {
    auto x = (rand() % 3200 - 1600) / 100.0f;
    x = x + (x < 0 ? -4 : 4);

    auto z = (rand() % 4000 - 2000) / 100.0f;
    return glm::vec3(x, 0, z);
  }

  // 初始化光照和相机
  void initCameraAndLight () {
    light->position = { 6, 20, -25 };
    light->power = 1000;

    camera->at = { 0, -8, 0 };
    camera->eye = { 0, 6, -26 };
    camera->update();
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

    auto grass3 = new ModelGroup("models/wild-grasses-3.obj");
    setModelPos(grass3, 0.515, -0.045, -0.503);
    grasses.push_back(grass3);

    car = new Car();

    ground = new ModelGroup("models/ground.obj");
    ground->getModels()[0]->setTexture("models/ground.jpg");
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

  void tick (const sf::RenderWindow *window) {
    if (window->hasFocus()) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        car->acceleration = 0.5;
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        car->acceleration = -0.5;
      } else {
        car->acceleration = 0;
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        car->setWheelRotation(car->getWheelRotation() - 8);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        car->setWheelRotation(car->getWheelRotation() + 8);
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        rotationY += 1;
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        rotationY -= 1;
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        exit(0);
      }
    }

    car->move();

    // 调整视角，使视角固定在车的后方
    auto direction = car->getRotationY() + rotationY;
    auto sinDir = glm::sin(glm::radians(direction));
    auto cosDir = glm::cos(glm::radians(direction));
    auto dirVector = glm::vec3(sinDir, 1, cosDir);

    camera->eye = eyeToCar * dirVector + car->getPosition();
    camera->at = atToEye * dirVector + camera->eye;
    camera->update();
  }
};

int main () {
  Application *app = new Application("2015150317_final");
  MyScene *scene = new MyScene();

  app->run(scene);
}
