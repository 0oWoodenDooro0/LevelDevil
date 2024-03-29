//
// Created by User on 2024/3/21.
//
#include "Level2.hpp"
#include <utility>
Level2::Level2(std::function<void(Level::State)> set_level_state_function) : set_level_state_function_(
    std::move(set_level_state_function)) {}

void Level2::Start() {
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/Level2/background.png");
    root_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_maganer_);
    button_->SetPosition({ -800, 416 });
    root_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_maganer_);
    character_->SetCheckPoint({ 704, -64 });
    root_.AddChild(character_);
    door_ = std::make_shared<Door>(audio_maganer_);
    door_->SetPosition({ -704, -64 });
    root_.AddChild(door_);
    auto wall_image = std::vector<std::string>{ RESOURCE_DIR"/image/level/Level2/hole.png",
                                               RESOURCE_DIR"/image/level/Level2/Lbottom.png",
                                               RESOURCE_DIR"/image/level/Level2/Lside.png",
                                               RESOURCE_DIR"/image/level/Level2/Ltop.png",
                                               RESOURCE_DIR"/image/level/Level2/Rbottom.png",
                                               RESOURCE_DIR"/image/level/Level2/Rtop.png" };
    for (int i = 0; i < 6; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image[i]));
        walls_.push_back(wall);
        root_.AddChild(wall);
    }
    walls_[0]->SetPosition({ 768, -320 });
    walls_[1]->SetPosition({ -64, -288 });
    walls_[2]->SetPosition({ -832, 0 });
    walls_[3]->SetPosition({ -736, 256 });
    walls_[4]->SetPosition({ 832, -288 });
    walls_[5]->SetPosition({ 128, 224 });
}

void Level2::Update() {
    audio_maganer_.Update();

    button_->Update();
    if (button_->GetState() == Button::State::Click) {
        set_level_state_function_(Level::State::LEVEL_SELECT);
    }
    character_->Update(walls_);
    door_->Update(character_);

    root_.Update();
}

void Level2::End() {

}

void Level2::ResetLevel() {
    
}