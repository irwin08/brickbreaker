#include <iostream>
#include <vector>
#include <map>
#include <any>
#include <cstdint>
#include <stack>

struct Component {
    std::uint64_t id;
};

struct ParticularComponent : public Component {
    inline static const std::uint64_t type_id = 1;
    int x;

    ParticularComponent(int x) {
        this->x = x;
        std::cout << x << std::endl;
    }
};

struct ComponentHandle {
    std::uint64_t type_id;
    std::uint64_t id;
};

class ComponentManager {
    public:
        template<typename T, typename... Args>
        ComponentHandle generate_component(Args&&... args) {
            T t{{std::forward<Args>(args)...}};
            std::uint64_t id;
            if(map.find(T::type_id) == map.end()) {
                
                map.emplace(T::type_id, std::vector<std::any>{t});
                id = get_new_id();
            }
            else {
                id = get_new_id();
                map.at(T::type_id).push_back(t);
            }
            t.id = id;
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

    private:
        //type, id
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




int main() {

    ComponentManager manager;

    std::vector<ComponentHandle> handles;

    auto handle = manager.generate_component<ParticularComponent>(4);
    handles.push_back(handle);

    manager.access_component<ParticularComponent>(handles[0].id).x = 1;
    std::cout << manager.access_component<ParticularComponent>(handles[0].id).x << std::endl;

    std::cout << "All done!" << std::endl;

    return 0;
}