//
// Created by User on 2024/3/4.
//

#include "Animator.hpp"
#include "Util/Animation.hpp"

#include <utility>

void Animator::SetAnimationStates(std::map<std::string, std::shared_ptr<Core::Drawable>, std::less<>> states) {
    animation_states_ = std::move(states);
}

void Animator::UpdateAnimationState(const std::string &animation_state_name,
                                    const std::function<void(std::shared_ptr<Core::Drawable>)> &set_drawable) {
    if (current_animation_state_ == animation_state_name) return;
    current_animation_state_ = animation_state_name;
    set_drawable(animation_states_[animation_state_name]);
    auto animation = std::dynamic_pointer_cast<Util::Animation>(animation_states_[animation_state_name]);
    if (animation == nullptr) return;
    animation->Play();
}

std::shared_ptr<Util::Animation> Animator::GetAnimation(const std::string& key) {
    return std::dynamic_pointer_cast<Util::Animation>(animation_states_[key]);
}
