#ifndef GAME_OVER_STATE_H
#define GAME_OVER_STATE_H

#include "GameState.h"
#include <SFML/Graphics.hpp>

class GameOverState : public GameState {
public:
    GameOverState(Game* game, int finalScore, bool won);
    ~GameOverState() override = default;

    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;
    void onEnter() override;

private:
    void handleMouseClick(sf::Vector2f mousePos);
    bool isMouseOverText(const sf::Text& text, sf::Vector2f mousePos) const;

    sf::Font font;
    sf::Text gameOverText;
    sf::Text scoreText;
    sf::Text restartText;
    sf::Text menuText;
    int finalScore;
    bool won;
    int selectedOption;
    static constexpr int MENU_OPTIONS = 2;
};

#endif // GAME_OVER_STATE_H
