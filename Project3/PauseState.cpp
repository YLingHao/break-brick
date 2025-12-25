#include "PauseState.h"
#include "Game.h"
#include "MenuState.h"
#include "PlayState.h"
#include <iostream>

PauseState::PauseState(Game* game) : GameState(game), selectedOption(0), fontLoaded(false),
enterKeyPressed(false), escKeyPressed(false), upKeyPressed(false), downKeyPressed(false) {
    // Try to load font from multiple paths
    // Try to load font from multiple paths
    // NOTE: In Visual Studio, the working directory may not be the .exe folder,
    // so loading "arial.ttf" from current directory often fails.
    if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        fontLoaded = true;
    }
    else if (font.loadFromFile("arial.ttf")) {
        fontLoaded = true;
    }
    else if (font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
        fontLoaded = true;
    }
    else if (font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        fontLoaded = true;
    }

    if (!fontLoaded) {
        std::cerr << "Warning: Failed to load font. Pause menu text may not display correctly." << std::endl;
        std::cerr << "Tried: C:/Windows/Fonts/arial.ttf, arial.ttf, and common Linux/macOS paths." << std::endl;
    }

    if (fontLoaded) {
        pauseText.setFont(font);
        resumeText.setFont(font);
        restartText.setFont(font);
        menuText.setFont(font);
        exitText.setFont(font);
    }

    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(60);
    pauseText.setFillColor(sf::Color::Yellow);
    pauseText.setStyle(sf::Text::Bold);

    // Use ASCII text by default to avoid garbled output when the loaded font
    // doesn't contain Chinese glyphs (e.g. Arial).
    resumeText.setString("Resume (Enter)");
    resumeText.setCharacterSize(35);
    resumeText.setFillColor(sf::Color::White);

    restartText.setString("Restart");
    restartText.setCharacterSize(35);
    restartText.setFillColor(sf::Color::White);

    menuText.setString("Main Menu");
    menuText.setCharacterSize(35);
    menuText.setFillColor(sf::Color::White);

    exitText.setString("Exit");
    exitText.setCharacterSize(35);
    exitText.setFillColor(sf::Color::White);
}

void PauseState::handleInput() {
    // Keyboard input with debouncing
    bool upCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool downCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool enterCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
    bool escCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    // Handle Up key
    if (upCurrentlyPressed && !upKeyPressed) {
        selectedOption = (selectedOption - 1 + MENU_OPTIONS) % MENU_OPTIONS;
        upKeyPressed = true;
    }
    else if (!upCurrentlyPressed) {
        upKeyPressed = false;
    }

    // Handle Down key
    if (downCurrentlyPressed && !downKeyPressed) {
        selectedOption = (selectedOption + 1) % MENU_OPTIONS;
        downKeyPressed = true;
    }
    else if (!downCurrentlyPressed) {
        downKeyPressed = false;
    }

    // Handle Enter key
    if (enterCurrentlyPressed && !enterKeyPressed) {
        if (selectedOption == 0) {
            // Resume game - pop this state
            game->popState();
        }
        else if (selectedOption == 1) {
            // Restart game - clear all states and create new PlayState
            game->replaceAllStates(std::make_unique<PlayState>(game));
            return;
        }
        else if (selectedOption == 2) {
            // Return to main menu (clear pause + play state to avoid stale rendering)
            game->replaceAllStates(std::make_unique<MenuState>(game));
            return;
        }
        else {
            // Exit game
            game->getWindow().close();
            return;
        }
        enterKeyPressed = true;
    }
    else if (!enterCurrentlyPressed) {
        enterKeyPressed = false;
    }

    // Handle Escape key (resume) with debouncing.
    // Note: When PauseState is first pushed, ESC may still be held down.
    if (escCurrentlyPressed && !escKeyPressed) {
        game->popState();
        escKeyPressed = true;
        return;
    }
    else if (!escCurrentlyPressed) {
        escKeyPressed = false;
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

void PauseState::update(float deltaTime) {
    // Update text colors based on selection
    resumeText.setFillColor(selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
    restartText.setFillColor(selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);
    menuText.setFillColor(selectedOption == 2 ? sf::Color::Yellow : sf::Color::White);
    exitText.setFillColor(selectedOption == 3 ? sf::Color::Yellow : sf::Color::White);
}

void PauseState::render() {
    sf::RenderWindow& window = game->getWindow();
    sf::Vector2u windowSize = window.getSize();

    // Draw semi-transparent overlay
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    // Center pause text
    sf::FloatRect pauseBounds = pauseText.getLocalBounds();
    pauseText.setPosition(windowSize.x / 2.0f - pauseBounds.width / 2.0f, 200);

    // Center menu options
    sf::FloatRect resumeBounds = resumeText.getLocalBounds();
    resumeText.setPosition(windowSize.x / 2.0f - resumeBounds.width / 2.0f, 320);

    sf::FloatRect restartBounds = restartText.getLocalBounds();
    restartText.setPosition(windowSize.x / 2.0f - restartBounds.width / 2.0f, 380);

    sf::FloatRect menuBounds = menuText.getLocalBounds();
    menuText.setPosition(windowSize.x / 2.0f - menuBounds.width / 2.0f, 440);

    sf::FloatRect exitBounds = exitText.getLocalBounds();
    exitText.setPosition(windowSize.x / 2.0f - exitBounds.width / 2.0f, 500);

    window.draw(pauseText);
    window.draw(resumeText);
    window.draw(restartText);
    window.draw(menuText);
    window.draw(exitText);
}

void PauseState::onEnter() {
    selectedOption = 0;
    enterKeyPressed = false;

    // If ESC is currently down (the key that opened the pause menu), mark it as pressed
    // so we don't instantly treat it as "resume".
    escKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    upKeyPressed = false;
    downKeyPressed = false;
}

void PauseState::handleMouseClick(sf::Vector2f mousePos) {
    if (isMouseOverText(resumeText, mousePos)) {
        game->popState();
    }
    else if (isMouseOverText(restartText, mousePos)) {
        // Restart game - clear all states and create new PlayState
        game->replaceAllStates(std::make_unique<PlayState>(game));
    }
    else if (isMouseOverText(menuText, mousePos)) {
        // Return to main menu (clear pause + play state)
        game->replaceAllStates(std::make_unique<MenuState>(game));
    }
    else if (isMouseOverText(exitText, mousePos)) {
        game->getWindow().close();
    }
}

bool PauseState::isMouseOverText(const sf::Text& text, sf::Vector2f mousePos) const {
    sf::FloatRect bounds = text.getGlobalBounds();
    return bounds.contains(mousePos);
}

