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

void Renderer::drawModel (const Model *model, const Camera *camera, Shader *shader) const {
  // 设置 shader 参数
  shader->use();
  shader->setModel(model->getModelMatrix());
  shader->setView(camera->getViewMatrix());
  shader->setProjection(camera->getProjectionMatrix());
  shader->setLightPosition(glm::vec3(4, 4, 4));
  shader->setTexture(model->texture);

  // 绑定顶点数据
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, model->getVertexBuffer());
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // 绑定纹理映射数据 (UVs)
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, model->getUVBuffer());
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Normals
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, model->getNormalBuffer());
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // 绘制模型
  glDrawArrays(GL_TRIANGLES, 0, model->getVertices().size());

  // 清理绘制内容
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}
