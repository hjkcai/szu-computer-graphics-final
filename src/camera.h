#ifndef __CG_CAMERA_H__
#define __CG_CAMERA_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
  glm::mat4 projection, view;
  glm::mat4 vp;

public:
  // 透视投影参数
  float fov = 45;
  float aspect = 1;
  float zNear = 0.1f;
  float zFar = 100;

  // 相机参数
  glm::vec3 eye = glm::vec3(0, 0, -1);
  glm::vec3 at = glm::vec3(0, 0, 0);
  glm::vec3 up = glm::vec3(0, 1, 0);

  Camera ();
  void update ();

  glm::mat4 getViewMatrix () const { return view; }
  glm::mat4 getProjectionMatrix () const { return projection; }
  glm::mat4 getViewProjectionMatrix () const { return vp; }
};

#endif
