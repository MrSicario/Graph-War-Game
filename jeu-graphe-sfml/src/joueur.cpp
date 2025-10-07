#include "joueur.hpp"
#include "graphe.hpp"

Joueur::Joueur() : nom(""), couleur(sf::Color::Transparent) {}

Joueur::Joueur(const std::string& nom, const sf::Color& couleur) : nom(nom), couleur(couleur) {}

sf::Color Joueur::getCouleur() const {
    return couleur;
}

void Joueur::setCouleur(const sf::Color& nouvelleCouleur) {
    couleur = nouvelleCouleur;
}

void Joueur::ajouterGraphe(Graphe* graphe) {
    graphes.push_back(graphe);
}

const std::vector<Graphe*>& Joueur::getGraphes() const {
    return graphes;
}