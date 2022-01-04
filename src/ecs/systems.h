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
            world.each<GraphicsComponent>([&](std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components){ GraphicsComponent& pComp = world.component_manager.access_component<GraphicsComponent>(std::get<1>(components[0])); window->draw(pComp.generate_sprite()); });
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
            world.each<PhysicsComponent>([&](std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components){
                PhysicsComponent& pComp = world.component_manager.access_component<PhysicsComponent>(std::get<1>(components[0]));
                auto coords = pComp.get_coords(); 

                if((*std::get<1>(coords) <= 300) && (*std::get<1>(coords) > 0)) {
                    (*std::get<1>(coords)) -= (gravity*deltaTime);
                } 
            });
        }

    private:
        float gravity;
    };

    class MovementSystem : public System {
    public:
        void tick(World& world, float deltaTime) override {
            world.each<PhysicsComponent>([&](std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components){
                PhysicsComponent& pComp = world.component_manager.access_component<PhysicsComponent>(std::get<1>(components[0])); 
                auto coords = pComp.get_coords(); 
                auto vel_x = pComp.get_velocity_x();

                *std::get<0>(coords) += (*vel_x * deltaTime);

                //slow with friction
                if(*vel_x > 0)
                    *vel_x -= 0.5;
                else if(*vel_x < 0)
                    *vel_x += 0.5;

            });
        }

    };

    class InputSystem : public System {

        void tick(World& world, float deltaTime) override {
            //need to make each function that grabs all entities with both a physics component & and input component
            
            world.each<PhysicsComponent, GraphicsComponent, InputComponent>([&](std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components){
                PhysicsComponent& physics = PhysicsComponent::find_physics(world, components);
                GraphicsComponent& graphics = GraphicsComponent::find_graphics(world, components);
                InputComponent input = InputComponent::find_input(world, components); 
                

                

                while(!input.keys_pressed.empty()) {
                    auto next = input.keys_pressed.front();

                    switch(next.code) {
                        case sf::Keyboard::A: {
                            int *vel_x = physics.get_velocity_x();
                            *vel_x -= 5;
                            if(*vel_x < -10)
                                *vel_x = -10;
                            break;
                        }
                        case sf::Keyboard::D: {
                            int *vel_x = physics.get_velocity_x();
                            *vel_x += 5;
                            if(*vel_x > 10)
                                *vel_x = 10;
                            break;
                        }
                    }

                    input.keys_pressed.pop();
                }
            });

            //clear queue of events processed this tick
            world.each<InputComponent>([&](std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components) {
                InputComponent& input = InputComponent::find_input(world, components);
                std::queue<sf::Event::KeyEvent>().swap(input.keys_pressed);
            });
        }
    };
}

#endif