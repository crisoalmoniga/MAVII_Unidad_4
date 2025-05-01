#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "ParteCuerpo.h"

#define SCALE 100.0f

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ragdoll - Brazos con límite");
    window.setFramerateLimit(60);

    b2Vec2 gravity(0.f, 9.8f);
    b2World world(gravity);

    // Pisoo
    b2BodyDef groundDef;
    groundDef.position.Set(0.f, 0.f);
    b2Body* ground = world.CreateBody(&groundDef);

    b2EdgeShape edge;
    edge.SetTwoSided(b2Vec2(0, 6), b2Vec2(8, 6));
    b2FixtureDef fixture;
    fixture.shape = &edge;
    ground->CreateFixture(&fixture);

    // Cuerpos
    ParteCuerpo cabeza(world, 400, 200, 80, 80);
    ParteCuerpo torso(world, 400, 300, 100, 100);
    ParteCuerpo brazoIzq(world, 330, 300, 60, 20);
    ParteCuerpo brazoDer(world, 470, 300, 60, 20);
    ParteCuerpo piernaIzq(world, 380, 420, 25, 90);
    ParteCuerpo piernaDer(world, 420, 420, 25, 90);

    b2Vec2 centroTorso = torso.getBody()->GetWorldCenter();

    // Función general para joints sin límite
    auto unirRevoluteLibre = [&](b2Body* a, b2Body* b, b2Vec2 puntoMundo) {
        b2RevoluteJointDef jointDef;
        jointDef.bodyA = a;
        jointDef.bodyB = b;
        jointDef.localAnchorA = a->GetLocalPoint(puntoMundo);
        jointDef.localAnchorB = b->GetLocalPoint(puntoMundo);
        jointDef.collideConnected = false;
        jointDef.enableLimit = false;
        world.CreateJoint(&jointDef);
        };

    // Función para brazos con límite
    auto unirBrazoConLimite = [&](b2Body* a, b2Body* b, b2Vec2 puntoMundo) {
        b2RevoluteJointDef jointDef;
        jointDef.bodyA = a;
        jointDef.bodyB = b;
        jointDef.localAnchorA = a->GetLocalPoint(puntoMundo);
        jointDef.localAnchorB = b->GetLocalPoint(puntoMundo);
        jointDef.collideConnected = false;
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.5f;  // -30 grados
        jointDef.upperAngle = 0.5f;  // 30 grados
        world.CreateJoint(&jointDef);
        };

    // Uniones
    unirRevoluteLibre(cabeza.getBody(), torso.getBody(), centroTorso + b2Vec2(0, -0.6f));
    unirBrazoConLimite(brazoIzq.getBody(), torso.getBody(), centroTorso + b2Vec2(-0.6f, 0));
    unirBrazoConLimite(brazoDer.getBody(), torso.getBody(), centroTorso + b2Vec2(0.6f, 0));
    unirRevoluteLibre(piernaIzq.getBody(), torso.getBody(), centroTorso + b2Vec2(-0.2f, 0.7f));
    unirRevoluteLibre(piernaDer.getBody(), torso.getBody(), centroTorso + b2Vec2(0.2f, 0.7f));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        world.Step(1.f / 60.f, 8, 3);

        window.clear();

        cabeza.draw(window);
        torso.draw(window);
        brazoIzq.draw(window);
        brazoDer.draw(window);
        piernaIzq.draw(window);
        piernaDer.draw(window);

        window.display();
    }

    return 0;
}