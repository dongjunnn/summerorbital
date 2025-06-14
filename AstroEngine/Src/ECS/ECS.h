// #pragma once

// #include <iostream>
// #include <vector>
// #include <memory>
// #include <algorithm>
// #include <bitset>
// #include <array>

// class Component;
// class Entity;
// class Manager;

// using ComponentID = std::size_t;
// using Group = std::size_t;

// inline ComponentID getNewComponentTypeID()
// {
//     static ComponentID lastID = 0u;
//     return lastID++;
// }

// template <typename T> inline ComponentID getComponentTypeID() noexcept
// {
//     static ComponentID typeID = getNewComponentTypeID();
//     return typeID;
// }

// constexpr std::size_t maxComponents = 32;
// constexpr std::size_t maxGroups = 32;

// using ComponentBitSet = std::bitset<maxComponents>;
// using ComponentArray = std::array<Component*, maxComponents>;
// using GroupBitSet = std::bitset<maxGroups>;

// class Component
// {
// public:
//     Entity* entity;

//     virtual void init() {}
//     virtual void update() {}
//     virtual void draw() {}

//     virtual ~Component() {}
// };

// class Entity
// {
// private:
//     Manager& manager;
//     bool active = true;
//     std::vector<std::unique_ptr<Component>> components;

//     ComponentArray componentArray;
//     ComponentBitSet componentBitSet;
//     GroupBitSet groupBitSet;

// public:
//     Entity(Manager& mManager) : manager(mManager) {} // mManager as in member
//     void update()
//     {
//         for (auto& c : components) c->update();
//     }

//     void draw()
//     {
//         for (auto& c : components) c->draw();
//     }

//     // void draw() {}
//     bool isActive() const { return active; }
//     void destroy() { active = false; }

//     bool hasGroup(Group mGroup)
//     {
//         return groupBitSet[mGroup];
//     }

//     template <typename T> bool hasComponent() const 
//     {
//         return componentBitSet[getComponentTypeID<T>()];
//     }

//     void addGroup(Group mGroup);
//     void delGroup(Group mGroup)
//     {
//         groupBitSet[mGroup] = false;
//     }

//     template <typename T, typename... TArgs>
//     T& addComponent(TArgs&&... mArgs)
//     {
//         T* c(new T(std::forward<TArgs>(mArgs)...));
//         c->entity = this;
//         std::unique_ptr<Component> uPtr{ c };
//         components.emplace_back(std::move(uPtr));

//         componentArray[getComponentTypeID<T>()] = c;
//         componentBitSet[getComponentTypeID<T>()] = true;

//         c->init();
//         return *c;
//     }

//     template<typename T> T& getComponent() const
//     {
//         auto ptr(componentArray[getComponentTypeID<T>()]);
//         return *static_cast<T*>(ptr);
//     }

// };


// class Manager
// {
// private:
//     std::vector<std::unique_ptr<Entity>> entities;
//     std::array<std::vector<Entity*>, maxGroups> groupedEntities;

// public:
//     void update()
//     {
//         for (auto& e : entities) e->update();
//     }
//     void draw()
//     {
//         for (auto& e : entities) e->draw();
//     }

//     void refresh()
//     {
//         for (auto i(0u); i < maxGroups; i++)
//         {
//             auto& v(groupedEntities[i]);
//             v.erase(
//                 std::remove_if(std::begin(v), std::end(v),
//                     [i](Entity* mEntity)
//             {
//                 return !mEntity->isActive() || !mEntity->hasGroup(i);
//             }),
//                 std::end(v));
//         }

//         entities.erase(std::remove_if(std::begin(entities), std::end(entities),
//             [](const std::unique_ptr<Entity>& mEntity)
//             {
//                 return !mEntity->isActive();
//             }), 
//             std::end(entities));
//     }

//     void AddToGroup(Entity* mEntity, Group mGroup)
//     {
//         groupedEntities[mGroup].emplace_back(mEntity);  //  add Entity to Group
//     }

