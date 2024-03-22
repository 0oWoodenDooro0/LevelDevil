//
// Created by User on 2024/3/22.
//

#include "TriggerCollider.hpp"
#include "CollisionHandler.hpp"

void TriggerCollider::Update(glm::vec2 position) {
    if (CollisionHandler::IsCollide(position, GetCollider())) {
        UpdateState(State::Trigger);
        return;
    }
    UpdateState(State::Idle);
}

void TriggerCollider::UpdateState(TriggerCollider::State state) {
    if (current_state_ == state)return;
    current_state_ = state;
}
