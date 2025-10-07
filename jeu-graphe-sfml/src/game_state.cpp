#include "game_state.hpp"

GameStateManager::GameStateManager() 
    : currentState(GameState::ACCUEIL)
    , player1Color(sf::Color::Transparent)
    , player2Color(sf::Color::Transparent)
    , diceResult(0)
    , diceMessageVisible(false) {
}

void GameStateManager::nextState() {
    switch (currentState) {
        case GameState::ACCUEIL:
            currentState = GameState::CHOIX_COULEUR_J1;
            break;
        case GameState::CHOIX_COULEUR_J1:
            if (isPlayer1ColorSelected()) {
                currentState = GameState::CHOIX_COULEUR_J2;
            }
            break;
        case GameState::CHOIX_COULEUR_J2:
            if (isPlayer2ColorSelected()) {
                currentState = GameState::LANCEMENT_DE;
            }
            break;
        case GameState::LANCEMENT_DE:
            if (isDiceRolled()) {
                currentState = GameState::JEU_PRINCIPAL;
            }
            break;
        case GameState::JEU_PRINCIPAL:
            currentState = GameState::FIN_JEU;
            break;
        case GameState::FIN_JEU:
            currentState = GameState::ACCUEIL;
            break;
    }
}