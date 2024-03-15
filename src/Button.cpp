//
// Created by User on 2024/3/15.
//

#include "Button.hpp"
#include "Util/Input.hpp"
#include "CollisionHandler.hpp"

Button::Button(const std::shared_ptr<Core::Drawable> &idle, const std::shared_ptr<Core::Drawable> &hover,
               const std::shared_ptr<Core::Drawable> &click, float z_index) : Util::GameObject() {
    SetDrawable(idle);
    SetZIndex(z_index);
    animator_.SetAnimationStates({{"Idle",  idle},
                                  {"Hover", hover},
                                  {"Click", click}});
}

void Button::UpdateState(Button::State state) {
    if (current_state_ == state) return;
    current_state_ = state;
    switch (current_state_) {
        case State::Idle:
            OnIdle();
            break;
        case State::Hover:
            OnHover();
            break;
        case State::Click:
            OnClick();
            break;
    }
}

void Button::Update() {
    if (CollisionHandler::IsCollide(Util::Input::GetCursorPosition(), GetCollider())) {
        UpdateState(State::Hover);
        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            UpdateState(State::Click);
        }
    } else {
        UpdateState(State::Idle);
    }
}
