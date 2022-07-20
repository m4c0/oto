#include "dumper.hpp"
#include "oto/dumper.yaml.hpp"

int main(int /*argc*/, char ** argv) {
  return poc::tool<oto::dumper::yaml>(*argv);
}
