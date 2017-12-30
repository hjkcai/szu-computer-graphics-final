#ifndef __CG_MODEL_H__
#define __CG_MODEL_H__

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"

class Model;

// 模型描述
class ModelDescription {
public:
  const Model *model;
  const glm::mat4 transform;

  ModelDescription (const Model *theModel, const glm::mat4 &theTransform): model(theModel), transform(theTransform) {}
};

class Model {
private:
  GLuint vBuffer, uvBuffer, normalBuffer;
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;

  Texture *texture;
  glm::mat4 _modelMatrix = glm::mat4();

  void init ();
  void loadObj (const char *file);

public:
  // 参数设置
  float scale = 1;
  float x = 0, y = 0, z = 0;
  float rotationX = 0, rotationY = 0, rotationZ = 0;
  glm::mat4 getModelMatrix () const;
  Model* update ();

  Model (const std::string &model, const std::string &texture);
  ~Model ();

  // 绘制数据获取
  std::vector<glm::vec3> getVertices () const { return vertices; }
  std::vector<glm::vec2> getUVs () const { return uvs; }
  std::vector<glm::vec3> getNormals () const { return normals; }

  GLuint getVertexBuffer () const { return vBuffer; }
  GLuint getUVBuffer () const { return uvBuffer; }
  GLuint getNormalBuffer () const { return normalBuffer; }

  Texture* getTexture () const { return texture; }

  ModelDescription render (const glm::mat4 &transform = glm::mat4()) const;
};

#endif
