//
// Created by User on 2024/3/4.
//

#ifndef LEVELDEVIL_ANIMATOR_HPP
#define LEVELDEVIL_ANIMATOR_HPP


#include <string>
#include <memory>
#include <map>
#include "Core/Drawable.hpp"
#include "Util/Animation.hpp"
#include <functional>

class Animator {
public:
    Animator() = default;

    void SetAnimationStates(std::map<std::string, std::shared_ptr<Core::Drawable>, std::less<>> states);

    void UpdateAnimationState(const std::string &animation_state_name,
                              const std::function<void(std::shared_ptr<Core::Drawable>)> &set_drawable);

    std::shared_ptr<Util::Animation> GetAnimation(const std::string& key);

private:
    std::string current_animation_state_;
    std::map<std::string, std::shared_ptr<Core::Drawable>, std::less<>> animation_states_;
};


#endif //LEVELDEVIL_ANIMATOR_HPP
