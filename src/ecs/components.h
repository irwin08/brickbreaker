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


    class PhysicsComponent : public Component {
        public:
            inline static std::uint64_t type_id = 1;

            PhysicsComponent(World *world, std::uint64_t entity_id) {
                this->entity_id = entity_id;
                this->world = world;

            }

            std::tuple<int*, int*> get_coords() {
                int *x = &world->get_entity_by_id(this->entity_id).x;
                int *y = &world->get_entity_by_id(this->entity_id).y;

                return std::tie(x,y);
            }

    };



    class InputComponent : public Component {

    };



}

#endif