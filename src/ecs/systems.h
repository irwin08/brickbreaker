#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ecs.h"

namespace ecs {

    
    class DrawingSystem : public System {

        public:
        
        DrawingSystem(sf::RenderWindow *window) {
            this->window = window;
        }

        void tick(World& world, float deltaTime) override {
            window->clear(sf::Color::Green);
            world.each<GraphicsComponent>([&](GraphicsComponent& component){  sf::Sprite sprite = sf::Sprite(component.texture); window->draw(component.generate_sprite()); });
        }

        private:
            sf::RenderWindow *window;
    };
}

#endif