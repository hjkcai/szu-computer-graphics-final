#ifndef __CG_MODEL_H__
#define __CG_MODEL_H__

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

class Model {
private:
  GLuint vBuffer, uvBuffer;
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;

  glm::mat4 _modelMatrix = glm::mat4();

  void init ();
  void loadObj (const char *file);

public:
  Shader *shader;
  Texture *texture;

  // 参数设置
  float scale = 1;
  float x = 0, y = 0, z = 0;
  float rotationX = 0, rotationY = 0, rotationZ = 0;

  Model (const char *model, Texture *texture = NULL, Shader *shader = NULL);
  ~Model ();

  std::vector<glm::vec3> getVertices () const { return vertices; }
  std::vector<glm::vec2> getUVs () const { return uvs; }
  std::vector<glm::vec3> getNormals () const { return normals; }

  GLuint getVertexBuffer () const { return vBuffer; }
  GLuint getUVBuffer () const { return uvBuffer; }

  Model* update ();
  glm::mat4 modelMatrix () const;
};

#endif
