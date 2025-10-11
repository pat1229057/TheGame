#include <SFML/Graphics.hpp>
#include <format>
#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>

int main() {

  sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);
  sf::Clock deltaClock;

  while (window.isOpen()) {

    while (std::optional<sf::Event> event = window.pollEvent()) {

      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::Begin("Hello, world!");
    if (ImGui::Button("Press me")) {
      std::cout << "Button Pressed\n";
    }
    ImGui::End();
    window.clear(sf::Color::Cyan);
    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
  return 0;
}
