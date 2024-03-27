//
// Created by User on 2024/3/15.
//

#include "Button.hpp"
#include "Util/Input.hpp"
#include "CollisionHandler.hpp"

Button::Button(const std::shared_ptr<Core::Drawable> &idle, const std::shared_ptr<Core::Drawable> &hover,
               const std::shared_ptr<Core::Drawable> &click, AudioManager audiomanager, float z_index)
        : Util::GameObject(), audiomanager_(std::move(audiomanager)) {
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
            audiomanager_.Play(AudioManager::SFX::Button);
            break;
    }
}

void Button::Update() {
    if (CollisionHandler::IsCollide(Util::Input::GetCursorPosition(), GetCollider())) {
        UpdateState(State::Hover);
        if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
            UpdateState(State::Click);
        }
    } else {
        UpdateState(State::Idle);
    }
}

void Button::Enable() {
    SetVisible(true);
    UpdateState(State::Idle);
}

void Button::Disable() {
    SetVisible(false);
    UpdateState(State::Idle);
}

void Button::OnClick() {
    animator_.UpdateAnimationState("Click", [this](const std::shared_ptr<Core::Drawable> &drawable) {
        this->SetDrawable(drawable);
    });
}

void Button::OnHover() {
    animator_.UpdateAnimationState("Hover", [this](const std::shared_ptr<Core::Drawable> &drawable) {
        this->SetDrawable(drawable);
    });
}

void Button::OnIdle() {
    animator_.UpdateAnimationState("Idle", [this](const std::shared_ptr<Core::Drawable> &drawable) {
        this->SetDrawable(drawable);
    });
}
