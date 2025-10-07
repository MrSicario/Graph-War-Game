#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Dashboard {
public:
    Dashboard();
    
    void setPlayer1Color(const sf::Color& color);
    void setPlayer2Color(const sf::Color& color);
    void setCurrentPlayer(int player);
    void setPlayer1Graphs(int rectangles, int triangles);
    void setPlayer2Graphs(int rectangles, int triangles);
    void setDiceResult(int result);
    
    void draw(sf::RenderWindow& window);
    
private:
    sf::Font font;
    sf::Text objectifTitle;
    sf::Text objectifDesc;
    sf::Text player1Text, player2Text;
    sf::Text currentPlayerText;
    sf::Text player1Stats, player2Stats;
    sf::Text diceText;
    
    sf::Color player1Color, player2Color;
    int currentPlayer;
    int player1Rectangles, player1Triangles;
    int player2Rectangles, player2Triangles;
    int diceResult;
    
    void updateTexts();
    void loadFont();
    std::string colorName(const sf::Color& c) const;
};