//
// Created by User on 2024/4/26.
//
#include "Level4.hpp"
#include "InputHandler.hpp"
#include "Util/Time.hpp"

#include <utility>

Level4::Level4(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
    : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void Level4::Start() {
    root_.AddChild(transition_.GetTop());
    root_.AddChild(transition_.GetBottom());
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/Level4/background.png");
    root_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_manager_);
    button_->SetPosition({ -800, 416 });
    root_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_manager_);
    character_->SetCheckPoint({ 704, -64 });
    root_.AddChild(character_);

    std::vector<std::string> img_paths = { (RESOURCE_DIR"/image/level/level4/door.png"),
        (RESOURCE_DIR"/image/level/level4/in_door1.png"),
        (RESOURCE_DIR"/image/level/level4/in_door4.png"),
        (RESOURCE_DIR"/image/level/level4/in_door3.png"),
        (RESOURCE_DIR"/image/level/level4/in_door4.png"),
        (RESOURCE_DIR"/image/level/level4/in_door5.png") };
    door_ = std::make_shared<Door>(audio_manager_, img_paths);
    door_->SetPosition({ -704, -64 });
    root_.AddChild(door_);
    auto wall_image = std::vector<std::string>{ RESOURCE_DIR"/image/level/Level4/hole.png",
                                               RESOURCE_DIR"/image/level/Level4/Lbottom.png",
                                               RESOURCE_DIR"/image/level/Level4/Lside.png",
                                               RESOURCE_DIR"/image/level/Level4/Ltop.png",
                                               RESOURCE_DIR"/image/level/Level4/Rbottom.png",
                                               RESOURCE_DIR"/image/level/Level4/Rtop.png" };
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

    auto spike_image = RESOURCE_DIR"/image/level/Level2/spike.png";
    for (int i = 0; i < 22; i++) {
        auto spike = std::make_shared<Spike>(spike_image, audio_manager_);
        spikes_.push_back(spike);
        root_.AddChild(spike);
        spike->SetPosition({ -640 + i * 64, -64 });
        spike->Disable();
    }

    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({ 128, 0 }, { 20, 1000 })));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({ -192, 0 }, { 20, 1000 })));
}

void Level4::Update() {
    if (character_->GetEnabled()) {
        if (character_->GetPosition().y < -480) {
            character_->UpdateState(Character::State::Dead);
        }
        auto input_vector = InputHandler::GetCharacterMoveVelocity();
        character_->Move(input_vector, walls_);
    }
    else {
        if (character_->GetCurrentState() != Character::State::LevelClear && InputHandler::isRevivePressed()) {
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

    for (const auto& spike : spikes_) {
        spike->Update(character_);
    }

    switch (current_state_) {
    case State::Intro:
        transition_.Intro([this]() { UpdateCurrentState(State::Start); });
        break;
    case State::Start:
        triggerColliders_[0]->Update(character_->GetPosition());
        if (triggerColliders_[0]->GetState() == TriggerCollider::State::Trigger) {
            UpdateCurrentState(State::CoinMove);
        }
        break;
    case State::CoinMove:
        break;
    case State::Outro:
        transition_.Outro([this]() { set_level_state_function_(level_); });
        break;
    }
    root_.Update();
}

void Level4::ResetLevel() {
    current_state_ = State::Start;
    for (int i = 0; i < 22; i++) {
        spikes_[i]->Disable();
    }
}

void Level4::UpdateCurrentState(State state) {
    if (current_state_ == state)return;
    switch (current_state_) {
    case State::Intro:
        if (state == State::Start) {
            current_state_ = state;
        }
        else if (state == State::Outro) {
            current_state_ = state;
            transition_.ResetTimer();
        }
        break;
    case State::Start:
        if (state == State::CoinMove) {
            current_state_ = state;
        }
        else if (state == State::Outro) {
            current_state_ = state;
            transition_.ResetTimer();
        }
        break;
    case State::CoinMove:
        if (state == State::Outro) {
            current_state_ = state;
            transition_.ResetTimer();
        }
        break;
    case State::Outro:
        break;
    }
}
