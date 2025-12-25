#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(1000, 700), "打砖块游戏 - Breakout", sf::Style::Close) {
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
}

Game::~Game() {
    while (!states.empty()) {
        states.pop();
    }
}

void Game::run() {
    gameLoop();
}

void Game::gameLoop() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Clamp deltaTime to avoid huge updates after stutters/breakpoints
        if (deltaTime > 0.1f) {
            deltaTime = FRAME_TIME;
        }

        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    // Handle input once per frame to avoid duplicate triggers caused by multiple events
    // in the same frame (States use sf::Keyboard::isKeyPressed, i.e. real-time key state).
    if (!states.empty()) {
        states.top()->handleInput();
    }
}

void Game::update(float deltaTime) {
    if (!states.empty()) {
        states.top()->update(deltaTime);
    }
}

void Game::render() {
    window.clear(sf::Color(30, 30, 50));

    // Render all states in the stack (for pause overlay)
    // We need to render from bottom to top so pause menu appears on top
    if (!states.empty()) {
        // Create a vector to store state pointers
        std::vector<GameState*> renderOrder;

        // Collect all state pointers into a vector
        // We need to be careful not to modify the stack during iteration
        std::stack<std::unique_ptr<GameState>> tempStack;

        // Move all states to temp stack and collect pointers
        while (!states.empty()) {
            renderOrder.push_back(states.top().get());
            tempStack.push(std::move(states.top()));
            states.pop();
        }

        // Restore the stack
        while (!tempStack.empty()) {
            states.push(std::move(tempStack.top()));
            tempStack.pop();
        }

        // Render from bottom to top (reverse of collection order)
        for (auto it = renderOrder.rbegin(); it != renderOrder.rend(); ++it) {
            if (*it) {  // Safety check
                (*it)->render();
            }
        }
    }

    window.display();
}

void Game::pushState(std::unique_ptr<GameState> state) {
    if (state) {
        states.push(std::move(state));
        states.top()->onEnter();
    }
}

void Game::popState() {
    if (!states.empty()) {
        states.top()->onExit();
        states.pop();
    }
}

void Game::changeState(std::unique_ptr<GameState> state) {
    if (!states.empty()) {
        states.top()->onExit();
        states.pop();
    }

    if (state) {
        states.push(std::move(state));
        states.top()->onEnter();
    }
}

void Game::replaceAllStates(std::unique_ptr<GameState> state) {
    // Clear all existing states
    while (!states.empty()) {
        states.top()->onExit();
        states.pop();
    }

    // Push the new state
    if (state) {
        states.push(std::move(state));
        states.top()->onEnter();
    }
}

GameState* Game::peekState() {
    if (states.empty()) {
        return nullptr;
    }
    return states.top().get();
}

