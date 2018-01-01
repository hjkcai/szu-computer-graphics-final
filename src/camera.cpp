#include "camera.h"

Camera::Camera () {
  update();
}

// 利用 glm 库完成对透视和投影矩阵的计算
void Camera::update () {
  projection = glm::perspective(fov, aspect, zNear, zFar);
  view = glm::lookAt(eye, at, up);
}
