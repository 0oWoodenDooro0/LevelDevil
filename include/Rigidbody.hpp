//
// Created by User on 2024/3/6.
//

#ifndef LEVELDEVIL_RIGIDBODY_HPP
#define LEVELDEVIL_RIGIDBODY_HPP


#include <glm/vec2.hpp>
#include <functional>
#include "Wall.hpp"
#include "Collider.hpp"

class Rigidbody {
public:
    Rigidbody() = default;

    inline void SetAcceleration(glm::vec2 acceleration) { acceleration_ = acceleration; }

    inline void SetVelocity(glm::vec2 velocity) { velocity_ = velocity; }

    inline glm::vec2 GetAcceleration() { return acceleration_; }

    inline glm::vec2 GetVelocity() { return velocity_; }

    inline void ResetAcceleration() { acceleration_ = {0, 0}; }

    inline void ResetVelocity() { velocity_ = {0, 0}; }

    void Update(Collider collider, const std::vector<std::shared_ptr<Wall>> &walls,
                const std::function<void(glm::vec2)> &translate);

private:
    glm::vec2 acceleration_ = {0, 0};
    glm::vec2 velocity_ = {0, 0};
};


#endif //LEVELDEVIL_RIGIDBODY_HPP
