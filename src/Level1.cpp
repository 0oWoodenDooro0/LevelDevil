//
// Created by User on 2024/3/20.
//

#include "Level1.hpp"

#include <utility>

Level1::Level1(std::function<void(Level::State)> set_level_state_function) : set_level_state_function_(
        std::move(set_level_state_function)) {}

void Level1::Start() {
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/background/test_background.png");
    root_.AddChild(background_);
    character_ = std::make_shared<Character>(audio_maganer_);
    character_->SetPosition({0, 0});
    root_.AddChild(character_);
    door_ = std::make_shared<Door>();
    door_->SetPosition({128, -136});
    root_.AddChild(door_);
    for (int i = 0; i < 10; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(RESOURCE_DIR"/image/wall/test_wall.png"));
        walls_.push_back(wall);
        root_.AddChild(wall);
    }
    walls_[0]->SetPosition({0, -200});
    walls_[1]->SetPosition({64, -200});
    walls_[2]->SetPosition({-64, -200});
    walls_[3]->SetPosition({-128, -200});
    walls_[4]->SetPosition({128, -200});
    walls_[5]->SetPosition({-192, -200});
    walls_[6]->SetPosition({192, -200});
    walls_[7]->SetPosition({-192, -136});
    walls_[8]->SetPosition({192, -136});
    walls_[9]->SetPosition({192, -72});
}

void Level1::Update() {
    audio_maganer_.Update();

    character_->Update(walls_);
    door_->Update(character_);

    root_.Update();
}

void Level1::End() {

}
