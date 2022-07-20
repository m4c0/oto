#include "dumper.hpp"
#include "oto/dumper.action.hpp"

template<typename D>
struct dumper : public oto::dumper::action<D> {
  using oto::dumper::action<D>::action;

  const oto::transition<D> * choose(oto::opcodes::choose<D> c) override {
    return &c[1].next;
  }
};

int main(int /*argc*/, char ** argv) {
  return poc::tool<dumper>(*argv);
}
