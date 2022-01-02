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
            world.each<GraphicsComponent>([&](std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components){ GraphicsComponent pComp = world.component_manager.access_component<GraphicsComponent>(std::get<1>(components[0])); window->draw(pComp.generate_sprite()); });
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
            world.each<PhysicsComponent>([&](std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components){PhysicsComponent pComp = world.component_manager.access_component<PhysicsComponent>(std::get<1>(components[0])); auto coords = pComp.get_coords(); if((*std::get<1>(coords)) <= 300) {(*std::get<1>(coords)) -= (gravity*deltaTime);} });
        }

    private:
        float gravity;
    };

    class InputSystem : public System {

        void tick(World& world, float deltaTime) override {
            //need to make each function that grabs all entities with both a physics component & and input component
            world.each<PhysicsComponent, GraphicsComponent>([&](std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components){
                PhysicsComponent physics = PhysicsComponent::find_physics(world, components);
                GraphicsComponent graphics = GraphicsComponent::find_graphics(world, components);

                
            });
        }
    };
}

#endif