//
// Created by User on 2024/3/4.
//

#ifndef LEVELDEVIL_ANIMATOR_HPP
#define LEVELDEVIL_ANIMATOR_HPP


#include <string>
#include <memory>
#include <map>
#include "Core/Drawable.hpp"

class Animator {
public:
    Animator() = default;
    void SetAnimationStates(std::map<std::string, std::shared_ptr<Core::Drawable>> states);
    void UpdateAnimationState(const std::string &animation_state_name,
                              const std::function<void(std::shared_ptr<Core::Drawable>)> &set_drawable);
private:
    std::string current_animation_state_;
    std::map<std::string, std::shared_ptr<Core::Drawable>> animation_states_;
};


#endif //LEVELDEVIL_ANIMATOR_HPP
