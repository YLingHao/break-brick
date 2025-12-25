#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "GameState.h"
#include <SFML/Graphics.hpp>

class MenuState : public GameState {
public:
    MenuState(Game* game);
    ~MenuState() override = default;

    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;
    void onEnter() override;

private:
    void handleMouseClick(sf::Vector2f mousePos);
    bool isMouseOverText(const sf::Text& text, sf::Vector2f mousePos) const;

    sf::Font font;
    sf::Text titleText;
    sf::Text startText;
    sf::Text exitText;
    int selectedOption;
    static constexpr int MENU_OPTIONS = 2;
};

#endif // MENU_STATE_H

