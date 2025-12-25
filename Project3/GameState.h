#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SFML/Graphics.hpp>

class Game;

class GameState {
public:
    virtual ~GameState() = default;
    virtual void handleInput() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void onEnter() {}
    virtual void onExit() {}

protected:
    GameState(Game* game) : game(game) {}
    Game* game;
};

#endif // GAME_STATE_H

