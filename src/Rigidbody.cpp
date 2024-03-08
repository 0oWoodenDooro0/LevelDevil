//
// Created by User on 2024/3/6.
//

#include <cmath>
#include <memory>
#include "Rigidbody.hpp"
#include "CollisionHandler.hpp"
#include "Wall.hpp"
#include "Collider.hpp"

void Rigidbody::Update(Collider collider, const std::vector<std::shared_ptr<Wall>> &walls,
                       const std::function<void(glm::vec2)> &translate) {
    SetVelocity(GetVelocity() + GetAcceleration());
    ResetAcceleration();

    float nearest_time = 1;
    glm::vec2 nearest_delta_position = GetVelocity();
    float max_time = hypot(GetVelocity().x, GetVelocity().y);
    for (const auto &wall: walls) {
        glm::vec2 delta_position = CollisionHandler::SweepTest(collider, wall->GetCollider(), GetVelocity());
        float delta_time = hypot(delta_position.x, delta_position.y);
        if (max_time != 0) {
            float time = delta_time / max_time;
            if (time < nearest_time) {
                nearest_time = time;
                nearest_delta_position = delta_position;
            }
        }
    }
    SetVelocity(nearest_delta_position);

    translate(GetVelocity());
}
