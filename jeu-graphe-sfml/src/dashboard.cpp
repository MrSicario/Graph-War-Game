#include "dashboard.hpp"

Dashboard::Dashboard() 
    : objectifTitle(font, "", 18)
    , objectifDesc(font, "", 14)
    , player1Text(font, "", 16)
    , player2Text(font, "", 16)
    , currentPlayerText(font, "", 16)
    , player1Stats(font, "", 14)
    , player2Stats(font, "", 14)
    , diceText(font, "", 16)
    , player1Color(sf::Color::Transparent)
    , player2Color(sf::Color::Transparent)
    , currentPlayer(1)
    , player1Rectangles(0), player1Triangles(0)
    , player2Rectangles(0), player2Triangles(0)
    , diceResult(0) {
    loadFont();
    updateTexts();
}

void Dashboard::setPlayer1Color(const sf::Color& color) {
    player1Color = color;
    updateTexts();
}

void Dashboard::setPlayer2Color(const sf::Color& color) {
    player2Color = color;
    updateTexts();
}

void Dashboard::setCurrentPlayer(int player) {
    currentPlayer = player;
    updateTexts();
}

void Dashboard::setPlayer1Graphs(int rectangles, int triangles) {
    player1Rectangles = rectangles;
    player1Triangles = triangles;
    updateTexts();
}

void Dashboard::setPlayer2Graphs(int rectangles, int triangles) {
    player2Rectangles = rectangles;
    player2Triangles = triangles;
    updateTexts();
}

void Dashboard::setDiceResult(int result) {
    diceResult = result;
    updateTexts();
}

void Dashboard::draw(sf::RenderWindow& window) {
    sf::RectangleShape background(sf::Vector2f(200, 600));
    background.setFillColor(sf::Color(50, 50, 50, 200));
    background.setPosition({600, 0});
    window.draw(background);
    
    window.draw(objectifTitle);
    window.draw(objectifDesc);
    
    window.draw(currentPlayerText);
    window.draw(player1Text);
    window.draw(player1Stats);
    window.draw(player2Text);
    window.draw(player2Stats);

    auto drawColorBox = [&](const sf::Text& label, const sf::Color& c) {
        sf::RectangleShape box({16.f, 16.f});
        box.setFillColor(c == sf::Color::Transparent ? sf::Color(120,120,120) : c);
        auto bounds = label.getLocalBounds();
        float bx = label.getPosition().x + bounds.size.x + 8.f;
        float by = label.getPosition().y - 1.f;
        box.setPosition({bx, by});
        window.draw(box);
    };
    drawColorBox(player1Text, player1Color);
    drawColorBox(player2Text, player2Color);

    auto drawResteIcons = [&](const sf::Text& baseText, const sf::Color& c, int rectLeft, int triLeft) {
        auto basePos = baseText.getPosition();
        auto bounds = baseText.getLocalBounds();
        float x = basePos.x + bounds.size.x + 10.f;
        float y = basePos.y - 2.f;

        sf::RectangleShape rectIcon({14.f, 10.f});
        rectIcon.setFillColor(c == sf::Color::Transparent ? sf::Color(120,120,120) : c);
        rectIcon.setPosition({x, y + 4});
        window.draw(rectIcon);

        sf::Text rectCount(font, "=" + std::to_string(rectLeft), 14);
        rectCount.setFillColor(sf::Color(200,200,200));
        rectCount.setPosition({x + 18.f, basePos.y});
        window.draw(rectCount);

        float tx = x + 18.f + rectCount.getLocalBounds().size.x + 16.f;
        sf::ConvexShape triIcon(3);
        triIcon.setPoint(0, {tx, y + 14});
        triIcon.setPoint(1, {tx + 12, y + 14});
        triIcon.setPoint(2, {tx, y + 2});
        triIcon.setFillColor(c == sf::Color::Transparent ? sf::Color(120,120,120) : c);
        window.draw(triIcon);

        sf::Text triCount(font, "=" + std::to_string(triLeft), 14);
        triCount.setFillColor(sf::Color(200,200,200));
        triCount.setPosition({tx + 16.f, basePos.y});
        window.draw(triCount);
    };

    int j1RectLeft = std::max(0, 1 - player1Rectangles);
    int j1TriLeft  = std::max(0, 2 - player1Triangles);
    int j2RectLeft = std::max(0, 1 - player2Rectangles);
    int j2TriLeft  = std::max(0, 2 - player2Triangles);

    drawResteIcons(player1Stats, player1Color, j1RectLeft, j1TriLeft);
    drawResteIcons(player2Stats, player2Color, j2RectLeft, j2TriLeft);

    if (diceResult > 0) {
        window.draw(diceText);
    }
}

