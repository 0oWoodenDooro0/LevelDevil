//
// Created by User on 2024/3/27.
//


#include "LevelButton.hpp"

#include <utility>
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "Character.hpp"

LevelButton::LevelButton(AudioManager audiomanager) : Util::GameObject(), audiomanager_(std::move(audiomanager)) {
    SetZIndex(0);
    m_Drawable = std::make_unique<Util::Image>(RESOURCE_DIR"/image/component/LbuttonUp.png");
    animator_.SetAnimationStates(
            {{"Up",   std::make_unique<Util::Image>(RESOURCE_DIR"/image/component/LbuttonUp.png")},
             {"Down", std::make_unique<Util::Image>(RESOURCE_DIR"/image/component/LbuttonDown.png")}});
}

void LevelButton::Update(const std::shared_ptr<Character> &character_) {

}

void LevelButton::Enable() {

}

void LevelButton::Disable() {

}