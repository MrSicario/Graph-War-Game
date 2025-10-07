#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "graphe.hpp"

class Joueur {
public:
    Joueur();
    Joueur(const std::string& nom, const sf::Color& couleur);

    void setCouleur(const sf::Color& couleur);
    sf::Color getCouleur() const;

    void ajouterGraphe(Graphe* graphe);
    const std::vector<Graphe*>& getGraphes() const;

private:
    std::string nom;
    sf::Color couleur;
    std::vector<Graphe*> graphes;
};