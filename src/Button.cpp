//
// Created by User on 2024/3/15.
//

#include "Button.hpp"

Button::Button(const std::shared_ptr<Core::Drawable> &idle, const std::shared_ptr<Core::Drawable> &hover,
               const std::shared_ptr<Core::Drawable> &click, float z_index) : Util::GameObject() {
    SetDrawable(idle);
    SetZIndex(z_index);
    animator_.SetAnimationStates({{"Idle", idle}, {"Hover", hover}, {"Click", click}});
}
