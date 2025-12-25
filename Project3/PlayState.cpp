#include "PlayState.h"
#include "Game.h"
#include "GameOverState.h"
#include "PauseState.h"
#include "MenuState.h"
#include <algorithm>
#include <cmath>
#include <iostream>

PlayState::PlayState(Game* game)
    : GameState(game), score(0), lives(3), gameOver(false), levelComplete(false), fontLoaded(false), escKeyPressed(false) {

    // Try to load font from multiple paths
    if (font.loadFromFile("arial.ttf")) {
        fontLoaded = true;
    }
    else if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        fontLoaded = true;
    }
    else if (font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
        fontLoaded = true;
    }
    else if (font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        fontLoaded = true;
    }

    if (fontLoaded) {
        scoreText.setFont(font);
        livesText.setFont(font);
    }

    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);

    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);

    initializeBricks();
    resetGame();
}

void PlayState::initializeBricks() {
    bricks.clear();

    sf::Vector2u windowSize = game->getWindow().getSize();
    float startX = (windowSize.x - (BRICK_COLS * (BRICK_WIDTH + BRICK_SPACING) - BRICK_SPACING)) / 2.0f;
    float startY = 100.0f;

    // Create brick rows with different colors
    sf::Color colors[] = {
        sf::Color(255, 100, 100), // Red
        sf::Color(255, 200, 100), // Orange
        sf::Color(255, 255, 100), // Yellow
        sf::Color(100, 255, 100), // Green
        sf::Color(100, 200, 255)  // Blue
    };

    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            float x = startX + col * (BRICK_WIDTH + BRICK_SPACING);
            float y = startY + row * (BRICK_HEIGHT + BRICK_SPACING);
            bricks.push_back(std::make_unique<Brick>(
                x, y, BRICK_WIDTH, BRICK_HEIGHT, colors[row % 5]
            ));
        }
    }
}

void PlayState::resetGame() {
    sf::Vector2u windowSize = game->getWindow().getSize();

    // Create ball
    ball = std::make_unique<Ball>(
        windowSize.x / 2.0f,
        windowSize.y / 2.0f,
        10.0f
    );

    // Create paddle
    paddle = std::make_unique<Paddle>(
        windowSize.x / 2.0f,
        windowSize.y - 50.0f,
        120.0f,
        20.0f
    );
    paddle->setWindowSize(windowSize.x, windowSize.y);

    gameOver = false;
    levelComplete = false;
}

void PlayState::handleInput() {
    // Check for pause (ESC key) - only when game is active
    if (!gameOver && !levelComplete) {
        bool escCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
        if (escCurrentlyPressed && !escKeyPressed) {
            // ESC key was just pressed (not held)
            game->pushState(std::make_unique<PauseState>(game));
            escKeyPressed = true;
            return;
        }
        else if (!escCurrentlyPressed) {
            // ESC key was released
            escKeyPressed = false;
        }
    }

    if (gameOver || levelComplete) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            if (levelComplete) {
                // Next level (reinitialize bricks)
                initializeBricks();
                resetGame();
                score += 1000; // Level completion bonus
            }
            else {
                // Restart game
                score = 0;
                lives = 3;
                initializeBricks();
                resetGame();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            // Return to main menu
            game->changeState(std::make_unique<MenuState>(game));
        }
    }
}

void PlayState::update(float deltaTime) {
    if (gameOver || levelComplete) {
        return;
    }

    // Update ball and paddle
    ball->update(deltaTime);
    paddle->update(deltaTime);

    // Check collisions
    checkCollisions();

    // Check if ball fell out of screen bottom
    sf::Vector2u windowSize = game->getWindow().getSize();
    if (ball->getPosition().y > windowSize.y + 50) {
        lives--;
        if (lives <= 0) {
            gameOver = true;
        }
        else {
            // Reset ball position
            ball->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
            float angle = (rand() % 60 + 60) * 3.14159f / 180.0f;
            ball->setVelocity(400.0f * std::cos(angle), -400.0f * std::sin(angle));
        }
    }

    // Check if level is complete
    if (isLevelComplete()) {
        levelComplete = true;
    }

    // Update UI text
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setPosition(20, 20);

    livesText.setString("Lives: " + std::to_string(lives));
    livesText.setPosition(20, 60);
}

