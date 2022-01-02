#ifndef ECS_H
#define ECS_H


#include <memory>
#include <stack>
#include <any>
#include <typeindex>
#include <SFML/Graphics.hpp>
#include <exception>
#include <ranges>


namespace ecs {


    struct GameObject;
    class World;

    struct Component {
        inline static std::uint64_t id;
        std::uint64_t entity_id;
        World *world;

    };


    struct ComponentHandle {
        std::uint64_t type_id;
        std::uint64_t id;
    };

    struct GameObject {
            int x, y;
            int velocity_x=0;
            int velocity_y=0;
            std::uint64_t id;

            std::vector<ComponentHandle> components;
    };


    class System {
        public:
        virtual void tick(World& world, float deltaTime) = 0;
    };





    class ComponentManager {
        public:
            template<typename T, typename... Args>
            ComponentHandle generate_component(Args&&... args) {
                T t{std::forward<Args>(args)...};
                std::uint64_t id;
                if(map.find(T::type_id) == map.end()) {
                    type_map.emplace(T::type_id, typeid(T));
                    id = get_new_id();
                    t.id = id;
                    map.emplace(T::type_id, std::vector<std::any>{std::move(t)});
                    
                }
                else {
                    id = get_new_id();
                    t.id = id;
                    map.at(T::type_id).push_back(t);
                }
                return ComponentHandle{T::type_id, id};
            }

            //NOTE: this is a reference to an element of a vector, hence it should be thought of as "one-and-done". Always access by id.
            template<typename T> 
            T& access_component(std::uint64_t id) {
                for(auto& elem : map.at(T::type_id)) {
                    if(std::any_cast<T&>(elem).id == id)
                        return std::any_cast<T&>(elem);
                }
                return std::any_cast<T&>(map.at(T::type_id).at(id));
            }


            std::map<std::uint64_t, std::type_index> type_map;
        private:
            
            std::map<std::uint64_t, std::vector<std::any>> map;
            std::stack<std::uint64_t> available_ids;
            std::uint64_t next_unused = 0;

            std::uint64_t get_new_id() {
                if(available_ids.size() == 0) {
                    return next_unused++;
                }
                else {
                    auto next = available_ids.top();
                    available_ids.pop();
                    return next;
                }
            }


    };




    class World {
        public:
           /* //need to make a version that affects entities with intersection of components
            template <typename T, typename F>
            void each(F func) {
                for(auto& object : objects) {
                    for(auto component : object.components) {
                        if(component_manager.type_map.at(component.type_id) == typeid(T))
                            func(component_manager.access_component<T>(component.id));
                    }
                }
            }*/

            template <typename... Ts, typename F>
            void each(F func) {
                for(auto& object : objects) {
                    auto range = (object.components | std::views::filter([](auto component){ 
                        std::vector<std::uint64_t> vec = {(Ts::id)...};
                        return (std::find(vec.begin(), vec.end(), component.id) != vec.end());
                    }));

                    if(std::distance(range.begin(), range.end()) == sizeof...(Ts)) {
                        std::vector<std::tuple<std::uint64_t, std::uint64_t>> data;
                        for(auto elem : range) {
                            auto tup = std::make_tuple(elem.type_id, elem.id);
                            
                            data.push_back(tup);
                        }

                        func(data);

                    }
                }
            }

            //return id of entity
            std::uint64_t addEntity() {
                GameObject object;
                object.id = get_new_id();
                objects.emplace_back(object);
                return object.id;
            }

            template<typename T, typename... Args>
            void add_component_to_id(World *world, std::uint64_t entity_id, Args&&... args) {
                auto handle = component_manager.generate_component<T>(world, entity_id, std::forward<Args>(args)...);
                for(auto& object : objects) {
                    if(object.id == entity_id) {
                        object.components.emplace_back(handle);
                    }
                }
            }
            

            void registerSystem(std::unique_ptr<System> s) {
                systems.emplace_back(std::move(s));
            }

            void tick(float deltaTime) {
                for(auto& system : systems) {
                    system->tick(*this, deltaTime);
                }
            }

            GameObject& get_entity_by_id(std::uint64_t entity_id) {
                for(auto& object : objects) {
                    if(object.id == entity_id) {
                        return object;
                    }
                }
                //reached end without finding object, given bad id
                throw std::runtime_error("Bad entity id, does not exist");
            }

            ComponentManager component_manager;
        private:
            std::vector<GameObject> objects;
            std::vector<std::unique_ptr<System>> systems;
            

            std::stack<std::uint64_t> available_object_ids;
            std::uint64_t next_unused = 0;

            std::uint64_t get_new_id() {
                if(available_object_ids.size() == 0) {
                    return next_unused++;
                }
                else {
                    auto next = available_object_ids.top();
                    available_object_ids.pop();
                    return next;
                }
            }
    };


}


#endif