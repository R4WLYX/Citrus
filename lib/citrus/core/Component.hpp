#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <typeinfo>

template <typename C>
struct Component {
    const char* type;
    
    Component()
    : type(typeid(C).name())
    {}
};

#endif