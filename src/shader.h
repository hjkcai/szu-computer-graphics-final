#ifndef __CG_SHADER_H__
#define __CG_SHADER_H__

#include <map>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"

class Shader {
private:
  GLuint programId = 0;
  std::map<std::string, GLuint> uniforms;

  void addUniform (const char *name);
  void initUniforms ();

public:
  Shader (const char *vshader, const char *fshader);
  ~Shader ();

  GLuint getProgramId () const { return programId; }

  void use ();
  void setMVP (const glm::mat4 &data);
  void setModel (const glm::mat4 &data);
  void setView (const glm::mat4 &data);
  void setLightPosition (const glm::vec3 &data);
  void setTexture (const Texture *texture);
};

#endif
