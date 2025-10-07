#pragma once

#include <SFML/Graphics.hpp>

enum class GameState {
    ACCUEIL,
    CHOIX_COULEUR_J1,
    CHOIX_COULEUR_J2,
    LANCEMENT_DE,
    JEU_PRINCIPAL,
    FIN_JEU
};

class GameStateManager {
public:
    GameStateManager();
    
    GameState getCurrentState() const { return currentState; }
    void setState(GameState newState) { currentState = newState; }
    void nextState();
    
    void setPlayer1Color(const sf::Color& color) { player1Color = color; }
    void setPlayer2Color(const sf::Color& color) { player2Color = color; }
    sf::Color getPlayer1Color() const { return player1Color; }
    sf::Color getPlayer2Color() const { return player2Color; }
    
    void setDiceResult(int result) { diceResult = result; diceMessageVisible = (result != 0); }
    int getDiceResult() const { return diceResult; }
    int getStartingPlayer() const { return diceResult == 1 ? 1 : 2; }

    void setDiceMessageVisible(bool visible) { diceMessageVisible = visible; }
    bool isDiceMessageVisible() const { return diceMessageVisible; }

    void setWinner(int w) { winner = w; }
    int getWinner() const { return winner; }
    
    bool isPlayer1ColorSelected() const { return player1Color != sf::Color::Transparent; }
    bool isPlayer2ColorSelected() const { return player2Color != sf::Color::Transparent; }
    bool isDiceRolled() const { return diceResult != 0; }

private:
    GameState currentState;
    sf::Color player1Color;
    sf::Color player2Color;
    int diceResult;
    bool diceMessageVisible;
    int winner = 0;
};