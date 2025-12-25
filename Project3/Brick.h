#ifndef BRICK_H
#define BRICK_H

#include <SFML/Graphics.hpp>

class Brick {
public:
    Brick(float x, float y, float width, float height, sf::Color color);
    
    void draw(sf::RenderWindow& window) const;
    
    bool isDestroyed() const { return destroyed; }
    void destroy() { destroyed = true; }
    
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    sf::Color getColor() const { return shape.getFillColor(); }

private:
    sf::RectangleShape shape;
    bool destroyed;
};

#endif // BRICK_H

