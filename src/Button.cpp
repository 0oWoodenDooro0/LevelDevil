//
// Created by User on 2024/3/11.
//

#include "Button.hpp"

#include <utility>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Collider.hpp"
#include "Util/Input.hpp"
#include "Character.hpp"
#include "Animator.hpp"

Button::Button(const std::string& Idle, const std::string& Cursor_OnTop, const std::string& Pressed) : Util::GameObject() {
    SetZIndex(1);
    m_Drawable = std::make_unique<Util::Image>(Idle);
    animator_.SetAnimationStates(
        { {"Idle", std::make_unique<Util::Image>(Idle)},
        {"Cursor_OnTop", std::make_unique<Util::Image>(Cursor_OnTop)},
         {"Pressed", std::make_unique<Util::Image>(Pressed)} });
}

void Button::Update() {
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [&](
        std::shared_ptr<Core::Drawable> drawable) { m_Drawable = std::move(drawable); };
    if (CollisionHandler::CheckCollision(Collider(GetPosition(), GetSize()), Collider(Util::Input::GetCursorPosition(), glm::vec2({ 0,0 })))) {
        if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
            animator_.UpdateAnimationState("Pressed", set_drawable_function);
            keyEvent_ = false;
        }
        else if(Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)){
            animator_.UpdateAnimationState("Idle", set_drawable_function);
            keyEvent_ = true;
        }
        else {
            animator_.UpdateAnimationState("Cursor_OnTop", set_drawable_function);
            keyEvent_ = false;
        }
    }
    else {
        animator_.UpdateAnimationState("Idle", set_drawable_function);
        keyEvent_ = false;
    }
}