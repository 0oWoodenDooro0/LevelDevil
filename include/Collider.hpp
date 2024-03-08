//
// Created by User on 2024/3/8.
//

#ifndef LEVELDEVIL_COLLIDER_HPP
#define LEVELDEVIL_COLLIDER_HPP

#include "glm/vec2.hpp"

class Collider {
public:
    Collider(glm::vec2 position, glm::vec2 _size);
    glm::vec2 center{}, size{};
    float top, bottom, left, right;
};


#endif //LEVELDEVIL_COLLIDER_HPP
