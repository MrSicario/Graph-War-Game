#ifndef LANCER_DE_HPP
#define LANCER_DE_HPP

#include <SFML/Graphics.hpp>
#include <random>

class LancerDe {
public:
    LancerDe();
    int lancer();
    void render(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void update(sf::RenderWindow& window);
    bool isDiceRolled() const { return diceRolled; }
    int getDiceResult() const { return diceResult; }
    bool shouldLeave();
    void reset();

private:
    sf::Texture textureDe;
    sf::Sprite spriteDe;
    sf::Font font;
    sf::Text instructionText;
    sf::Text diceText;
    sf::RectangleShape rollButton;
    sf::Text rollButtonText;
    std::mt19937 rng;
    std::uniform_int_distribution<int> distribution;
    bool diceRolled;
    int diceResult;

    bool isRolling = false;
    sf::Clock rollingClock;
    float rollingDuration = 0.9f;
    float angle = 0.f;

    sf::Clock resultClock;
    float resultHold = 1.8f;
    
    void updateTexts();
    void loadFont();
};

#endif