//     std::vector<Entity*>& getGroup(Group mGroup)
//     {
//         return groupedEntities[mGroup];
//     }

//     Entity& addEntity()
//     {
//         Entity* e = new Entity(*this);
//         std::unique_ptr<Entity> uPtr{ e };
//         entities.emplace_back(std::move(uPtr));
//         return *e;
//     }

// };

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

// ... (getNewComponentTypeID and getComponentTypeID are unchanged) ...

inline ComponentID getNewComponentTypeID()
{
    static ComponentID lastID = 0u;
    return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}


constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;

class Component
{
public:
    Entity* entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}

    virtual ~Component() {}
};

class Entity
{
private:
    Manager& manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitSet groupBitSet;

    std::size_t id; // <-- ADD THIS: Each entity will now have a unique ID.

public:
    // Modify the constructor to accept and set the entity's ID
    Entity(Manager& mManager, std::size_t mId) : manager(mManager), id(mId) {} // <-- MODIFY THIS LINE

    // Add a getter function for the ID
    std::size_t getID() const { return id; } // <-- ADD THIS FUNCTION

    void update()
    {
        for (auto& c : components) c->update();
    }

    void draw()
    {
        for (auto& c : components) c->draw();
    }

    bool isActive() const { return active; }
    void destroy() { active = false; }

    bool hasGroup(Group mGroup)
    {
        return groupBitSet[mGroup];
    }

    void addGroup(Group mGroup);
    void delGroup(Group mGroup)
    {
        groupBitSet[mGroup] = false;
    }

    template <typename T> bool hasComponent() const 
    {
        return componentBitSet[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{ c };
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template<typename T> T& getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
};


class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;
    std::size_t lastEntityID = 0; // <-- ADD THIS: To generate unique IDs for new entities.

public:
    void update()
    {
        for (auto& e : entities) e->update();
    }
    void draw()
    {
        for (auto& e : entities) e->draw();
    }

    void refresh()
    {
        for (auto i(0u); i < maxGroups; i++)
        {
            auto& v(groupedEntities[i]);
            v.erase(
                std::remove_if(std::begin(v), std::end(v),
                    [i](Entity* mEntity)
            {
                return !mEntity->isActive() || !mEntity->hasGroup(i);
            }),
                std::end(v));
        }

        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
            [](const std::unique_ptr<Entity>& mEntity)
            {
                return !mEntity->isActive();
            }),
            std::end(entities));
    }

    void AddToGroup(Entity* mEntity, Group mGroup)
    {
        groupedEntities[mGroup].emplace_back(mEntity);
    }

    std::vector<Entity*>& getGroup(Group mGroup)
    {
        return groupedEntities[mGroup];
    }

    // This version of addEntity is for creating an entity with a SPECIFIC ID
    // The client uses this to create "ghost" entities that match the server's IDs.
    Entity& addEntity(std::size_t id) // <-- ADD THIS FUNCTION
    {
        Entity *e = new Entity(*this, id);
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
        return *e;
    }

    // This version of addEntity is for creating a new entity with a NEW, unique ID.
    // The server uses this when a new player connects.
    Entity& addEntity() // <-- MODIFY THIS FUNCTION
    {
        // It now calls the other addEntity with an incrementing ID
        return addEntity(lastEntityID++);
    }
    
    // Function to check if an entity with a given ID exists.
    bool hasEntity(std::size_t id) const // <-- ADD THIS FUNCTION
    {
        // This loops through all entities and returns true if one with the matching ID is found.
        return std::any_of(entities.begin(), entities.end(), [id](const auto& e) {
            return e->getID() == id;
        });
    }

    // Function to retrieve a reference to an entity using its ID.
    Entity& getEntity(std::size_t id) // <-- ADD THIS FUNCTION
    {
        // This loops through all entities and returns a reference to the one with the matching ID.
        auto found = std::find_if(entities.begin(), entities.end(), [id](const auto& e) {
            return e->getID() == id;
        });
        return *found->get();
    }
};