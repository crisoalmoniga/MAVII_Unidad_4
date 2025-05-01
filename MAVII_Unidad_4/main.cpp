#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "ParteCuerpo.h"
#include "Ragdoll.h"
#include <list>
#include <vector>
#include <memory>
#include <cmath>

#define SCALE 100.0f

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "TP - Ragdoll Cannon");
    window.setFramerateLimit(60);

    b2Vec2 gravity(0.f, 9.8f);
    b2World world(gravity);

    // Piso y límites del mundo
    b2BodyDef groundDef;
    groundDef.position.Set(0.f, 0.f);
    b2Body* ground = world.CreateBody(&groundDef);

    b2EdgeShape edge;

    // Piso
    edge.SetTwoSided(b2Vec2(0, 6), b2Vec2(8, 6));
    ground->CreateFixture(&edge, 0.0f);

    // Techo
    edge.SetTwoSided(b2Vec2(0, 0), b2Vec2(8, 0));
    ground->CreateFixture(&edge, 0.0f);

    // Pared izquierda
    edge.SetTwoSided(b2Vec2(0, 0), b2Vec2(0, 6));
    ground->CreateFixture(&edge, 0.0f);

    // Pared derecha
    edge.SetTwoSided(b2Vec2(8, 0), b2Vec2(8, 6));
    ground->CreateFixture(&edge, 0.0f);

    // Lista de ragdolls
    std::list<Ragdoll> ragdolls;

    // Andamio dinámico
    std::vector<std::unique_ptr<ParteCuerpo>> andamio;

    // Cañón
    sf::Vector2f posicionCanon(100.f, 500.f);
    sf::Vector2f mouseWorld = posicionCanon;
    float potenciaActual = 0.f;
    float anguloCanon = 0.f;

    sf::Vertex lineaDisparo[2];
    lineaDisparo[0].color = sf::Color::Red;
    lineaDisparo[1].color = sf::Color::Red;

    // Crear andamio (madera)
    float baseX = 550.f, baseY = 460.f;
    sf::Color madera(139, 69, 19);

    andamio.emplace_back(std::make_unique<ParteCuerpo>(world, baseX - 20, baseY, 10, 140, madera));
    andamio.emplace_back(std::make_unique<ParteCuerpo>(world, baseX + 20, baseY, 10, 140, madera));
    andamio.emplace_back(std::make_unique<ParteCuerpo>(world, baseX, baseY - 85, 80, 10, madera));

    // Obstáculo estático (bloque azul)
    b2BodyDef staticDef;
    staticDef.position.Set(6.5f, 5.0f);
    b2Body* bloqueEstatico = world.CreateBody(&staticDef);

    b2PolygonShape staticShape;
    staticShape.SetAsBox(0.5f, 0.5f);

    b2FixtureDef staticFixture;
    staticFixture.shape = &staticShape;
    bloqueEstatico->CreateFixture(&staticFixture);

    sf::RectangleShape bloqueVisual(sf::Vector2f(100, 100));
    bloqueVisual.setOrigin(50, 50);
    bloqueVisual.setFillColor(sf::Color::Blue);

    // Bucle principal
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

        // Dibujar línea de disparo
        lineaDisparo[0].position = posicionCanon;
        lineaDisparo[1].position = posicionCanon + sf::Vector2f(std::cos(anguloCanon), std::sin(anguloCanon)) * potenciaActual;
        window.draw(lineaDisparo, 2, sf::Lines);

        // Dibujar ragdolls
        for (auto& ragdoll : ragdolls)
            ragdoll.draw(window);

        // Dibujar andamio
        for (auto& pieza : andamio)
            pieza->draw(window);

        // Dibujar obstáculo estático
        bloqueVisual.setPosition(bloqueEstatico->GetPosition().x * SCALE, bloqueEstatico->GetPosition().y * SCALE);
        window.draw(bloqueVisual);

        window.display();
    }

    return 0;
}