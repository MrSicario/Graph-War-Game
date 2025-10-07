#ifndef CHOIX_COULEUR_HPP
#define CHOIX_COULEUR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "joueur.hpp"

class ChoixCouleur {
public:
    ChoixCouleur();

    void render(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, Joueur& joueur1, Joueur& joueur2);
    bool estComplete(const Joueur& joueur1, const Joueur& joueur2) const;
    void setCurrentPlayer(int player) { currentPlayer = player; }
    void resetToPlayer1();

private:
    std::vector<sf::Color> couleurs;
    std::vector<sf::RectangleShape> rectangles;
    sf::Font font;
    sf::Text instructionText;
    sf::Text playerText;
    sf::Text confirmationText;
    int currentPlayer;
    
    void updateTexts();
    void loadFont();
    std::string getColorName(const sf::Color& color) const;
};

#endif