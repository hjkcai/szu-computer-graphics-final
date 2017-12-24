#ifndef __CG_RENDER_H__
#define __CG_RENDER_H__

#include <GL/glew.h>

class Renderer {
private:
  GLbitfield paramsOfClearing = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

public:
  Renderer ();

  void setClearParams (const GLbitfield &params);

  void clear () const;
};

#endif
