#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "ParteCuerpo.h"
#include "Ragdoll.h"
#include <list>
#include <cmath>

#define SCALE 100.0f

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "TP - Ragdoll Cannon");
    window.setFramerateLimit(60);

    b2Vec2 gravity(0.f, 9.8f);
    b2World world(gravity);

    // Piso
    b2BodyDef groundDef;
    groundDef.position.Set(0.f, 0.f);
    b2Body* ground = world.CreateBody(&groundDef);

    b2EdgeShape edge;
    edge.SetTwoSided(b2Vec2(0, 6), b2Vec2(8, 6));
    b2FixtureDef fixture;
    fixture.shape = &edge;
    ground->CreateFixture(&fixture);

    // Lista de ragdolls
    std::list<Ragdoll> ragdolls;

    sf::Vector2f posicionCanon(100.f, 500.f);
    sf::Vector2f mouseWorld = posicionCanon;
    float potenciaActual = 0.f;
    float anguloCanon = 0.f;

    sf::Vertex lineaDisparo[2];
    lineaDisparo[0].color = sf::Color::Red;
    lineaDisparo[1].color = sf::Color::Red;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                mouseWorld = window.mapPixelToCoords(mousePos);
                sf::Vector2f dir = mouseWorld - posicionCanon;
                potenciaActual = std::min(std::sqrt(dir.x * dir.x + dir.y * dir.y), 150.f);
                anguloCanon = std::atan2(dir.y, dir.x);
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                ragdolls.emplace_back(world, posicionCanon, anguloCanon, potenciaActual);
            }
        }

        world.Step(1.f / 60.f, 8, 3);
        window.clear();

        lineaDisparo[0].position = posicionCanon;
        lineaDisparo[1].position = posicionCanon + sf::Vector2f(std::cos(anguloCanon), std::sin(anguloCanon)) * potenciaActual;
        window.draw(lineaDisparo, 2, sf::Lines);

        for (auto& ragdoll : ragdolls)
            ragdoll.draw(window);

        window.display();
    }

    return 0;
}