void Dashboard::updateTexts() {
    objectifTitle = sf::Text(font, "Objectif du jeu", 18);
    objectifTitle.setFillColor(sf::Color::Yellow);
    objectifTitle.setPosition({620, 10});

    objectifDesc = sf::Text(font, "Un joueur doit remplir\nles conditions des 3 graphes\npour gagner\n( 1 Rectangle et 2 Triangles )", 12);
    objectifDesc.setFillColor(sf::Color(220,220,220));
    objectifDesc.setPosition({620, 40});

    float y = objectifDesc.getPosition().y + objectifDesc.getLocalBounds().size.y + 20.f;

    currentPlayerText = sf::Text(font, "Tour: Joueur " + std::to_string(currentPlayer), 16);
    currentPlayerText.setFillColor(sf::Color::White);
    currentPlayerText.setPosition({620, y});
    y += 30.f;

    player1Text = sf::Text(font, std::string("J1 :"), 16);
    player1Text.setFillColor(sf::Color::White);
    player1Text.setPosition({620, y});
    y += 22.f;

    player1Stats = sf::Text(font, "Reste :", 14);
    player1Stats.setFillColor(sf::Color(200,200,200));
    player1Stats.setPosition({620, y});
    y += 34.f;

    player2Text = sf::Text(font, std::string("J2 :"), 16);
    player2Text.setFillColor(sf::Color::White);
    player2Text.setPosition({620, y});
    y += 22.f;

    player2Stats = sf::Text(font, "Reste :", 14);
    player2Stats.setFillColor(sf::Color(200,200,200));
    player2Stats.setPosition({620, y});
    y += 34.f;

    if (diceResult > 0) {
        sf::String s = U"Dé: J";
        s += std::to_string(diceResult);
        s += U" commence";
        diceText = sf::Text(font, s, 16);
        diceText.setFillColor(sf::Color::Yellow);
        diceText.setPosition({620, y});
    } else {
        diceText = sf::Text(font, "", 16);
    }
}

void Dashboard::loadFont() {
    if (!font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") &&
        !font.openFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf") &&
        !font.openFromFile("/System/Library/Fonts/Arial.ttf") &&
        !font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
    }
}

std::string Dashboard::colorName(const sf::Color& c) const {
    if (c == sf::Color::Red) return "ROUGE";
    if (c == sf::Color::Green) return "VERT";
    if (c == sf::Color::Blue) return "BLEU";
    if (c == sf::Color::Yellow) return "JAUNE";
    if (c == sf::Color::Magenta) return "MAGENTA";
    if (c == sf::Color::Cyan) return "CYAN";
    if (c == sf::Color(255,165,0)) return "ORANGE";
    if (c == sf::Color(128,0,128)) return "VIOLET";
    if (c == sf::Color(0,128,128)) return "TURQUOISE";
    if (c == sf::Color(128,128,0)) return "OLIVE";
    if (c == sf::Color(255,192,203)) return "ROSE";
    if (c == sf::Color(0,0,0)) return "NOIR";
    if (c == sf::Color::White) return "BLANC";
    return "INCONNUE";
}