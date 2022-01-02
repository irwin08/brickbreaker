#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include "ecs/components.h"
#include "ecs/systems.h"




int main()
{
    sf::RenderWindow window{sf::VideoMode(800, 600), "My Window"};

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60 );

    ecs::World world;

    ecs::DrawingSystem drawing(&window);
    ecs::GravitySystem physics(-2);

    world.registerSystem(std::make_unique<ecs::DrawingSystem>(drawing));
    world.registerSystem(std::make_unique<ecs::GravitySystem>(physics));

    std::uint64_t boris_id = world.addEntity();
    world.add_component_to_id<ecs::GraphicsComponent>(&world, boris_id, "textures/cobrameeting.PNG", 5,5,200,200);
    world.add_component_to_id<ecs::PhysicsComponent>(&world, boris_id);

    while(window.isOpen()) {

        sf::Event event;
        while(window.pollEvent(event)) {

            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                default:
                    break;
            }
        }

        //window.clear(sf::Color::White);

        world.tick(1);

        window.display();
    }

    return 0;
}