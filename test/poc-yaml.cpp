#include "oto/dumper.yaml.hpp"
#include "poc.hpp"

#include <fstream>
#include <iostream>

namespace poc {}

int main() {
  try {
    std::fstream out { "poc.yaml" };
    oto::dumper::yaml<poc::domain>(out).dump(poc::game);
  } catch (const oto::dumper::unnamed_choice & e) {
    std::cerr << "ERROR: Found unnamed choice\n";
    return 1;
  } catch (const oto::dumper::unnamed_scene & e) {
    std::cerr << "ERROR: Found unnamed scene\n";
    return 1;
  } catch (const oto::dumper::actor_without_cast & e) {
    std::cerr << "ERROR: Found actor without cast - " << e.what() << "\n";
    return 1;
  }
}
