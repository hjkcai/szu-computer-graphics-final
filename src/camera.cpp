#include "camera.h"

void Camera::update () {
  // projection = glm::perspective(fov, aspect, zNear, zFar);
  projection = glm::ortho(-2, 2, -2, 2, -2, 2);
  view = glm::lookAt(eye, at, up);

  // 令模型矩阵永远为单位矩阵
  vp = projection * view;
}

glm::mat4 Camera::mvp (const glm::mat4 &model) const {
  return vp * model;
}