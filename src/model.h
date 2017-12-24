#ifndef __CG_MODEL_H__
#define __CG_MODEL_H__

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Model {
private:
  GLuint vao, buffer;
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;

  void loadObj (const char *file);

public:
  Model (const char *file);
};

#endif
