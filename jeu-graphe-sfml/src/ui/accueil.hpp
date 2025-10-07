#pragma once

#include <SFML/Graphics.hpp>

class Accueil {
public:
    Accueil();
    Accueil(sf::RenderWindow& window);

    void run();
    void render(sf::RenderWindow& window);
    void handleEvent(sf::Event event, sf::RenderWindow& window);
    bool isStartButtonClicked() const { return startButtonClicked; }
    void resetStartButton() { startButtonClicked = false; }

private:
    sf::Font font;
    sf::Text title;
    sf::RectangleShape startButtonShape;
    sf::Text startButtonText;
    bool startButtonClicked;

    bool isHovering = false;
    sf::Clock hoverClock;
    float hoverProgress = 0;
};