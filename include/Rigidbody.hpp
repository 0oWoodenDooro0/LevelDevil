//
// Created by User on 2024/3/6.
//

#ifndef LEVELDEVIL_RIGIDBODY_HPP
#define LEVELDEVIL_RIGIDBODY_HPP


#include <glm/vec2.hpp>
#include <functional>
#include "Collider.hpp"
#include "Sprite.hpp"

class Rigidbody {
public:
    Rigidbody() = default;

    inline void SetAcceleration(glm::vec2 acceleration) { acceleration_ = acceleration; }

    inline void SetVelocity(glm::vec2 velocity) { velocity_ = velocity; }

    [[nodiscard]] inline glm::vec2 GetAcceleration() const { return acceleration_; }

    [[nodiscard]] inline glm::vec2 GetVelocity() const { return velocity_; }

    inline void ResetAcceleration() { acceleration_ = {0, 0}; }

    inline void ResetVelocity() { velocity_ = {0, 0}; }

    inline void AddAcceleration(glm::vec2 acceleration) { acceleration_ += acceleration; }

    void Update(const Collider &collider, const std::vector<std::shared_ptr<GameObject>> &walls,
                const std::function<void(glm::vec2)> &translate);

private:
    glm::vec2 acceleration_ = {0, 0};
    glm::vec2 velocity_ = {0, 0};
};


#endif //LEVELDEVIL_RIGIDBODY_HPP
