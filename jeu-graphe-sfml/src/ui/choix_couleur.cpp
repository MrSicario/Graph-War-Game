#include "choix_couleur.hpp"
#include "joueur.hpp"

ChoixCouleur::ChoixCouleur() 
    : instructionText(font, "", 24)
    , playerText(font, "", 20)
    , confirmationText(font, "", 18)
    , currentPlayer(1) {
    couleurs = {
        sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow,
        sf::Color::Magenta, sf::Color::Cyan, sf::Color(255, 165, 0),
        sf::Color(128, 0, 128), sf::Color(0, 128, 128), sf::Color(128, 128, 0),
        sf::Color(255, 192, 203), sf::Color(0, 0, 0)
    };
    rectangles.clear();
    for (size_t i = 0; i < couleurs.size(); ++i) {
        sf::RectangleShape rectangle(sf::Vector2f(50, 50));
        rectangle.setFillColor(couleurs[i]);
        rectangle.setPosition({100.0f + i * 60.0f, 200.0f});
        rectangles.push_back(rectangle);
    }
    loadFont();
    updateTexts();
}

void ChoixCouleur::resetToPlayer1() {
    loadFont();
    currentPlayer = 1;
    instructionText = sf::Text(font, "Ce jeu se joue localement et par 2 joueurs", 24);
    playerText = sf::Text(font, "Joueur 1, choisissez votre couleur", 20);
    confirmationText = sf::Text(font, "", 18);

    instructionText.setFillColor(sf::Color::White);
    playerText.setFillColor(sf::Color::Yellow);
    confirmationText.setFillColor(sf::Color::Green);

    instructionText.setPosition({400 - instructionText.getGlobalBounds().size.x/2, 50});
    playerText.setPosition({400 - playerText.getGlobalBounds().size.x/2, 100});
    confirmationText.setPosition({400 - confirmationText.getGlobalBounds().size.x/2, 150});
}

void ChoixCouleur::render(sf::RenderWindow& window) {
    window.draw(instructionText);
    window.draw(playerText);
    window.draw(confirmationText);
    
    for (const auto& rectangle : rectangles) {
        window.draw(rectangle);
    }
}

void ChoixCouleur::handleEvent(const sf::Event& event, Joueur& joueur1, Joueur& joueur2) {
    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
            for (size_t i = 0; i < rectangles.size(); ++i) {
                if (rectangles[i].getGlobalBounds().contains(sf::Vector2f(mouseEvent->position.x, mouseEvent->position.y))) {
                    if (currentPlayer == 1 && joueur1.getCouleur() == sf::Color::Transparent) {
                        joueur1.setCouleur(couleurs[i]);
                        confirmationText = sf::Text(font, "J1 vous avez choisis " + getColorName(couleurs[i]), 18);
                        confirmationText.setFillColor(sf::Color::Green);
                        confirmationText.setPosition({400 - confirmationText.getGlobalBounds().size.x/2, 560});
                        currentPlayer = 2;
                        updateTexts();
                    } else if (currentPlayer == 2 && joueur2.getCouleur() == sf::Color::Transparent && couleurs[i] != joueur1.getCouleur()) {
                        joueur2.setCouleur(couleurs[i]);
                        confirmationText = sf::Text(font, "J2 vous avez choisi " + getColorName(couleurs[i]), 18);
                        confirmationText.setFillColor(sf::Color::Green);
                        confirmationText.setPosition({400 - confirmationText.getGlobalBounds().size.x/2, 560});
                        updateTexts();
                    }
                }
            }
        }
    }
}

bool ChoixCouleur::estComplete(const Joueur& joueur1, const Joueur& joueur2) const {
    return joueur1.getCouleur() != sf::Color::Transparent && joueur2.getCouleur() != sf::Color::Transparent;
}

void ChoixCouleur::updateTexts() {
    instructionText = sf::Text(font, "Ce jeu se joue localement et par 2 joueurs", 24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition({400 - instructionText.getGlobalBounds().size.x/2, 50});
    
    playerText = sf::Text(font, "Joueur " + std::to_string(currentPlayer) + ", choisissez votre couleur", 20);
    playerText.setFillColor(sf::Color::Yellow);
    playerText.setPosition({400 - playerText.getGlobalBounds().size.x/2, 100});
    
    confirmationText = sf::Text(font, "", 18);
    confirmationText.setFillColor(sf::Color::Green);
    confirmationText.setPosition({400 - confirmationText.getGlobalBounds().size.x/2, 150});
}

void ChoixCouleur::loadFont() {
    if (!font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") &&
        !font.openFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf") &&
        !font.openFromFile("/System/Library/Fonts/Arial.ttf") &&
        !font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
    }
}

std::string ChoixCouleur::getColorName(const sf::Color& color) const {
    if (color == sf::Color::Red) return "ROUGE";
    if (color == sf::Color::Green) return "VERT";
    if (color == sf::Color::Blue) return "BLEU";
    if (color == sf::Color::Yellow) return "JAUNE";
    if (color == sf::Color::Magenta) return "MAGENTA";
    if (color == sf::Color::Cyan) return "CYAN";
    if (color == sf::Color(255, 165, 0)) return "ORANGE";
    if (color == sf::Color(128, 0, 128)) return "VIOLET";
    if (color == sf::Color(0, 128, 128)) return "TURQUOISE";
    if (color == sf::Color(128, 128, 0)) return "OLIVE";
    if (color == sf::Color(255, 192, 203)) return "ROSE";
    if (color == sf::Color(0, 0, 0)) return "NOIR";
    return "INCONNUE";
}