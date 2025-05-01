#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class ParteCuerpo {
public:
    ParteCuerpo(b2World& world, float x, float y, float width, float height);

    void draw(sf::RenderWindow& window);
    b2Body* getBody();
    sf::Vector2f getPosition() const;

private:
    b2Body* body;
    sf::RectangleShape rect;
};