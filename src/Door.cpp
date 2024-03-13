//
// Created by User on 2024/3/8.
//

#include "Door.hpp"

#include <utility>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "CollisionHandler.hpp"
#include "Util/Input.hpp"
#include "Character.hpp"
#include "Animator.hpp"

Door::Door() : Util::GameObject() {
    SetZIndex(0);
    m_Drawable = std::make_unique<Util::Image>(RESOURCE_DIR"/image/door/door.png");
    animator_.SetAnimationStates(
        { {"Idle",std::make_unique<Util::Image>(RESOURCE_DIR"/image/door/door.png")},
         {"StageClear",  std::make_unique<Util::Animation>(
                 std::vector<std::string>{(RESOURCE_DIR"/image/door/in_door1.png"),
                                          (RESOURCE_DIR"/image/door/in_door2.png"),
                                          (RESOURCE_DIR"/image/door/in_door3.png"),
                                          (RESOURCE_DIR"/image/door/in_door4.png"),
                                          (RESOURCE_DIR"/image/door/in_door5.png")}, true, 450,
                 false, 0)} });
}

void Door::IfStageClear(bool ifclear) {
    std::function<void(std::shared_ptr<Core::Drawable>)> set_drawable_function = [&](
        std::shared_ptr<Core::Drawable> drawable) { m_Drawable = std::move(drawable); };
    if (ifclear)
        animator_.UpdateAnimationState("StageClear", set_drawable_function);
    else
        animator_.UpdateAnimationState("Idle", set_drawable_function);
    
}

bool Door::Update(std::shared_ptr<Character> character_) {
    if (CollisionHandler::CheckCollision(character_->GetCollider(), GetCollider())) {
        return true;
    }
    else {
        return false;
    }
}