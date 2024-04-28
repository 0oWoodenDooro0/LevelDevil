//
// Created by User on 2024/3/2.
//

#ifndef LEVELDEVIL_COLLISIONHANDLER_HPP
#define LEVELDEVIL_COLLISIONHANDLER_HPP


#include <glm/vec2.hpp>
#include <optional>
#include "Collider.hpp"

class CollisionHandler {
public:
    static bool CheckCollision(const Collider &collider, const Collider &other);

    static bool IsCollide(glm::vec2 position, const Collider &collider);

    static glm::vec2 SweepTest(const Collider &collider, const Collider &other, glm::vec2 velocity);
};


#endif //LEVELDEVIL_COLLISIONHANDLER_HPP
