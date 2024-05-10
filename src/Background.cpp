//
// Created by User on 2024/3/1.
//

#include "Background.hpp"

Background::Background(const std::string &path) : GameObject() {
    SetDrawable(std::make_unique<Util::Image>(path));
    SetZIndex(-10);
}