void PlayState::render() {
    sf::RenderWindow& window = game->getWindow();

    // Draw game objects
    paddle->draw(window);
    ball->draw(window);

    for (auto& brick : bricks) {
        brick->draw(window);
    }

    // Draw UI
    window.draw(scoreText);
    window.draw(livesText);

    // Draw game over or level complete message
    if (gameOver) {
        sf::Text gameOverText;
        if (fontLoaded) {
            gameOverText.setFont(font);
        }
        gameOverText.setString("Game Over! Press Enter to Restart");
        gameOverText.setCharacterSize(40);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = gameOverText.getLocalBounds();
        gameOverText.setPosition(
            window.getSize().x / 2.0f - bounds.width / 2.0f,
            window.getSize().y / 2.0f - bounds.height / 2.0f
        );
        window.draw(gameOverText);
    }
    else if (levelComplete) {
        sf::Text levelText;
        if (fontLoaded) {
            levelText.setFont(font);
        }
        levelText.setString("Level Complete! Press Enter to Continue");
        levelText.setCharacterSize(40);
        levelText.setFillColor(sf::Color::Green);
        levelText.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = levelText.getLocalBounds();
        levelText.setPosition(
            window.getSize().x / 2.0f - bounds.width / 2.0f,
            window.getSize().y / 2.0f - bounds.height / 2.0f
        );
        window.draw(levelText);
    }
}

void PlayState::onEnter() {
    score = 0;
    lives = 3;
    escKeyPressed = false;
    initializeBricks();
    resetGame();
}

void PlayState::checkCollisions() {
    // Ball collision with walls
    sf::Vector2u windowSize = game->getWindow().getSize();
    sf::Vector2f ballPos = ball->getPosition();
    float radius = ball->getRadius();

    if (ballPos.x - radius < 0 || ballPos.x + radius > windowSize.x) {
        ball->reverseVelocityX();
        // Ensure ball doesn't get stuck in wall
        if (ballPos.x - radius < 0) {
            ball->setPosition(radius, ballPos.y);
        }
        else {
            ball->setPosition(windowSize.x - radius, ballPos.y);
        }
    }

    if (ballPos.y - radius < 0) {
        ball->reverseVelocityY();
        ball->setPosition(ballPos.x, radius);
    }

    // Ball collision with paddle
    if (checkBallPaddleCollision(*ball, *paddle)) {
        // Adjust bounce angle based on hit position on paddle
        float paddleCenter = paddle->getPosition().x;
        float ballCenter = ball->getPosition().x;
        float hitPos = (ballCenter - paddleCenter) / (paddle->getSize().x / 2.0f);
        hitPos = std::max(-1.0f, std::min(1.0f, hitPos));

        float angle = hitPos * 60.0f * 3.14159f / 180.0f; // -60 to 60 degrees
        ball->setVelocity(400.0f * std::sin(angle), -400.0f * std::cos(angle));
    }

    // Ball collision with bricks
    for (auto& brick : bricks) {
        if (!brick->isDestroyed() && checkBallBrickCollision(*ball, *brick)) {
            brick->destroy();
            score += 10;
        }
    }
}

bool PlayState::checkBallBrickCollision(Ball& ball, Brick& brick) {
    sf::FloatRect ballBounds = ball.getBounds();
    sf::FloatRect brickBounds = brick.getBounds();

    if (!ballBounds.intersects(brickBounds)) {
        return false;
    }

    // Calculate collision direction
    sf::Vector2f ballCenter = ball.getPosition();
    sf::Vector2f brickCenter(
        brickBounds.left + brickBounds.width / 2.0f,
        brickBounds.top + brickBounds.height / 2.0f
    );

    float dx = ballCenter.x - brickCenter.x;
    float dy = ballCenter.y - brickCenter.y;

    // Determine collision direction
    if (std::abs(dx) > std::abs(dy)) {
        ball.reverseVelocityX();
    }
    else {
        ball.reverseVelocityY();
    }

    return true;
}

bool PlayState::checkBallPaddleCollision(Ball& ball, Paddle& paddle) {
    sf::FloatRect ballBounds = ball.getBounds();
    sf::FloatRect paddleBounds = paddle.getBounds();

    return ballBounds.intersects(paddleBounds);
}

bool PlayState::isLevelComplete() const {
    for (const auto& brick : bricks) {
        if (!brick->isDestroyed()) {
            return false;
        }
    }
    return true;
}

