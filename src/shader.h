#ifndef __CG_SHADER_H__
#define __CG_SHADER_H__

#include <GL/glew.h>

class Shader {
private:
  GLuint _id = 0;

public:
  Shader (const char *vshader, const char *fshader);

  GLuint id () const { return _id; }
};

#endif
