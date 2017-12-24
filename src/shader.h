#ifndef __CG_SHADER_H__
#define __CG_SHADER_H__

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"

class Shader {
private:
  GLuint programId = 0;

  GLuint mvpId = 0;
  GLuint textureSamplerId = 0;

public:
  Shader (const char *vshader, const char *fshader);
  ~Shader ();

  GLuint getProgramId () const { return programId; }
  GLuint getMvpId () const { return mvpId; }
  GLuint getTextureSamplerId () const { return textureSamplerId; }

  void use ();
  void setMvp (const glm::mat4 &data);
  void setTexture (const Texture *texture);
};

#endif
