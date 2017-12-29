#include "camera.h"

Camera::Camera () {
  update();
}

void Camera::update () {
  projection = glm::perspective(fov, aspect, zNear, zFar);
  view = glm::lookAt(eye, at, up);

  // 这里先不设置模型矩阵
  vp = projection * view;
}
