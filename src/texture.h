#ifndef __CG_TEXTURE_H__
#define __CG_TEXTURE_H__

#include <string>
#include <GL/glew.h>

class Texture {
private:
  GLuint _id = 0;

public:
  Texture (const std::string &file);
  ~Texture ();

  GLuint id () const { return _id; }
};

#endif
