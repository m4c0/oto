#include "casein.hpp"

extern "C" void loggy(const char *);
void casein_event(const casein::event & e) {
  switch (e.type()) {
  case casein::CREATE_WINDOW:
    loggy("created");
    break;
  case casein::REPAINT:
    loggy("repaint");
    break;
  case casein::KEY_DOWN:
    loggy("up");
    break;
  case casein::KEY_UP:
    loggy("down");
    break;
  case casein::QUIT:
    loggy("quit");
    break;
  }
}
