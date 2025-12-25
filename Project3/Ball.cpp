#include "Ball.h"
#include <cmath>

Ball::Ball(float x, float y, float radius) {
    shape.setRadius(radius);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(radius, radius);
    
    // 随机初始速度方向
    float angle = (rand() % 60 + 60) * 3.14159f / 180.0f; // 60-120度
    velocity.x = SPEED * std::cos(angle);
    velocity.y = -SPEED * std::sin(angle);
}

void Ball::update(float deltaTime) {
    sf::Vector2f pos = shape.getPosition();
    pos.x += velocity.x * deltaTime;
    pos.y += velocity.y * deltaTime;
    shape.setPosition(pos);
}

void Ball::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void Ball::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

void Ball::setVelocity(float vx, float vy) {
    velocity.x = vx;
    velocity.y = vy;
    // 标准化速度
    float length = std::sqrt(vx * vx + vy * vy);
    if (length > 0) {
        velocity.x = (vx / length) * SPEED;
        velocity.y = (vy / length) * SPEED;
    }
}

