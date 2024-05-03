//
// Created by User on 2024/3/24.
//

#include "EscButton.hpp"

#include <utility>

EscButton::EscButton(AudioManager audio_manager)
        : Button(std::make_shared<Util::Image>(RESOURCE_DIR"/image/button/escbtn_up.png"),
                 std::make_shared<Util::Image>(RESOURCE_DIR"/image/button/escbtn_up.png"),
                 std::make_shared<Util::Image>(RESOURCE_DIR"/image/button/escbtn_down.png"),
                 std::move(audio_manager)) {}