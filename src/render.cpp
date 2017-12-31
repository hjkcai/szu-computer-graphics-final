#include "render.h"

#include <iostream>
#include <algorithm>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer (const float &w, const float &h) {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  shader = new SceneShader();
  initShadow();

  renderSize = std::max(w, h);
  offsetX = w > h ? 0 : (w - h);
  offsetY = h > w ? 0 : (h - w);
}

void Renderer::initShadow () {
  glEnable(GL_CULL_FACE);
  depthShader = new DepthShader();

  // 生成用于绘制阴影的缓冲区
  glGenFramebuffers(1, &depthBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);

  // 生成用于记录阴影大小的缓冲区
  glGenTextures(1, &depthTexture);
  glBindTexture(GL_TEXTURE_2D, depthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

  // 计算阴影时去除颜色数据
  glDrawBuffer(GL_NONE);

  // 测试缓冲区是否正常可用
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Failed to init frame buffer" << std::endl;
  }
}

void Renderer::clear () const {
  glClear(paramsOfClearing);
}

void Renderer::setClearParams (const GLbitfield &params) {
  paramsOfClearing = params;
}

void Renderer::render (const Scene *scene) {
  renderShadow(scene);
  renderScene(scene);
}

void Renderer::renderShadow (const Scene *scene) {
  // 把相机位置移动到光源位置，渲染整个场景，从而计算出阴影位置
  glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);
  glViewport(0, 0, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  clear();

  depthShader->use();

  auto V = depthViewMatrix = glm::lookAt(scene->getLightingOptions()->position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  auto P = depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);

  for (auto group : scene->getModelGroups()) {
    for (auto model : group->getModels()) {
      auto M = group->getModelMatrix() * model->getModelMatrix();
      drawModel(model, depthShader, M, V, P);
    }
  }
}

void Renderer::renderScene (const Scene *scene) {
  // 然后按正常方式，渲染整个场景
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(offsetX, offsetY, renderSize, renderSize);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  clear();

  shader->use();

  // 将阴影数据传入 shader
  auto depthMatrix = depthBiasMatrix * depthProjectionMatrix * depthViewMatrix;
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, depthTexture);
  shader->setShadowMap(1);

  // 将光照数据传入 shader
  shader->setLightingEnabled(true);
  shader->setLightColor(scene->getLightingOptions()->color);
  shader->setLightPower(scene->getLightingOptions()->power);
  shader->setLightPosition(scene->getLightingOptions()->position);

  // 从相机中获取 view 和 projection 矩阵
  auto V = scene->getCamera()->getViewMatrix();
  auto P = scene->getCamera()->getProjectionMatrix();

  // 绘制层级模型
  for (auto group : scene->getModelGroups()) {
    for (auto model : group->getModels()) {
      if (model->getTexture() != NULL) {
        shader->setTextureEnabled(true);
        shader->setTexture(model->getTexture());
      } else {
        shader->setTextureEnabled(false);

        auto material = model->getMaterial();
        if (material != NULL) {
          shader->setDiffuseColor(glm::vec3(material->diffuse[0], material->diffuse[1], material->diffuse[2]));
          shader->setSpecularColor(glm::vec3(material->specular[0], material->specular[1], material->specular[2]));
        } else {
          shader->setSpecularColor(glm::vec3(0.1, 0.1, 0.1));
        }
      }

      auto M = group->getModelMatrix() * model->getModelMatrix();
      shader->setDepthMatrix(depthMatrix * M);
      drawModel(model, shader, M, V, P);
    }
  }
}

void Renderer::drawModel (const Model *model, BasicShader *shader, const glm::mat4 &M, const glm::mat4 &V, const glm::mat4 &P) {
  // 设置 shader 参数
  shader->setModel(M);
  shader->setView(V);
  shader->setProjection(P);

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
  glDrawArrays(GL_TRIANGLES, 0, model->getVertices()->size());

  // 清理绘制内容
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}
