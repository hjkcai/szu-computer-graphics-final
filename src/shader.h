#ifndef __CG_SHADER_H__
#define __CG_SHADER_H__

#include <map>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"

// shader 中使用到的 uniform 名称
#define MODEL_MATRIX "model"
#define VIEW_MATRIX "view"
#define PROJ_MATRIX "projection"

#define DEPTH_BIAS_MVP "DepthBiasMVP"
#define SHADOW_MAP "shadowMap"

#define LIGHTING_ENABLED "lightingEnabled"
#define LIGHT_COLOR "lightColor"
#define LIGHT_POWER "lightPower"
#define LIGHT_POSITION "lightPosition"

#define TEXTURE_SAMPLER "textureSampler"

// 基础 shader 定义
class BasicShader {
private:
  void initBasicUniforms ();

protected:
  GLuint programId = 0;
  std::map<std::string, GLuint> uniforms;

  void addUniform (const char *name);

public:
  BasicShader (const char *vshader, const char *fshader);
  ~BasicShader ();

  void use ();
  GLuint getProgramId () const { return programId; }

  void setModel (const glm::mat4 &data);
  void setView (const glm::mat4 &data);
  void setProjection (const glm::mat4 &data);
};

// 用于绘制场景的 shader
class SceneShader : public BasicShader {
public:
  SceneShader ();

  void setDepthMatrix (const glm::mat4 &data);
  void setShadowMap (const GLuint &data);

  void setLightingEnabled (const bool &data);
  void setLightColor (const glm::vec3 &data);
  void setLightPower (const float &data);
  void setLightPosition (const glm::vec3 &data);

  void setTexture (const Texture *texture);
};

// 用户绘制阴影的 shader
class DepthShader : public BasicShader {
public:
  DepthShader ();
};

#endif
