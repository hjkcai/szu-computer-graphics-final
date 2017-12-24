#ifndef __CG_APP_H__
#define __CG_APP_H__

#include <string>
#include "context.h"

class Application {
private:
  Context *ctx;

public:
  Application (const std::string &title);
  ~Application ();

  void mainLoop ();
  void handleWindowEvents ();
};

#endif
