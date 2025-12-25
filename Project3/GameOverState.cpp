#include "GameOverState.h"
#include "Game.h"
#include "MenuState.h"
#include "PlayState.h"
#include <iostream>

GameOverState::GameOverState(Game* game, int finalScore, bool won)
    : GameState(game), finalScore(finalScore), won(won), selectedOption(0) {

    // Try to load font from multiple paths
    bool fontLoaded = false;
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
        gameOverText.setFont(font);
        scoreText.setFont(font);
        restartText.setFont(font);
        menuText.setFont(font);
    }

    gameOverText.setString(won ? "Congratulations! You Won!" : "Game Over");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(won ? sf::Color::Green : sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);

    scoreText.setString("Final Score: " + std::to_string(finalScore));
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);

    restartText.setString("Restart (Enter)");
    restartText.setCharacterSize(35);
    restartText.setFillColor(sf::Color::White);

    menuText.setString("Main Menu (Esc)");
    menuText.setCharacterSize(35);
    menuText.setFillColor(sf::Color::White);
}

void GameOverState::handleInput() {
    // Keyboard input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        selectedOption = 0;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        selectedOption = 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        if (selectedOption == 0) {
            // Restart game - clear all states and create new PlayState
            game->replaceAllStates(std::make_unique<PlayState>(game));
        }
        else {
            game->changeState(std::make_unique<MenuState>(game));
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        game->changeState(std::make_unique<MenuState>(game));
    }

    // Mouse input
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f mousePos = sf::Vector2f(
            sf::Mouse::getPosition(game->getWindow()).x,
            sf::Mouse::getPosition(game->getWindow()).y
        );
        handleMouseClick(mousePos);
    }
}

void GameOverState::update(float deltaTime) {
    restartText.setFillColor(selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
    menuText.setFillColor(selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);
}

void GameOverState::render() {
    sf::RenderWindow& window = game->getWindow();
    sf::Vector2u windowSize = window.getSize();

    // Center all text
    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(windowSize.x / 2.0f - gameOverBounds.width / 2.0f, 150);

    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setPosition(windowSize.x / 2.0f - scoreBounds.width / 2.0f, 250);

    sf::FloatRect restartBounds = restartText.getLocalBounds();
    restartText.setPosition(windowSize.x / 2.0f - restartBounds.width / 2.0f, 350);

    sf::FloatRect menuBounds = menuText.getLocalBounds();
    menuText.setPosition(windowSize.x / 2.0f - menuBounds.width / 2.0f, 420);

    window.draw(gameOverText);
    window.draw(scoreText);
    window.draw(restartText);
    window.draw(menuText);
}

void GameOverState::onEnter() {
    selectedOption = 0;
}

void GameOverState::handleMouseClick(sf::Vector2f mousePos) {
    if (isMouseOverText(restartText, mousePos)) {
        // Restart game - clear all states and create new PlayState
        game->replaceAllStates(std::make_unique<PlayState>(game));
    }
    else if (isMouseOverText(menuText, mousePos)) {
        game->changeState(std::make_unique<MenuState>(game));
    }
}

bool GameOverState::isMouseOverText(const sf::Text& text, sf::Vector2f mousePos) const {
    sf::FloatRect bounds = text.getGlobalBounds();
    return bounds.contains(mousePos);
}

