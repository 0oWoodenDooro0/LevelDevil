//
// Created by User on 2024/3/12.
//

#include "Sprite.hpp"

Sprite::Sprite(const std::shared_ptr<Core::Drawable>& drawable, float z_index) : GameObject() {
    SetDrawable(drawable);
    SetZIndex(z_index);
}
