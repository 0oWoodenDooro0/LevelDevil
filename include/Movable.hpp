//
// Created by User on 2024/5/10.
//

#ifndef LEVELDEVIL_MOVABLE_HPP
#define LEVELDEVIL_MOVABLE_HPP

#include "GameObject.hpp"

class Movable {
public:
    static void Move(const std::shared_ptr<GameObject> &gameObject, glm::vec2 target_position, float speed);
};


#endif //LEVELDEVIL_MOVABLE_HPP
