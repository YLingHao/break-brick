#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <stack>
#include "GameState.h"

class Game {
public:
    Game();
    ~Game();

    void run();
    void pushState(std::unique_ptr<GameState> state);
    void popState();
    void changeState(std::unique_ptr<GameState> state);
    void replaceAllStates(std::unique_ptr<GameState> state);
    GameState* peekState();

    sf::RenderWindow& getWindow() { return window; }
    const sf::RenderWindow& getWindow() const { return window; }

private:
    void gameLoop();
    void handleEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow window;
    std::stack<std::unique_ptr<GameState>> states;
    sf::Clock clock;
    static constexpr float FPS = 60.0f;
    static constexpr float FRAME_TIME = 1.0f / FPS;
};

#endif // GAME_H

