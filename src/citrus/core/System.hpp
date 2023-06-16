#ifndef SYSTEM_HPP
#define SYSTEM_HPP

class System {
public:
    virtual void on_start() {};
    virtual void on_update(float deltaTime) = 0;
};


#endif