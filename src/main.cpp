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
    ecs::InputSystem input;
    ecs::MovementSystem movement;

    world.registerSystem(std::make_unique<ecs::DrawingSystem>(drawing));
    world.registerSystem(std::make_unique<ecs::GravitySystem>(physics));
    world.registerSystem(std::make_unique<ecs::InputSystem>(input));
    world.registerSystem(std::make_unique<ecs::MovementSystem>(movement));


    std::uint64_t boris_id = world.addEntity();
    world.add_component_to_id<ecs::GraphicsComponent>(&world, boris_id, "textures/cobrameeting.PNG", 5,5,200,200);
    world.add_component_to_id<ecs::PhysicsComponent>(&world, boris_id);
    world.add_component_to_id<ecs::InputComponent>(&world, boris_id);

    sf::Clock clock;

    while(window.isOpen()) {

        sf::Time time_1 = clock.restart();

        sf::Event event;
        while(window.pollEvent(event)) {

            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    world.each<ecs::InputComponent>([&](std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components){
                        ecs::InputComponent& input = world.component_manager.access_component<ecs::InputComponent>(std::get<1>(components[0]));
                        input.keys_pressed.emplace(event.key);
                        //ISSUE HERE
                        std::cout << "emplaced" << std::endl;
                    });
                    break;

                default:
                    break;
            }
        }

        //window.clear(sf::Color::White);

        sf::Time time_2 = clock.getElapsedTime();
        
        auto elapsed = (time_2.asMilliseconds() - time_1.asMilliseconds());
        if(elapsed < 16)
            sf::sleep(sf::Time(sf::milliseconds(16-elapsed)));

        world.tick(1);

        window.display();
    }

    return 0;
}