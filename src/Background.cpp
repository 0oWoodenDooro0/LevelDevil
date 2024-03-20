//
// Created by User on 2024/3/1.
//

#include "Background.hpp"

Background::Background(const std::string& path) : GameObject(
        std::make_unique<Util::Image>(path), -10) {}
