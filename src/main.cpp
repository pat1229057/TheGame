#include "GameEngine.h"
#include <SFML/Graphics.hpp>

int main() {
  GameEngine g("game_data/assets.txt");
  g.run();
}
