#include "render.h"

#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer () {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  shader = new SceneShader();
  initShadow();
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
  for (auto data : scene->render()) {
    auto M = data.model->getModelMatrix() * data.transform;
    auto V = depthViewMatrix = glm::lookAt(scene->getLightingOptions()->position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    auto P = depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
    drawModel(data.model, depthShader, M, V, P);
  }
}

void Renderer::renderScene (const Scene *scene) {
  // 然后按正常方式，渲染整个场景
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, 600, 600);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  clear();

  shader->use();

  // 将阴影数据传入 shader
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, depthTexture);
  shader->setShadowMap(1);

  // 将光照数据传入 shader
  shader->setLightingEnabled(true);
  shader->setLightColor(scene->getLightingOptions()->color);
  shader->setLightPower(scene->getLightingOptions()->power);
  shader->setLightPosition(scene->getLightingOptions()->position);

  for (auto data : scene->render()) {
    auto M = data.model->getModelMatrix() * data.transform;
    auto V = scene->getCamera()->getViewMatrix();
    auto P = scene->getCamera()->getProjectionMatrix();

    shader->setTexture(data.model->getTexture());
    shader->setDepthMatrix(depthBiasMatrix * depthProjectionMatrix * depthViewMatrix * M);

    drawModel(data.model, shader, M, V, P);
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
  glDrawArrays(GL_TRIANGLES, 0, model->getVertices().size());

  // 清理绘制内容
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}
