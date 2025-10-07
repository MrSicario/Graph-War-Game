#ifndef GRAPHE_HPP
#define GRAPHE_HPP

#include <vector>
#include <SFML/Graphics.hpp>

class Graphe {
public:
    Graphe();

    void draw(sf::RenderWindow& window);
    bool checkVictory();
    void update();

private:
};

#endif