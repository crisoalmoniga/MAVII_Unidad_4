#include "Ragdoll.h"
#include <cmath>

#define SCALE 100.0f

Ragdoll::Ragdoll(b2World& world, sf::Vector2f origen, float anguloCanon, float potencia) {

    cabeza = std::make_unique<ParteCuerpo>(world, origen.x, origen.y - 20, 10, 10);
    torso = std::make_unique<ParteCuerpo>(world, origen.x, origen.y, 16.5f, 16.5f);
    brazoIzq = std::make_unique<ParteCuerpo>(world, origen.x - 11, origen.y, 10, 3.5f);
    brazoDer = std::make_unique<ParteCuerpo>(world, origen.x + 11, origen.y, 10, 3.5f);
    piernaIzq = std::make_unique<ParteCuerpo>(world, origen.x - 3.5f, origen.y + 20, 4, 15);
    piernaDer = std::make_unique<ParteCuerpo>(world, origen.x + 3.5f, origen.y + 20, 4, 15);

    b2Vec2 centroTorso = torso->getBody()->GetWorldCenter();

    auto unir = [&](b2Body* a, b2Body* b, b2Vec2 puntoMundo, bool limitado = false) {
        b2RevoluteJointDef jointDef;
        jointDef.bodyA = a;
        jointDef.bodyB = b;
        jointDef.localAnchorA = a->GetLocalPoint(puntoMundo);
        jointDef.localAnchorB = b->GetLocalPoint(puntoMundo);
        jointDef.collideConnected = false;
        jointDef.enableLimit = limitado;
        if (limitado) {
            jointDef.lowerAngle = -0.5f;
            jointDef.upperAngle = 0.5f;
        }
        world.CreateJoint(&jointDef);
        };

    unir(cabeza->getBody(), torso->getBody(), centroTorso + b2Vec2(0, -0.1f));
    unir(brazoIzq->getBody(), torso->getBody(), centroTorso + b2Vec2(-0.1f, 0), true);
    unir(brazoDer->getBody(), torso->getBody(), centroTorso + b2Vec2(0.1f, 0), true);
    unir(piernaIzq->getBody(), torso->getBody(), centroTorso + b2Vec2(-0.035f, 0.125f));
    unir(piernaDer->getBody(), torso->getBody(), centroTorso + b2Vec2(0.035f, 0.125f));

    float impulseFactor = potencia * 0.02f;
    b2Vec2 impulso = impulseFactor * b2Vec2(std::cos(anguloCanon), std::sin(anguloCanon));
    torso->getBody()->ApplyLinearImpulseToCenter(impulso, true);
}

void Ragdoll::draw(sf::RenderWindow& window) {
    cabeza->draw(window);
    torso->draw(window);
    brazoIzq->draw(window);
    brazoDer->draw(window);
    piernaIzq->draw(window);
    piernaDer->draw(window);
}