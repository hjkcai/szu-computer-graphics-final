#ifndef __CG_MODEL_H__
#define __CG_MODEL_H__

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "lib/tiny_obj_loader.h"

// 抽象模型定义
class AbstractModel {
protected:
  float scale = 1;
  float x = 0, y = 0, z = 0;
  float rotationX = 0, rotationY = 0, rotationZ = 0;

  glm::mat4 _modelMatrix = glm::mat4();

  void update ();
  void copyFrom (const AbstractModel *model);

public:
  float getScale () const { return scale; }
  void setScale (const float &value) { scale = value; update(); }

  float getX () const { return x; }
  void setX (const float &value) { x = value; update(); }

  float getY () const { return y; }
  void setY (const float &value) { y = value; update(); }

  float getZ () const { return z; }
  void setZ (const float &value) { z = value; update(); }

  float getRotationX () const { return rotationX; }
  void setRotationX (const float &value) { rotationX = value; update(); }

  float getRotationY () const { return rotationY; }
  void setRotationY (const float &value) { rotationY = value; update(); }

  float getRotationZ () const { return rotationZ; }
  void setRotationZ (const float &value) { rotationZ = value; update(); }

  glm::mat4 getModelMatrix () const { return _modelMatrix; }
};

class Model : public AbstractModel {
private:
  bool cloned = false;

  GLuint vBuffer, uvBuffer, normalBuffer;
  std::vector<glm::vec3> *vertices = NULL;
  std::vector<glm::vec2> *uvs = NULL;
  std::vector<glm::vec3> *normals = NULL;

  tinyobj::material_t *material = NULL;
  Texture *texture = NULL;

  Model ();
  void init ();

public:
  Model (const std::vector<glm::vec3> &theVertices, const std::vector<glm::vec2> &theUVs, const std::vector<glm::vec3> &theNormals, tinyobj::material_t *theMaterial = NULL);
  ~Model ();

  Model* clone () const;

  std::vector<glm::vec3>* getVertices () const { return vertices; }
  std::vector<glm::vec2>* getUVs () const { return uvs; }
  std::vector<glm::vec3>* getNormals () const { return normals; }

  GLuint getVertexBuffer () const { return vBuffer; }
  GLuint getUVBuffer () const { return uvBuffer; }
  GLuint getNormalBuffer () const { return normalBuffer; }

  Texture* getTexture () const { return texture; }
  void setTexture (const std::string &path) {
    if (texture != NULL) delete texture;
    texture = new Texture(path);
  }

  tinyobj::material_t* getMaterial () { return material; }
};

class ModelGroup : public AbstractModel {
private:
  bool cloned = false;
  std::vector<Model*> models;

  void loadObj (const std::string &obj);
  ModelGroup ();

public:
  ModelGroup (const std::string &obj);
  ~ModelGroup ();

  ModelGroup* clone () const;
  std::vector<Model*> getModels () { return models; }
  std::vector<Model*> getModels () const { return models; }
};

#endif
