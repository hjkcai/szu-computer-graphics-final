#include "model.h"
#include <string>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

void AbstractModel::update () {
  _modelMatrix =
    // 平移
    glm::translate(glm::mat4(), glm::vec3(x, y, z)) *

    // 旋转
    glm::rotate(glm::radians(rotationX), glm::vec3(1, 0, 0)) *
    glm::rotate(glm::radians(rotationY), glm::vec3(0, 1, 0)) *
    glm::rotate(glm::radians(rotationZ), glm::vec3(0, 0, 1)) *

    // 缩放
    glm::scale(glm::vec3(scale));
}

void AbstractModel::copyFrom (const AbstractModel *model) {
  scale = model->scale;
  x = model->x;
  y = model->y;
  z = model->z;
  rotationX = model->rotationX;
  rotationY = model->rotationY;
  rotationZ = model->rotationZ;
  update();
}

Model::Model (): cloned(true) {}

Model::Model (
  const std::vector<glm::vec3> &theVertices,
  const std::vector<glm::vec2> &theUVs,
  const std::vector<glm::vec3> &theNormals,
  tinyobj::material_t *theMaterial
) {
  vertices = new std::vector<glm::vec3>(theVertices);
  uvs = new std::vector<glm::vec2>(theUVs);
  normals = new std::vector<glm::vec3>(theNormals);
  material = theMaterial;

  init();
  update();

  if (material != NULL && !material->diffuse_texname.empty()) {
    setTexture(material->diffuse_texname);
  }
}

Model::~Model () {
  if (!cloned) {
    delete vertices; vertices = NULL;
    delete uvs; uvs = NULL;
    delete normals; normals = NULL;

    if (texture != NULL) delete texture;
    if (material != NULL) delete material;

    glDeleteBuffers(1, &vBuffer); vBuffer = 0;
    glDeleteBuffers(1, &uvBuffer); uvBuffer = 0;
    glDeleteBuffers(1, &normalBuffer); normalBuffer = 0;
  }
}

void Model::init () {
  glGenBuffers(1, &vBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(glm::vec3), vertices->data(), GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs->size() * sizeof(glm::vec2), uvs->data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(glm::vec3), normals->data(), GL_STATIC_DRAW);
}

Model* Model::clone () const {
  Model *clonedModel = new Model();
  clonedModel->copyFrom(this);
  clonedModel->vBuffer = vBuffer;
  clonedModel->uvBuffer = uvBuffer;
  clonedModel->normalBuffer = normalBuffer;
  clonedModel->vertices = vertices;
  clonedModel->uvs = uvs;
  clonedModel->normals = normals;
  clonedModel->material = material;
  clonedModel->texture = texture;
  return clonedModel;
}

ModelGroup::ModelGroup (): cloned(true) {}

ModelGroup::ModelGroup (const std::string &obj) {
  loadObj(obj);
}

ModelGroup::~ModelGroup () {
  for (auto model : models) {
    delete model;
  }

  models.clear();
}

void ModelGroup::loadObj (const std::string &obj) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, obj.c_str(), "models/");
  if (!err.empty()) std::cerr << err << std::endl;
  if (!ret) return;

  for (size_t s = 0; s < shapes.size(); s++) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    size_t offset = 0;
    int lastMaterial = shapes[0].mesh.material_ids[0];

    for (size_t f = 0, l = shapes[s].mesh.num_face_vertices.size(); f <= l; f++) {
      if (shapes[s].mesh.material_ids[f] != lastMaterial || f == l) {
        tinyobj::material_t *material = NULL;
        if (lastMaterial != -1) {
          material = new tinyobj::material_t(materials[lastMaterial]);
        }

        Model *model = new Model(vertices, uvs, normals, material);
        models.push_back(model);

        if (f == l) break;
        else {
          vertices.clear();
          uvs.clear();
          normals.clear();
        }
      }

      int fv = shapes[s].mesh.num_face_vertices[f];
      for (size_t v = 0; v < fv; v++) {
        tinyobj::index_t idx = shapes[s].mesh.indices[offset + v];
        tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
        tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
        tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
        vertices.push_back(glm::vec3(vx, vy, vz));

        if (idx.normal_index != -1) {
          tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
          tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
          tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
          normals.push_back(glm::vec3(nx, ny, nz));
        } else {
          normals.push_back(glm::vec3(0, 0, 0));
        }

        if (idx.texcoord_index != -1) {
          tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
          tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
          uvs.push_back(glm::vec2(tx, -ty));
        } else {
          uvs.push_back(glm::vec2(0, 0));
        }
      }

      offset += fv;
      lastMaterial = shapes[s].mesh.material_ids[f];
    }
  }
}

ModelGroup* ModelGroup::clone () const {
  ModelGroup *clonedGroup = new ModelGroup();
  clonedGroup->copyFrom(this);

  for (auto model : models) {
    clonedGroup->models.push_back(model->clone());
  }

  return clonedGroup;
}
