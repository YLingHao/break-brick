#include "Brick.h"

Brick::Brick(float x, float y, float width, float height, sf::Color color) 
    : destroyed(false) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.0f);
}

void Brick::draw(sf::RenderWindow& window) const {
    if (!destroyed) {
        window.draw(shape);
    }
}

