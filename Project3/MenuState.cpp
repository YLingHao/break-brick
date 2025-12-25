#include "MenuState.h"
#include "Game.h"
#include "PlayState.h"
#include <iostream>

MenuState::MenuState(Game* game) : GameState(game), selectedOption(0) {
    // Try to load font from multiple paths
    bool fontLoaded = false;

    // Try current directory
    if (font.loadFromFile("arial.ttf")) {
        fontLoaded = true;
    }
    // Windows system font path
    else if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        fontLoaded = true;
    }
    // Linux common font path
    else if (font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
        fontLoaded = true;
    }
    // macOS font path
    else if (font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        fontLoaded = true;
    }

    if (!fontLoaded) {
        std::cerr << "Warning: Failed to load font file, text may not display" << std::endl;
        std::cerr << "Please refer to FONT_SETUP.md for font setup" << std::endl;
    }

    if (fontLoaded) {
        titleText.setFont(font);
        startText.setFont(font);
        exitText.setFont(font);
    }

    titleText.setString("Breakout Game");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);

    startText.setString("Start Game (Enter)");
    startText.setCharacterSize(40);
    startText.setFillColor(sf::Color::White);

    exitText.setString("Exit Game (Esc)");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::White);
}

void MenuState::handleInput() {
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
            game->changeState(std::make_unique<PlayState>(game));
        }
        else {
            game->getWindow().close();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        game->getWindow().close();
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

void MenuState::update(float deltaTime) {
    // Update text color
    startText.setFillColor(selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
    exitText.setFillColor(selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);
}

void MenuState::render() {
    sf::RenderWindow& window = game->getWindow();
    sf::Vector2u windowSize = window.getSize();

    // Center title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(windowSize.x / 2.0f - titleBounds.width / 2.0f, 150);

    // Center menu options
    sf::FloatRect startBounds = startText.getLocalBounds();
    startText.setPosition(windowSize.x / 2.0f - startBounds.width / 2.0f, 350);

    sf::FloatRect exitBounds = exitText.getLocalBounds();
    exitText.setPosition(windowSize.x / 2.0f - exitBounds.width / 2.0f, 420);

    window.draw(titleText);
    window.draw(startText);
    window.draw(exitText);
}

void MenuState::onEnter() {
    selectedOption = 0;
}

void MenuState::handleMouseClick(sf::Vector2f mousePos) {
    if (isMouseOverText(startText, mousePos)) {
        game->changeState(std::make_unique<PlayState>(game));
    }
    else if (isMouseOverText(exitText, mousePos)) {
        game->getWindow().close();
    }
}

bool MenuState::isMouseOverText(const sf::Text& text, sf::Vector2f mousePos) const {
    sf::FloatRect bounds = text.getGlobalBounds();
    return bounds.contains(mousePos);
}

