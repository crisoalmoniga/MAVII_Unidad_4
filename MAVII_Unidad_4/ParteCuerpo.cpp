#include "ParteCuerpo.h"

#define SCALE 100.0f

ParteCuerpo::ParteCuerpo(b2World& world, float x, float y, float width, float height)
    : ParteCuerpo(world, x, y, width, height, sf::Color::Yellow) {}

ParteCuerpo::ParteCuerpo(b2World& world, float x, float y, float width, float height, sf::Color color) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / SCALE, y / SCALE);
    body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox((width / 2) / SCALE, (height / 2) / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 5.0f;
    fixtureDef.friction = 0.6f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = 0x0001;
    fixtureDef.filter.maskBits = 0xFFFF;
    body->CreateFixture(&fixtureDef);

    rect.setSize(sf::Vector2f(width, height));
    rect.setOrigin(width / 2, height / 2);
    rect.setFillColor(color);
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