#include "lancer_de.hpp"
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>

LancerDe::LancerDe() 
    : spriteDe(textureDe)
    , instructionText(font, "", 24)
    , diceText(font, "", 20)
    , rollButtonText(font, "", 18)
    , rng(std::random_device{}())
    , distribution(1, 2)
    , diceRolled(false)
    , diceResult(0) {
    loadFont();

    {
        std::random_device rd;
        auto t = static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count());
        std::seed_seq seq{rd(), rd(), rd(), rd(), rd(), t,
                          static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this))};
        rng.seed(seq);
    }
    
    rollButton.setSize(sf::Vector2f(250, 60));
    rollButton.setFillColor(sf::Color(0, 100, 200));
    rollButton.setOutlineColor(sf::Color(0, 150, 255));
    rollButton.setOutlineThickness(3);
    rollButton.setPosition({275, 380});
    
    updateTexts();
}

int LancerDe::lancer() {
    return distribution(rng);
}

void LancerDe::render(sf::RenderWindow& window) {
    if (isRolling && rollingClock.getElapsedTime().asSeconds() >= rollingDuration) {
        isRolling = false;
        diceResult = lancer();
        diceRolled = true;
        resultClock.restart();
        updateTexts();
    }

    window.draw(instructionText);
    window.draw(diceText);
    
    sf::RectangleShape dice(sf::Vector2f(100, 100));
    dice.setOrigin({50, 50});
    dice.setFillColor(sf::Color::White);
    dice.setOutlineColor(sf::Color::Black);
    dice.setOutlineThickness(3);
    dice.setPosition({400, 300});

    if (isRolling) {
        angle = 720.f * rollingClock.getElapsedTime().asSeconds();
        dice.setRotation(sf::degrees(angle));
    }

    window.draw(dice);

    sf::Text center(font, "", 40);
    center.setFillColor(sf::Color::Black);
    center.setOutlineColor(sf::Color::White);
    center.setOutlineThickness(2);

    if (isRolling) {
        int idx = static_cast<int>(rollingClock.getElapsedTime().asMilliseconds() / 120) % 2;
        center = sf::Text(font, idx == 0 ? "J1" : "J2", 40);
        center.setFillColor(idx == 0 ? sf::Color::Red : sf::Color::Blue);
    } else if (diceRolled) {
        center = sf::Text(font, diceResult == 1 ? "J1" : "J2", 40);
        center.setFillColor(diceResult == 1 ? sf::Color::Red : sf::Color::Blue);
    } else {
        center = sf::Text(font, "?", 50);
        center.setFillColor(sf::Color::Black);
    }
    center.setPosition({400 - center.getGlobalBounds().size.x/2, 300 - center.getGlobalBounds().size.y/2});
    window.draw(center);
    
    if (diceRolled) {
        sf::Text winner(font, (diceResult == 1 ? std::string("J1 commence !") : std::string("J2 commence !")), 30);
        winner.setFillColor(sf::Color::Yellow);
        winner.setOutlineColor(sf::Color::Black);
        winner.setOutlineThickness(2);
        winner.setPosition({400 - winner.getGlobalBounds().size.x/2, 450});
        window.draw(winner);
    }
    
    if (!diceRolled && !isRolling) {
        window.draw(rollButton);
        window.draw(rollButtonText);
    }
}

void LancerDe::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!diceRolled && !isRolling && event.is<sf::Event::MouseButtonPressed>()) {
        auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
            if (rollButton.getGlobalBounds().contains(sf::Vector2f(mouseEvent->position.x, mouseEvent->position.y))) {
                isRolling = true;
                diceRolled = false;
                diceResult = 0;
                rollingClock.restart();
                resultClock.restart();
                angle = 0.f;
                updateTexts();
            }
        }
    }
}

void LancerDe::update(sf::RenderWindow& window) {
    while (auto event = window.pollEvent()) {
        handleEvent(*event, window);
    }
}

bool LancerDe::shouldLeave() {
    return diceRolled && !isRolling && resultClock.getElapsedTime().asSeconds() >= resultHold;
}

void LancerDe::reset() {
    isRolling = false;
    diceRolled = false;
    diceResult = 0;
    angle = 0.f;
    rollingClock.restart();
    resultClock.restart();
    updateTexts();
}

void LancerDe::updateTexts() {
    instructionText = sf::Text(font, U"Lancez le dé pour choisir qui commence (J1 / J2)", 24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setOutlineColor(sf::Color::Black);
    instructionText.setOutlineThickness(2);
    instructionText.setPosition({400 - instructionText.getGlobalBounds().size.x/2, 100});
    
    rollButtonText = sf::Text(font, U"LANCER LE DÉ", 18);
    rollButtonText.setFillColor(sf::Color::White);
    rollButtonText.setOutlineColor(sf::Color::Black);
    rollButtonText.setOutlineThickness(1);
    rollButtonText.setPosition({400 - rollButtonText.getGlobalBounds().size.x/2, 410 - rollButtonText.getGlobalBounds().size.y/2});
    
    if (diceRolled) {
        sf::String s = (diceResult == 1 ? sf::String(U"J1") : sf::String(U"J2"));
        s += U" commence !";
        diceText = sf::Text(font, s, 20);
        diceText.setFillColor(sf::Color::Yellow);
        diceText.setOutlineColor(sf::Color::Black);
        diceText.setOutlineThickness(2);
        diceText.setPosition({400 - diceText.getGlobalBounds().size.x/2, 200});
    } else if (isRolling) {
        diceText = sf::Text(font, U"Le dé tourne...", 20);
        diceText.setFillColor(sf::Color::Cyan);
        diceText.setOutlineColor(sf::Color::Black);
        diceText.setOutlineThickness(2);
        diceText.setPosition({400 - diceText.getGlobalBounds().size.x/2, 200});
    } else {
        diceText = sf::Text(font, U"Cliquez pour lancer le dé", 20);
        diceText.setFillColor(sf::Color::Cyan);
        diceText.setOutlineColor(sf::Color::Black);
        diceText.setOutlineThickness(2);
        diceText.setPosition({400 - diceText.getGlobalBounds().size.x/2, 200});
    }
}

void LancerDe::loadFont() {
    if (!font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") &&
        !font.openFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf") &&
        !font.openFromFile("/System/Library/Fonts/Arial.ttf") &&
        !font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
    }
}