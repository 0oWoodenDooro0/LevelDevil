//
// Created by User on 2024/3/22.
//

#ifndef LEVELDEVIL_TRIGGERCOLLIDER_HPP
#define LEVELDEVIL_TRIGGERCOLLIDER_HPP

#include "Util/GameObject.hpp"
#include "glm/vec2.hpp"
#include "Collider.hpp"

class TriggerCollider {
public:
    enum class State {
        Idle,
        Trigger
    };

    explicit TriggerCollider(Collider collider) : position_(collider.center), size_(collider.size) {};

    inline glm::vec2 GetPosition() { return position_; }

    inline glm::vec2 GetSize() { return size_; }

    inline Collider GetCollider() { return {GetPosition(), GetSize()}; }

    inline State GetState() { return current_state_; }

    inline void SetPosition(glm::vec2 position) { position_ = position; }

    inline void SetSize(glm::vec2 size) { size_ = size; }

    void UpdateState(TriggerCollider::State state);

    void Update(glm::vec2 position);

private:
    State current_state_ = State::Idle;
    glm::vec2 position_ = {0, 0};
    glm::vec2 size_ = {0, 0};
};


#endif //LEVELDEVIL_TRIGGERCOLLIDER_HPP
