//
// Created by User on 2024/3/4.
//

#include "Animator.hpp"

#include <utility>

void Animator::SetAnimationStates(std::map<std::string, std::shared_ptr<Core::Drawable>, std::less<>> states) {
    animation_states_ = std::move(states);
}

void Animator::UpdateAnimationState(const std::string &animation_state_name, set_drawable_function set_drawable) {
    if (current_animation_state_ == animation_state_name) return;
    current_animation_state_ = animation_state_name;
    set_drawable(animation_states_[animation_state_name]);
}
