//
// Created by User on 2024/3/1.
//

#include "Background.hpp"

Background::Background() : GameObject(
        std::make_unique<Util::Image>(RESOURCE_DIR"/image/background/test_background.png"), -10) {}