//
// Created by User on 2024/3/6.
//

#include <cmath>
#include <memory>
#include "Rigidbody.hpp"
#include "CollisionHandler.hpp"
#include "Collider.hpp"
#include "Sprite.hpp"
#include "Util/Logger.hpp"

void Rigidbody::Update(Collider collider, const std::vector<std::shared_ptr<Sprite>> &walls,
                       const std::function<void(glm::vec2)> &translate) {
    SetVelocity(GetVelocity() + GetAcceleration());
    ResetAcceleration();

    glm::vec2 nearest_delta_position = GetVelocity();
    glm::vec2 max_velocity = {GetVelocity().x, GetVelocity().y};
    for (const auto &wall: walls) {
        if (!wall->enabled) { continue; }
        glm::vec2 delta_velocity = CollisionHandler::SweepTest(collider, wall->GetCollider(), GetVelocity());
        if (hypot(GetVelocity().x, GetVelocity().y) != 0) {
            if (abs(delta_velocity.x) < abs(max_velocity.x)) {
                nearest_delta_position.x = delta_velocity.x;
            }
            if (abs(delta_velocity.y) < abs(max_velocity.y)) {
                nearest_delta_position.y = delta_velocity.y;
            }
        }
    }
    if (collider.top + nearest_delta_position.y > walls[5]->GetCollider().bottom){
        LOG_DEBUG("{}", nearest_delta_position.y);
        glm::vec2 delta_velocity = CollisionHandler::SweepTest(collider, walls[5]->GetCollider(), GetVelocity());
    }
    SetVelocity(nearest_delta_position);

    translate(GetVelocity());
}
