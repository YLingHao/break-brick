#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include "GameState.h"
#include <SFML/Graphics.hpp>

class PauseState : public GameState {
public:
    PauseState(Game* game);
    ~PauseState() override = default;

    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;
    void onEnter() override;

private:
    void handleMouseClick(sf::Vector2f mousePos);
    bool isMouseOverText(const sf::Text& text, sf::Vector2f mousePos) const;

    sf::Font font;
    sf::Text pauseText;
    sf::Text resumeText;
    sf::Text restartText;
    sf::Text menuText;
    sf::Text exitText;
    int selectedOption;
    bool fontLoaded;
    bool enterKeyPressed;
    bool escKeyPressed;
    bool upKeyPressed;
    bool downKeyPressed;
    static constexpr int MENU_OPTIONS = 4;
};

#endif // PAUSE_STATE_H
