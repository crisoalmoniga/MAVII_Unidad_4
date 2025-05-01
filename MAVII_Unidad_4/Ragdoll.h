#pragma once
#include "ParteCuerpo.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Ragdoll {
public:
    Ragdoll(b2World& world, sf::Vector2f origen, float anguloCanon, float potencia);
    void draw(sf::RenderWindow& window);

private:
    std::unique_ptr<ParteCuerpo> cabeza;
    std::unique_ptr<ParteCuerpo> torso;
    std::unique_ptr<ParteCuerpo> brazoIzq;
    std::unique_ptr<ParteCuerpo> brazoDer;
    std::unique_ptr<ParteCuerpo> piernaIzq;
    std::unique_ptr<ParteCuerpo> piernaDer;
};
