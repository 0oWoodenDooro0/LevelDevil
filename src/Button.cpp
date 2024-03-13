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

Button::Button(std::shared_ptr<Core::Drawable> Idle, std::shared_ptr<Core::Drawable> Cursor_OnTop, std::shared_ptr<Core::Drawable> Pressed) : Util::GameObject() {
    SetZIndex(1);
    m_Drawable = std::make_unique<Util::Image>(Idle);
    animator_.SetAnimationStates(
        { {"Idle", Idle},
        {"Cursor_OnTop", Cursor_OnTop},
         {"Pressed", Pressed} });
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