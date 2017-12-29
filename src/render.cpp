#include "render.h"

Renderer::Renderer () {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

void Renderer::setClearParams (const GLbitfield &params) {
  paramsOfClearing = params;
}

void Renderer::clear () const {
  glClear(paramsOfClearing);
}

void Renderer::drawModel (const Camera *camera, const Model *model) const {
  // 计算模-视矩阵
  glm::mat4 mvp = camera->getViewProjectionMatrix() * model->modelMatrix();

  model->shader->use();
  model->shader->setMvp(mvp);
  model->shader->setTexture(model->texture);

  // Vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, model->getVertexBuffer());
  glVertexAttribPointer(
    0,                  // attribute
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, model->getUVBuffer());
  glVertexAttribPointer(
    1,                                // attribute
    2,                                // size
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
  );

  // Normals
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, model->getNormalBuffer());
  glVertexAttribPointer(
    2,                                // attribute
    3,                                // size
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
  );

  // 绘制模型
  glDrawArrays(GL_TRIANGLES, 0, model->getVertices().size());

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
