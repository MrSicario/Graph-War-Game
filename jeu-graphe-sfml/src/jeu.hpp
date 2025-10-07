#pragma once

#include <SFML/Graphics.hpp>
#include "joueur.hpp"
#include "graphe.hpp"

class Jeu {
public:
    Jeu();

    void update();
    void nextTurn();
    int getCurrentPlayer() const;
    void setPlayerColor(int playerIndex, const sf::Color& color);
    void render(sf::RenderWindow& window);

private:
    Joueur joueur1;
    Joueur joueur2;
    Graphe graphe;
    int tour;
};