#include "Paddle.h"

Paddle::Paddle(float x, float y, float width, float height) 
    : speed(DEFAULT_SPEED), windowWidth(0) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color(100, 150, 255));
    shape.setOrigin(width / 2.0f, height / 2.0f);
}

void Paddle::update(float deltaTime) {
    handleInput();
}

void Paddle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void Paddle::handleInput() {
    float moveDistance = speed * (1.0f / 60.0f); // 假设60fps
    sf::Vector2f pos = shape.getPosition();
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        pos.x -= moveDistance;
        if (pos.x < shape.getSize().x / 2.0f) {
            pos.x = shape.getSize().x / 2.0f;
        }
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        pos.x += moveDistance;
        if (windowWidth > 0 && pos.x > windowWidth - shape.getSize().x / 2.0f) {
            pos.x = windowWidth - shape.getSize().x / 2.0f;
        }
    }
    
    shape.setPosition(pos);
}

void Paddle::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

void Paddle::setWindowSize(unsigned int width, unsigned int height) {
    windowWidth = width;
}

