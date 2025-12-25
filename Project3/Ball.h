#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(float x, float y, float radius);
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    float getRadius() const { return shape.getRadius(); }
    sf::Vector2f getVelocity() const { return velocity; }
    
    void setPosition(float x, float y);
    void setVelocity(float vx, float vy);
    void reverseVelocityX() { velocity.x = -velocity.x; }
    void reverseVelocityY() { velocity.y = -velocity.y; }
    
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    static constexpr float SPEED = 400.0f;
};

#endif // BALL_H

