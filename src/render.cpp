#include "render.h"

#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint FramebufferName = 0;
GLuint depthTexture;
Scene *_scene;

Renderer::Renderer () {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_CULL_FACE);
  depthShader = new Shader("shaders/depth-vshader.glsl", "shaders/depth-fshader.glsl");

  // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

  // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Failed to init frame buffer" << std::endl;
    return;
  }
}

void Renderer::clear () const {
  glClear(paramsOfClearing);
}

void Renderer::setClearParams (const GLbitfield &params) {
  paramsOfClearing = params;
}

void Renderer::renderScene (Scene *scene) {
  _scene = scene;

  // Render to our framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
  glViewport(0,0,1024,1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right

  // We don't use bias in the shader, but instead we draw back faces,
  // which are already separated from the front faces by a small distance
  // (if your geometry is made this way)
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

  clear();
  depthShader->use();

  // Render the scene
  scene->render(this);

  // Render to the screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, 600, 600); // Render on the whole framebuffer, complete from the lower left corner to the upper right

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

  clear();
  scene->shader->use();

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, depthTexture);

	GLuint ShadowMapID = glGetUniformLocation(scene->shader->getProgramId(), "shadowMap");
  glUniform1i(ShadowMapID, 1);

  scene->render(this);
}

void Renderer::drawModel (const Model *model, const Camera *camera, Shader *shader, const glm::mat4 &transform) const {
  // 设置 shader 参数
  shader->setModel(model->getModelMatrix() * transform);
  shader->setView(camera->getViewMatrix());
  shader->setProjection(camera->getProjectionMatrix());
  shader->setTexture(model->texture);

  glm::vec3 lightInvDir = _scene->light.position;
  // Compute the MVP matrix from the light's point of view
  glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
  glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
  // or, for spot light :
  //glm::vec3 lightPos(5, 20, 20);
  //glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
  //glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

  glm::mat4 depthModelMatrix = model->getModelMatrix() * transform;
  glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

  // Send our transformation to the currently bound shader,
  // in the "MVP" uniform
  GLuint depthMatrixID = glGetUniformLocation(depthShader->getProgramId(), "depthMVP");
  glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);

  glm::mat4 biasMatrix(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
  );

  glm::mat4 depthBiasMVP = biasMatrix * depthMVP;
  // Send our transformation to the currently bound shader,
  // in the "MVP" uniform
	GLuint DepthBiasID = glGetUniformLocation(_scene->shader->getProgramId(), "DepthBiasMVP");
  glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);

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
