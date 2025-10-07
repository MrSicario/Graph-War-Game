#include "jeu.hpp"
#include "joueur.hpp"
#include "graphe.hpp"
#include <SFML/Graphics.hpp>

Jeu::Jeu() {
    joueur1 = Joueur();
    joueur2 = Joueur();
    tour = 0;
    graphe = Graphe();
}

void Jeu::update() {
    if (graphe.checkVictory()) {
    }
}

void Jeu::render(sf::RenderWindow& window) {
    graphe.draw(window);
}

void Jeu::nextTurn() {
    tour = (tour + 1) % 2;
}

int Jeu::getCurrentPlayer() const {
    return tour;
}

void Jeu::setPlayerColor(int playerIndex, const sf::Color& color) {
    if (playerIndex == 0) {
        joueur1.setCouleur(color);
    } else if (playerIndex == 1) {
        joueur2.setCouleur(color);
    }
}