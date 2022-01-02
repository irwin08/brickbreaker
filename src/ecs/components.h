#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "ecs.h"
#include <tuple>


namespace ecs {


    class GraphicsComponent : public Component {
        public:
            inline static std::uint64_t type_id = 0;


            GraphicsComponent(World *world, std::uint64_t entity_id, std::string texture_file, int x, int y, int w, int h) {
                this->entity_id = entity_id;
                this->world = world;
                this->world->get_entity_by_id(entity_id).x = x;
                this->world->get_entity_by_id(entity_id).y = y;
                

                texture.loadFromFile(texture_file);
                //this->sprite = sf::Sprite(this->texture);
                this->texture_rect = sf::IntRect(x,y,w,h);
            }

            GraphicsComponent(const GraphicsComponent& copy) {
                this->entity_id = copy.entity_id;
                this->world = copy.world;
                this->texture = copy.texture;
                this->texture_rect = copy.texture_rect;
            }

            ~GraphicsComponent() {}

            GraphicsComponent& operator=(const GraphicsComponent& copy) {
                if(this == &copy) return *this;
                this->entity_id = copy.entity_id;
                this->world = copy.world;
                this->texture = copy.texture;
                this->texture_rect = copy.texture_rect;
                return *this;
            }

            static GraphicsComponent find_graphics(World& world, std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components) {
                for(auto tup : components) {
                    if(std::get<0>(tup) == GraphicsComponent::type_id)
                        return world.component_manager.access_component<GraphicsComponent>(std::get<1>(tup));
                }
            }



            sf::Sprite generate_sprite() {
                sf::Sprite sprite{};
                //return sf::Sprite(this->texture);
                sprite.setPosition(this->world->get_entity_by_id(entity_id).x,this->world->get_entity_by_id(entity_id).y);
                sprite.setTexture(this->texture);
                sprite.setTextureRect(texture_rect);
                return sprite;
            }

            //TODO rewrite so sprite and texture are unique pointers. Need to be on heap, because component is moved around in memory
            sf::Texture texture;
            //sf::Sprite sprite;
            sf::IntRect texture_rect;
    };

    //no move semantics might implement later
    class PhysicsComponent : public Component {
        public:
            inline static std::uint64_t type_id = 1;

            PhysicsComponent(World *world, std::uint64_t entity_id) {
                this->entity_id = entity_id;
                this->world = world;

            }

            PhysicsComponent(const PhysicsComponent& copy) {
                this->entity_id = copy.entity_id;
                this->world = copy.world;
            }

            ~PhysicsComponent() {}

            PhysicsComponent& operator=(const PhysicsComponent& copy) {
                if(this == &copy) return *this;
                this->entity_id = entity_id;
                this->world = world;
                return *this;
            }

            std::tuple<int*, int*> get_coords() {
                int *x = &world->get_entity_by_id(this->entity_id).x;
                int *y = &world->get_entity_by_id(this->entity_id).y;

                return std::tie(x,y);
            }

            static PhysicsComponent find_physics(World& world, std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components) {
                for(auto tup : components) {
                    if(std::get<0>(tup) == PhysicsComponent::type_id)
                        return world.component_manager.access_component<PhysicsComponent>(std::get<1>(tup));
                }
            }

    };


    //no move semantics, might implement later
    class InputComponent : public Component {
    public:
        inline static std::uint64_t type_id = 2;


        InputComponent(World *world, std::uint64_t entity_id) {
            this->entity_id = entity_id;
            this->world = world;
        }

        InputComponent(const InputComponent& copy) {
            this->entity_id = copy.entity_id;
            this->world = copy.world;
        }

        ~InputComponent() {}

        InputComponent& operator=(const InputComponent& copy) {
            if(this == &copy) return *this;
            this->world = copy.world;
            this->entity_id = copy.entity_id;
            return *this;
        }

        static InputComponent find_input(World& world, std::vector<std::tuple<std::uint64_t, std::uint64_t>>& components) {
                for(auto tup : components) {
                    if(std::get<0>(tup) == InputComponent::type_id)
                        return world.component_manager.access_component<InputComponent>(std::get<1>(tup));
                }
        }

        
    };



}

#endif