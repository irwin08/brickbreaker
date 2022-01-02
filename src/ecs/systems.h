#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ecs.h"
#include "components.h"

namespace ecs {

    
    class DrawingSystem : public System {

        public:
        
        DrawingSystem(sf::RenderWindow *window) {
            this->window = window;
        }

        void tick(World& world, float deltaTime) override {
            window->clear(sf::Color::Green);
            world.each<GraphicsComponent>([&](GraphicsComponent& component){ window->draw(component.generate_sprite()); });
        }

        private:
            sf::RenderWindow *window;
    };

    class GravitySystem : public System {
    public:
        GravitySystem(float g_force) {
            gravity = g_force;
        }

        void tick(World& world, float deltaTime) override {
            world.each<PhysicsComponent>([&](PhysicsComponent& component){auto coords = component.get_coords(); if((*std::get<1>(coords)) <= 300) {(*std::get<1>(coords)) -= (gravity*deltaTime);} });
        }

    private:
        float gravity;
    };
}

#endif