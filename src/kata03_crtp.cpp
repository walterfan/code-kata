#include <string>
#include <memory>
#include <sstream>
#include <iostream>

template <typename T>
class Entity  {
public:
    Entity(uint32_t id, const std::string& name): m_id(id), m_name(name) {

    }
    const std::string& getName() const { return m_name; }
    uint32_t getId() const { return m_id; }

    std::string toString() const {

        return static_cast<T*>(this)->toString();
    }

    static std::shared_ptr<T> createInstance(uint32_t id, const std::string& name)
    {
        return T::createInstance(id, name);
    }
protected:
    uint32_t m_id;
    std::string m_name;

};


class User : Entity<User>
{
public:

    User(uint32_t id, const std::string& name):Entity<User>(id, name) {  }
    std::string toString() const {
        std::ostringstream out;
        out << "user: id=" << m_id;
        out << ", name=" << m_name;
        return out.str();

    }
    static std::shared_ptr<User> createInstance(uint32_t id, const std::string& name) {
        return std::make_shared<User>(id, name);
    }
};

class Room : Entity<Room>
{
public:

    Room(uint32_t id, const std::string& name):Entity<Room>(id, name) {  }
    std::string toString() const {
        std::ostringstream out;
        out << "room: id=" << m_id;
        out << ", name=" << m_name;
        return out.str();

    }
    static std::shared_ptr<Room> createInstance(uint32_t id, const std::string& name) {
        return std::make_shared<Room>(id, name);
    }
};

int kata03_crtp(int argc, char** argv)
{
    auto user = User::createInstance(1, "Alice");
    std::cout<< user->toString()<<std::endl;

    auto room = Room::createInstance(2, "House");
    std::cout<< room->toString()<<std::endl;

    return 0;
}
