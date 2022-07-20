#include "oto/dumper.yaml.hpp"
#include "poc.hpp"

int main(int /*argc*/, char ** argv) {
  return poc::tool<oto::dumper::yaml>(*argv);
}
