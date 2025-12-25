#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

class Paddle {
public:
    Paddle(float x, float y, float width, float height);
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    void handleInput();
    
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    sf::Vector2f getSize() const { return shape.getSize(); }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    
    void setPosition(float x, float y);
    void setWindowSize(unsigned int width, unsigned int height);

private:
    sf::RectangleShape shape;
    float speed;
    unsigned int windowWidth;
    static constexpr float DEFAULT_SPEED = 500.0f;
};

#endif // PADDLE_H

