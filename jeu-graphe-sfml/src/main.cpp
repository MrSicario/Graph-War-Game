#include <SFML/Graphics.hpp>
#include "jeu.hpp"
#include "ui/accueil.hpp"
#include "ui/choix_couleur.hpp"
#include "ui/lancer_de.hpp"
#include "game_state.hpp"
#include "game_grid.hpp"
#include "dashboard.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Jeu de Graphes - SFML");
    
    GameStateManager stateManager;
    Accueil accueil(window);
    ChoixCouleur choixCouleur;
    LancerDe lancerDe;
    GameGrid gameGrid(10, 10);
    Dashboard dashboard;
    Joueur joueur1, joueur2;
    
    bool gameRunning = true;
    
    auto computeCurrentPlayer = [&](int totalPoints){
        return (totalPoints % 2 == 0) ? stateManager.getStartingPlayer() : (stateManager.getStartingPlayer() == 1 ? 2 : 1);
    };

    auto getCurrentPlayerColor = [&](int currentPlayer){
        return (currentPlayer == 1) ? stateManager.getPlayer1Color() : stateManager.getPlayer2Color();
    };

    bool pendingRestart = false;
    while (window.isOpen() && gameRunning) {
        while (auto event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
                window.close();
                gameRunning = false;
    } else {
                switch (stateManager.getCurrentState()) {
    case GameState::ACCUEIL:
        accueil.handleEvent(*event, window);
        break;
    case GameState::CHOIX_COULEUR_J1:
    case GameState::CHOIX_COULEUR_J2:
           choixCouleur.handleEvent(*event, joueur1, joueur2);
           break;
    case GameState::LANCEMENT_DE:
           lancerDe.handleEvent(*event, window);
                break;
    case GameState::JEU_PRINCIPAL:
            {
        int totalPoints = 0;
        for (int y = 0; y < gameGrid.getHeight(); ++y) {
            for (int x = 0; x < gameGrid.getWidth(); ++x) {
                if (gameGrid.getPointAt(x, y).occupied) totalPoints++;
               }
           }
                int currentPlayer = computeCurrentPlayer(totalPoints);
                   sf::Color currentPlayerColor = getCurrentPlayerColor(currentPlayer);

                   if (event->is<sf::Event::MouseButtonPressed>()) {
                       auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                      if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
                          sf::Vector2i gridPos = gameGrid.screenToGrid(mouseEvent->position.x, mouseEvent->position.y);
                           Point p = gameGrid.getPointAt(gridPos.x, gridPos.y);
                           if (p.occupied && p.color == currentPlayerColor) {
                               gameGrid.startDrag(gridPos.x, gridPos.y, currentPlayerColor);
                          } else if (gameGrid.canPlacePoint(gridPos.x, gridPos.y)) {
                               bool placed = gameGrid.placePoint(gridPos.x, gridPos.y, currentPlayerColor);
                               if (placed) {
                                   if (stateManager.isDiceMessageVisible()) {
                                    stateManager.setDiceMessageVisible(false);
                                }
                            }
                         }
                    }
                   } else if (event->is<sf::Event::MouseMoved>()) {
                       auto mouse = event->getIf<sf::Event::MouseMoved>();
                       if (mouse) {
                        gameGrid.updateDragScreen(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
                       }
                 } else if (event->is<sf::Event::MouseButtonReleased>()) {
                     auto mouseEvent = event->getIf<sf::Event::MouseButtonReleased>();
                    if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
                            sf::Vector2i gridPos = gameGrid.screenToGrid(mouseEvent->position.x, mouseEvent->position.y);
                        gameGrid.endDrag(gridPos.x, gridPos.y, currentPlayerColor);
                    }
                 }
            }
            break;
            case GameState::FIN_JEU:
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    auto mb = event->getIf<sf::Event::MouseButtonPressed>();
                    if (mb && mb->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mp(mb->position.x, mb->position.y);
                        sf::FloatRect restartRect({150.f, 500.f}, {220.f, 48.f});
                        sf::FloatRect quitRect({430.f, 500.f}, {220.f, 48.f});
                        if (restartRect.contains(mp)) {
                            pendingRestart = true;
                        } else if (quitRect.contains(mp)) {
                            window.close();
                            gameRunning = false;
                        }
                     }
                }
                break;
        }
    }
}

        if (pendingRestart) {
            stateManager = GameStateManager();
            gameGrid = GameGrid(10, 10);
            dashboard = Dashboard();
            joueur1 = Joueur();
            joueur2 = Joueur();
            lancerDe.reset();
            choixCouleur.resetToPlayer1();
            stateManager.setPlayer1Color(sf::Color::Transparent);
            stateManager.setPlayer2Color(sf::Color::Transparent);
            stateManager.setState(GameState::CHOIX_COULEUR_J1);
            pendingRestart = false;
        }
        
        switch (stateManager.getCurrentState()) {
            case GameState::ACCUEIL:
                if (accueil.isStartButtonClicked()) {
                    stateManager.nextState();
                    accueil.resetStartButton();
                }
                break;
            case GameState::CHOIX_COULEUR_J1:
            case GameState::CHOIX_COULEUR_J2:
                if (joueur1.getCouleur() != sf::Color::Transparent && joueur2.getCouleur() != sf::Color::Transparent) {
                    stateManager.setPlayer1Color(joueur1.getCouleur());
                    stateManager.setPlayer2Color(joueur2.getCouleur());
                    lancerDe.reset();
                    stateManager.nextState();
                }
                break;
            case GameState::LANCEMENT_DE:
                if (lancerDe.isDiceRolled()) {
                    stateManager.setDiceResult(lancerDe.getDiceResult());
                    if (lancerDe.shouldLeave()) {
                        stateManager.nextState();
                    }
                }
                break;
            case GameState::JEU_PRINCIPAL:
                dashboard.setPlayer1Color(stateManager.getPlayer1Color());
                dashboard.setPlayer2Color(stateManager.getPlayer2Color());
                dashboard.setDiceResult(stateManager.isDiceMessageVisible() ? stateManager.getDiceResult() : 0);
                {
                    int totalPoints = 0;
                    for (int y = 0; y < gameGrid.getHeight(); ++y) {
                        for (int x = 0; x < gameGrid.getWidth(); ++x) {
                            if (gameGrid.getPointAt(x, y).occupied) totalPoints++;
                        }
                    }
                    int currentPlayer = computeCurrentPlayer(totalPoints);
                    dashboard.setCurrentPlayer(currentPlayer);

                    auto g1 = gameGrid.detectRectangles(stateManager.getPlayer1Color());
                    auto t1 = gameGrid.detectRightTriangles(stateManager.getPlayer1Color());
                    auto g2 = gameGrid.detectRectangles(stateManager.getPlayer2Color());
                    auto t2 = gameGrid.detectRightTriangles(stateManager.getPlayer2Color());
                    int p1Rect = static_cast<int>(g1.size());
                    int p1Tri  = static_cast<int>(t1.size());
                    int p2Rect = static_cast<int>(g2.size());
                    int p2Tri  = static_cast<int>(t2.size());
                    dashboard.setPlayer1Graphs(p1Rect, p1Tri);
                    dashboard.setPlayer2Graphs(p2Rect, p2Tri);

                    bool p1Win = (p1Rect >= 1 && p1Tri >= 2);
                    bool p2Win = (p2Rect >= 1 && p2Tri >= 2);
                    if (p1Win || p2Win) {
                        stateManager.setWinner(p1Win ? 1 : 2);
                        stateManager.setState(GameState::FIN_JEU);
                    }
                }
                break;
        }
        
        window.clear(sf::Color::Black);
        
        switch (stateManager.getCurrentState()) {
            case GameState::ACCUEIL:
                accueil.render(window);
                break;
            case GameState::CHOIX_COULEUR_J1:
            case GameState::CHOIX_COULEUR_J2:
                choixCouleur.render(window);
                break;
            case GameState::LANCEMENT_DE:
                lancerDe.render(window);
                break;
            case GameState::JEU_PRINCIPAL:
                gameGrid.draw(window);
                dashboard.draw(window);
                break;
            case GameState::FIN_JEU: {
                sf::RectangleShape overlay({800.f, 600.f});
                overlay.setFillColor(sf::Color(0, 0, 0, 220));
                window.draw(overlay);

                static sf::Font f;
                static bool loaded = false;
                if (!loaded) {
                    loaded = f.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") ||
                             f.openFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf") ||
                             f.openFromFile("/System/Library/Fonts/Arial.ttf") ||
                             f.openFromFile("C:/Windows/Fonts/arial.ttf");
                }

                int w = stateManager.getWinner();
                sf::Text title(f, U"Félicitations !", 36);
                title.setFillColor(sf::Color::White);
                title.setPosition({400.f - title.getGlobalBounds().size.x/2, 80.f});
                window.draw(title);

                sf::Text winText(f, (w==1 ? sf::String(U"Le joueur 1 a gagné") : sf::String(U"Le joueur 2 a gagné")), 28);
                winText.setFillColor(sf::Color(255, 230, 120));
                winText.setOutlineColor(sf::Color::Black);
                winText.setOutlineThickness(2);
                winText.setPosition({400.f - winText.getGlobalBounds().size.x/2, 130.f});
                window.draw(winText);

                static sf::Texture trophyTex;
                static bool trophyLoaded = false;
                if (!trophyLoaded) {
                    trophyLoaded = trophyTex.loadFromFile("/home/sicario/Desktop/Game Graphe/jeu-graphe-sfml/src/ui/victory.png");
                }
                if (trophyLoaded) {
                    sf::Sprite trophy(trophyTex);

                    auto sz = trophyTex.getSize();
                    float baseScaleX = 360.f / static_cast<float>(sz.x);
                    float baseScaleY = 320.f / static_cast<float>(sz.y);
                    float scale = std::min(baseScaleX, baseScaleY) * 1.20f;
                    trophy.setScale(sf::Vector2f(scale, scale));

                    float regionTop = winText.getPosition().y + winText.getGlobalBounds().size.y + 20.f;
                    float regionBottom = 500.f - 20.f;
                    float regionHeight = std::max(40.f, regionBottom - regionTop);

                    float x = 400.f - (sz.x * scale) / 2.f;
                    float centerY = regionTop + regionHeight / 2.f;
                    float y = centerY - (sz.y * scale) / 2.f;
                    trophy.setPosition(sf::Vector2f(x, y));
                    window.draw(trophy);
                }

                sf::RectangleShape restartBtn({220.f, 48.f});
                sf::RectangleShape quitBtn({220.f, 48.f});
                restartBtn.setPosition({150.f, 500.f});
                quitBtn.setPosition({430.f, 500.f});
                restartBtn.setFillColor(sf::Color(40, 140, 40));
                quitBtn.setFillColor(sf::Color(180, 50, 50));

                window.draw(restartBtn);
                window.draw(quitBtn);

                sf::Text restartText(f, U"Recommencer", 22);
                sf::Text quitText(f, U"Quitter le jeu", 22);
                restartText.setFillColor(sf::Color::White);
                quitText.setFillColor(sf::Color::White);
                restartText.setPosition({restartBtn.getPosition().x + (restartBtn.getSize().x - restartText.getGlobalBounds().size.x)/2,
                                          restartBtn.getPosition().y + 10.f});
                quitText.setPosition({quitBtn.getPosition().x + (quitBtn.getSize().x - quitText.getGlobalBounds().size.x)/2,
                                       quitBtn.getPosition().y + 10.f});
                window.draw(restartText);
                window.draw(quitText);
                break;
            }
        }
        
        window.display();
    }
    
    return 0;
}