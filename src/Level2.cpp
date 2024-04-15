//
// Created by User on 2024/3/21.
//
#include "Level2.hpp"
#include "InputHandler.hpp"

#include <utility>

Level2::Level2(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_maganer_(std::move(audio_manager)) {}

void Level2::Start() {
    auto top = std::make_shared<MovableSprite>(
            std::make_shared<Util::Image>(RESOURCE_DIR"/image/ui/transition_top.png"), 20);
    auto bottom = std::make_shared<MovableSprite>(
            std::make_shared<Util::Image>(RESOURCE_DIR"/image/ui/transition_bottom.png"), 20);
    top->SetPosition({0, 208});
    bottom->SetPosition({0, -210});
    transitions_.push_back(top);
    transitions_.push_back(bottom);
    root_.AddChild(top);
    root_.AddChild(bottom);
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/Level2/background.png");
    root_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_maganer_);
    button_->SetPosition({-800, 416});
    root_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_maganer_);
    character_->SetCheckPoint({704, -64});
    root_.AddChild(character_);
    door_ = std::make_shared<Door>(audio_maganer_);
    door_->SetPosition({-704, -64});
    root_.AddChild(door_);
    auto wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/Level2/hole.png",
                                               RESOURCE_DIR"/image/level/Level2/Lbottom.png",
                                               RESOURCE_DIR"/image/level/Level2/Lside.png",
                                               RESOURCE_DIR"/image/level/Level2/Ltop.png",
                                               RESOURCE_DIR"/image/level/Level2/Rbottom.png",
                                               RESOURCE_DIR"/image/level/Level2/Rtop.png"};
    for (int i = 0; i < 6; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image[i]));
        walls_.push_back(wall);
        root_.AddChild(wall);
    }
    walls_[0]->SetPosition({768, -320});
    walls_[1]->SetPosition({-64, -288});
    walls_[2]->SetPosition({-832, 0});
    walls_[3]->SetPosition({-736, 256});
    walls_[4]->SetPosition({832, -288});
    walls_[5]->SetPosition({128, 224});

    auto spike_image = RESOURCE_DIR"/image/level/Level2/spike.png";
    for (int i = 0; i < 22; i++) {
        auto spike = std::make_shared<Spike>(spike_image, audio_maganer_);
        spikes_.push_back(spike);
        root_.AddChild(spike);
        spike->SetPosition({-640 + i * 64, -64});
    }
}

void Level2::Update() {
    if (character_->GetEnabled()) {
        if (character_->GetPosition().y < -480) {
            character_->UpdateState(Character::State::Dead);
        }
        auto input_vector = InputHandler::GetCharacterMoveVelocity();
        character_->Move(input_vector, walls_);
    } else {
        if (character_->GetCurrentState() != Character::State::LevelClear &&
            (InputHandler::isForwardPressed() || InputHandler::isBackwardPressed() || InputHandler::isJumpPressed())) {
            character_->Revive();
            ResetLevel();
        }
    }
    button_->Update();
    if (button_->GetState() == Button::State::Click) {
        level_ = Level::State::LEVEL_SELECT;
        UpdateCurrentState(State::Outro);
    }

    door_->Update(character_);
    if (!door_->GetEnabled()) {
        level_ = Level::State::LEVEL_SELECT;
        UpdateCurrentState(State::Outro);
    }

    switch (current_state_) {
        case State::Intro:
            transitions_[0]->Move({0, 752}, 750);
            transitions_[1]->Move({0, -750}, 750);
            if (transitions_[0]->GetPosition() == glm::vec2{0, 752} &&
                transitions_[1]->GetPosition() == glm::vec2{0, -750}) {
                UpdateCurrentState(State::Start);
            }
            break;
        case State::Start:
            break;
        case State::Spike:
            break;
        case State::Outro:
            transitions_[0]->Move({0, 208}, 750);
            transitions_[1]->Move({0, -210}, 750);
            if (transitions_[0]->GetPosition() == glm::vec2{0, 208} &&
                transitions_[1]->GetPosition() == glm::vec2{0, -210}) {
                set_level_state_function_(level_);
            }
            break;
    }
    root_.Update();
}

void Level2::End() {

}

void Level2::ResetLevel() {
    current_state_ = State::Start;
    for (int i = 0; i < 22; i++) {
        spikes_[i]->Disable();
    }
}

void Level2::UpdateCurrentState(State state) {
    if (current_state_ == state)return;
    switch (current_state_) {
        case State::Intro:
            if (state == State::Start) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
            }
            break;
        case State::Start:
            if (state == State::Spike) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
            }
            break;
        case State::Spike:
            if (state == State::Outro) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
            }
            break;
        case State::Outro:
            break;
    }
}