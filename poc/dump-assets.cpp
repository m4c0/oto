#include "dumper.hpp"
#include "oto/dumper.assets.hpp"

int main(int /*argc*/, char ** argv) {
  return poc::tool<oto::dumper::assets>(*argv);
}
