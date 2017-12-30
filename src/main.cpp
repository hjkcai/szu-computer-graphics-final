#include "app.h"

class MyScene : public Scene {
private:
  ModelGroup *table, *anotherTable;
  ModelGroup *ground;
  ModelGroup *tree;

public:
  MyScene () {
    // 读入模型和纹理，并设置其初始参数
    table = new ModelGroup("models/table.obj");
    table->getModels()[0]->setTexture("textures/table.png");
    table->setRotationX(-90);
    table->setY(0.6736);
    table->setZ(-2);
    modelGroups.push_back(table);

    anotherTable = table->clone();
    anotherTable->setX(2);
    anotherTable->setZ(0);
    modelGroups.push_back(anotherTable);

    ground = new ModelGroup("models/ground.obj");
    ground->getModels()[0]->setTexture("textures/ground.jpg");
    modelGroups.push_back(ground);
  }

  void onKeydown (const sf::Event::KeyEvent &e) {
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
};

int main () {
  Application *app = new Application("2015150317_final");
  MyScene *scene = new MyScene();

  app->run(scene);
}
