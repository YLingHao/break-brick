#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "GameState.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class PauseState;

class PlayState : public GameState {
public:
    PlayState(Game* game);
    ~PlayState() override = default;

    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;
    void onEnter() override;

private:
    void initializeBricks();
    void checkCollisions();
    bool checkBallBrickCollision(Ball& ball, Brick& brick);
    bool checkBallPaddleCollision(Ball& ball, Paddle& paddle);
    void resetGame();
    bool isLevelComplete() const;

    std::unique_ptr<Ball> ball;
    std::unique_ptr<Paddle> paddle;
    std::vector<std::unique_ptr<Brick>> bricks;

    sf::Font font;
    sf::Text scoreText;
    sf::Text livesText;
    bool fontLoaded;

    int score;
    int lives;
    bool gameOver;
    bool levelComplete;
    bool escKeyPressed;  // Flag to prevent repeated ESC key triggers

    static constexpr int BRICK_ROWS = 5;
    static constexpr int BRICK_COLS = 10;
    static constexpr float BRICK_WIDTH = 80.0f;
    static constexpr float BRICK_HEIGHT = 30.0f;
    static constexpr float BRICK_SPACING = 5.0f;
};

#endif // PLAY_STATE_H

