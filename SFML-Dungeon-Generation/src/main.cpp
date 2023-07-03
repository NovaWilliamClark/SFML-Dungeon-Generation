#include "Map/Generation/DungeonGenerator.hpp"
#include "Map/Map.hpp"

int main() {
    auto window = sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dungeon Generation");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    const auto map = std::make_shared<Map>(MAP_WIDTH, MAP_HEIGHT);
    DungeonGenerator::getInstance(*map).generate();

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    const sf::Time deltaTime = sf::seconds(TIME_PER_FRAME);
    while (window.isOpen()) {
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > deltaTime) {
            timeSinceLastUpdate -= deltaTime;
            sf::Event event{};
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
        }
        window.clear();

        if (map != nullptr) {
            map->draw(window);
        }

        window.display();
    }

    return 0;
}
