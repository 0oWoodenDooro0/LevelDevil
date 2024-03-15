//
// Created by User on 2024/3/15.
//

#ifndef LEVELDEVIL_BUTTON_HPP
#define LEVELDEVIL_BUTTON_HPP


#include "Util/GameObject.hpp"
#include "Animator.hpp"

class Button : public Util::GameObject {
public:
    enum class State{
        Idle,
        Hover,
        Click
    };

    explicit Button(const std::shared_ptr<Core::Drawable> &idle, const std::shared_ptr<Core::Drawable> &hover,
                    const std::shared_ptr<Core::Drawable> &click, float z_index);

    virtual void OnClick() = 0;

    virtual void OnHover() = 0;

    virtual void OnIdle() = 0;

private:
    Animator animator_;
    State current_state_ = State::Idle;
};


#endif //LEVELDEVIL_BUTTON_HPP
