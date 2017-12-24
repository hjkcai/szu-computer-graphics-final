#ifndef __CG_TEXTURE_H__
#define __CG_TEXTURE_H__

#include <GL/glew.h>

class Texture {
private:
  GLuint _id = 0;

public:
  Texture (const char *file);
  ~Texture ();

  void bind () const;
};

#endif
