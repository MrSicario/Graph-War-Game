#include "accueil.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>

Accueil::Accueil() : title(font, "GRAPH WARS", 60), startButtonText(font, "COMMENCER LE JEU", 24), startButtonClicked(false) {
    bool fontLoaded = false;
    if (font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf") ||
        font.openFromFile("/usr/share/fonts/TTF/DejaVuSans-Bold.ttf") ||
        font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") ||
        font.openFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf") ||
        font.openFromFile("/System/Library/Fonts/Arial.ttf") ||
        font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        fontLoaded = true;
    }
    
    if (!fontLoaded) {
        font = sf::Font();
    }
    
    title.setFillColor(sf::Color::Cyan);
    title.setOutlineColor(sf::Color::Blue);
    title.setOutlineThickness(3);
    title.setPosition({400 - title.getGlobalBounds().size.x/2, 80});

    startButtonShape.setSize(sf::Vector2f(320, 80));
    startButtonShape.setFillColor(sf::Color(0, 150, 0));
    startButtonShape.setOutlineColor(sf::Color(0, 200, 0));
    startButtonShape.setOutlineThickness(4);
    startButtonShape.setPosition({400 - 160, 280});

    startButtonText.setFillColor(sf::Color::White);
    startButtonText.setOutlineColor(sf::Color::Black);
    startButtonText.setOutlineThickness(2);
    startButtonText.setPosition({400 - startButtonText.getGlobalBounds().size.x/2, 320 - startButtonText.getGlobalBounds().size.y/2});
}

Accueil::Accueil(sf::RenderWindow&) : Accueil() {}

void Accueil::handleEvent(sf::Event event, sf::RenderWindow&) {
    if (event.is<sf::Event::MouseMoved>()) {
        auto mouse = event.getIf<sf::Event::MouseMoved>();
        if (mouse) {
            sf::Vector2f pos(mouse->position.x, mouse->position.y);
            bool over = startButtonShape.getGlobalBounds().contains(pos);
            if (over && !isHovering) {
                isHovering = true;
                hoverClock.restart();
            } else if (!over && isHovering) {
                isHovering = false;
                hoverProgress = 0.f;
            }
        }
    }

    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(mouseEvent->position.x, mouseEvent->position.y);
            if (startButtonShape.getGlobalBounds().contains(mousePos)) {
                startButtonClicked = true;
            }
        }
    }
}

void Accueil::render(sf::RenderWindow& window) {
    if (isHovering) {
        float t = hoverClock.getElapsedTime().asSeconds();
        hoverProgress = std::min(1.f, t / 0.6f);
        sf::Color base = sf::Color(0, 150, 0);
        sf::Color glow = sf::Color(0, 200, 0);
        sf::Color mix(
            static_cast<std::uint8_t>(base.r + (glow.r - base.r) * 0.3f),
            static_cast<std::uint8_t>(base.g + (glow.g - base.g) * 0.3f),
            static_cast<std::uint8_t>(base.b + (glow.b - base.b) * 0.3f)
        );
        startButtonShape.setFillColor(mix);
    } else {
        startButtonShape.setFillColor(sf::Color(0, 150, 0));
        hoverProgress = 0.f;
    }

    startButtonText.setPosition({
        startButtonShape.getPosition().x + startButtonShape.getSize().x / 2 - startButtonText.getGlobalBounds().size.x/2,
        startButtonShape.getPosition().y + startButtonShape.getSize().y / 2 - startButtonText.getGlobalBounds().size.y/2
    });

    window.draw(title);

    const float r = 16.f;
    const float border = 4.f;
    auto pos = startButtonShape.getPosition();
    auto size = startButtonShape.getSize();

    auto drawRounded = [&](sf::Vector2f p, sf::Vector2f s, sf::Color fill){
        sf::RectangleShape center({s.x - 2*r, s.y});
        center.setPosition({p.x + r, p.y});
        center.setFillColor(fill);
        window.draw(center);
        sf::RectangleShape middle({s.x, s.y - 2*r});
        middle.setPosition({p.x, p.y + r});
        middle.setFillColor(fill);
        window.draw(middle);
        sf::CircleShape c(r);
        c.setFillColor(fill);
        c.setPointCount(24);
        c.setPosition({p.x, p.y});                    window.draw(c);
        c.setPosition({p.x + s.x - 2*r, p.y});        window.draw(c);
        c.setPosition({p.x, p.y + s.y - 2*r});        window.draw(c);
        c.setPosition({p.x + s.x - 2*r, p.y + s.y - 2*r}); window.draw(c);
    };

    drawRounded({pos.x - border, pos.y - border}, {size.x + 2*border, size.y + 2*border}, startButtonShape.getOutlineColor());
    drawRounded(pos, size, startButtonShape.getFillColor());

    if (hoverProgress > 0.f) {
        float w = startButtonShape.getSize().x * hoverProgress;
        sf::RectangleShape bar({w, 6});
        bar.setFillColor(sf::Color(255, 255, 255, 180));
        bar.setPosition({startButtonShape.getPosition().x, startButtonShape.getPosition().y + startButtonShape.getSize().y - 6});
        window.draw(bar);
    }

    window.draw(startButtonText);
}

void Accueil::run() {
}