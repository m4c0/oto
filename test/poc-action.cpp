#include "oto/dumper.action.hpp"
#include "poc.hpp"

int main(int /*argc*/, char ** argv) {
  return poc::tool<oto::dumper::action>(*argv);
}
