//
// Created by User on 2024/3/21.
//

#include "Level1.hpp"

#include <utility>

Level1::Level1(std::function<void(Level::State)> set_level_state_function) : set_level_state_function_(
        std::move(set_level_state_function)) {}

void Level1::Start() {
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/level1/background.png");
    root_.AddChild(background_);
    button_ = std::make_shared<EscButton>();
    button_->SetPosition({-800, 416});
    root_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_maganer_);
    character_->SetCheckPoint({-576, -128});
    root_.AddChild(character_);
    door_ = std::make_shared<Door>(audio_maganer_);
    door_->SetPosition({576, -128});
    root_.AddChild(door_);
    auto wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/level1/top.png",
                                               RESOURCE_DIR"/image/level/level1/bottom.png",
                                               RESOURCE_DIR"/image/level/level1/side.png",
                                               RESOURCE_DIR"/image/level/level1/side.png"};
    for (int i = 0; i < 4; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image[i]));
        walls_.push_back(wall);
        root_.AddChild(wall);
    }
    walls_[0]->SetPosition({0, 320});
    walls_[1]->SetPosition({640, -320});
    walls_[2]->SetPosition({768, 0});
    walls_[3]->SetPosition({-768, 0});
    auto movable_wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/level1/move1.png",
                                                       RESOURCE_DIR"/image/level/level1/move2.png"};
    for (int i = 0; i < 2; ++i) {
        auto movable_wall = std::make_shared<MovableSprite>(std::make_shared<Util::Image>(movable_wall_image[i]));
        movable_walls_.push_back(movable_wall);
        walls_.push_back(movable_wall);
        root_.AddChild(movable_wall);
    }
    walls_[4]->SetPosition({-288, -320});
    walls_[5]->SetPosition({256, -320});
}

void Level1::Update() {
    audio_maganer_.Update();

    button_->Update();
    if (button_->GetState() == Button::State::Click) {
        set_level_state_function_(Level::State::LEVEL_SELECT);
    }

    character_->Update(walls_);
    door_->Update(character_);

    root_.Update();
}

void Level1::End() {

}