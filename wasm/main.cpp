#include "casein.hpp"

extern "C" void loggy(const char *);
void casein_event(const casein::event & e) {
  loggy("ok");
}
