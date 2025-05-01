#include "ParteCuerpo.h"
#include <SFML/Graphics.hpp>

#define SCALE 100.0f

ParteCuerpo::ParteCuerpo(b2World& world, float x, float y, float width, float height) {
    // Definir el cuerpo físico
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / SCALE, y / SCALE);
    body = world.CreateBody(&bodyDef);

    // Definir forma
    b2PolygonShape shape;
    shape.SetAsBox((width / 2) / SCALE, (height / 2) / SCALE);

    // Fixture rígido
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 5.0f;       // Alta densidad = más masa
    fixtureDef.friction = 0.6f;      // Fricción más fuerte
    fixtureDef.restitution = 0.0f;   // Sin rebote
    fixtureDef.filter.categoryBits = 0x0001;
    fixtureDef.filter.maskBits = 0xFFFF;
    body->CreateFixture(&fixtureDef);

    // Gráfico SFML
    rect.setSize(sf::Vector2f(width, height));
    rect.setOrigin(width / 2, height / 2);
    rect.setFillColor(sf::Color::Yellow);
}

void ParteCuerpo::draw(sf::RenderWindow& window) {
    b2Vec2 pos = body->GetPosition();
    float angle = body->GetAngle() * 180 / b2_pi;
    rect.setPosition(pos.x * SCALE, pos.y * SCALE);
    rect.setRotation(angle);
    window.draw(rect);
}

b2Body* ParteCuerpo::getBody() {
    return body;
}

sf::Vector2f ParteCuerpo::getPosition() const {
    b2Vec2 pos = body->GetPosition();
    return sf::Vector2f(pos.x * SCALE, pos.y * SCALE);
}
