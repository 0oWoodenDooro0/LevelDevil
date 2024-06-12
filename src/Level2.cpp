//
// Created by User on 2024/3/21.
//
#include "Level2.hpp"
#include "InputHandler.hpp"
#include "Util/Time.hpp"

#include <utility>

Level2::Level2(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void Level2::Start() {
    renderer_.AddChild(transition_.GetTop());
    renderer_.AddChild(transition_.GetBottom());
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/Level2/background.png");
    renderer_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_manager_);
    button_->SetPosition({-800, 416});
    renderer_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_manager_);
    character_->SetCheckPoint({704, -64});
    renderer_.AddChild(character_);

    std::vector<std::string> img_paths = {RESOURCE_DIR"/image/level/level2/door.png",
                                          RESOURCE_DIR"/image/level/level2/in_door1.png",
                                          RESOURCE_DIR"/image/level/level2/in_door2.png",
                                          RESOURCE_DIR"/image/level/level2/in_door3.png",
                                          RESOURCE_DIR"/image/level/level2/in_door4.png",
                                          RESOURCE_DIR"/image/level/level2/in_door5.png"};
    door_ = std::make_shared<Door>(audio_manager_, img_paths);
    door_->SetPosition({-704, -64});
    renderer_.AddChild(door_);
    auto wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/Level2/hole.png",
                                               RESOURCE_DIR"/image/level/Level2/Lbottom.png",
                                               RESOURCE_DIR"/image/level/Level2/Lside.png",
                                               RESOURCE_DIR"/image/level/Level2/Ltop.png",
                                               RESOURCE_DIR"/image/level/Level2/Rbottom.png",
                                               RESOURCE_DIR"/image/level/Level2/Rtop.png"};
    for (int i = 0; i < 6; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image[i]));
        walls_.push_back(wall);
        renderer_.AddChild(wall);
    }
    walls_[0]->SetPosition({768, -320});
    walls_[1]->SetPosition({-64, -288});
    walls_[2]->SetPosition({-832, 0});
    walls_[3]->SetPosition({-736, 256});
    walls_[4]->SetPosition({832, -288});
    walls_[5]->SetPosition({128, 224});

    auto spike_image = RESOURCE_DIR"/image/level/Level2/spike.png";
    for (int i = 0; i < 22; i++) {
        auto spike = std::make_shared<Spike>(spike_image, Spike::Position::Bottom, audio_manager_);
        spikes_.push_back(spike);
        renderer_.AddChild(spike);
        spike->SetPosition({-640 + i * 64, -64});
        spike->Disable();
    }

    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({128, 0}, {20, 1000})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({-192, 0}, {20, 1000})));
}

void Level2::Update() {
    if (character_->GetEnabled()) {
        if (character_->GetPosition().y < -480 && character_->GetCurrentState() != Character::State::Vanish) {
            character_->UpdateState(Character::State::Dead);
        }
        if (InputHandler::isGodPressed()){
            character_->ChangeGod();
        }
        glm::vec2 input_velocity = {0, 0};
        if (character_->GetGod()) {
            input_velocity = InputHandler::GetGodMoveVelocity();
        } else {
            input_velocity = InputHandler::GetCharacterMoveVelocity();
        }
        character_->Update(input_velocity, walls_);
    } else {
        if (revive_timer_ > 0) {
            revive_timer_ -= Util::Time::GetDeltaTimeMs();
        } else if (character_->GetCurrentState() != Character::State::LevelClear && InputHandler::isRevivePressed()) {
            ResetLevel();
        }
    }

    if (InputHandler::isResetLevelPressed() && door_->GetState() == Door::State::Idle) {
        ResetLevel();
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

    for (const auto &spike: spikes_) {
        spike->Update(character_);
    }

    switch (current_state_) {
        case State::Intro:
            transition_.Intro([this]() { UpdateCurrentState(State::Start); });
            break;
        case State::Start:
            triggerColliders_[0]->Update(character_->GetPosition());
            if (triggerColliders_[0]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Spike1);
                spike_num_ = 0;
                timer_ = 110;
            }
            break;
        case State::Spike1:
            if (spike_num_ < 26) Spike1Act();
            triggerColliders_[1]->Update(character_->GetPosition());
            if (triggerColliders_[1]->GetState() == TriggerCollider::State::Trigger && spike_num_ == 26) {
                UpdateCurrentState(State::Spike2);
                spike_num_ = 21;
                timer_ = 70;
            }
            break;
        case State::Spike2:
            if (spike_num_ >= 0) Spike2Act();
            break;
        case State::Outro:
            transition_.Outro([this]() { set_level_state_function_(level_); });
            break;
    }
    renderer_.Update();
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        UpdateCurrentState(State::Outro);
        level_ = Level::State::LEVEL_SELECT;
    }
}

void Level2::ResetLevel() {
    character_->Revive();
    revive_timer_ = 500;
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
                transition_.ResetTimer();
            }
            break;
        case State::Start:
            if (state == State::Spike1) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Spike1:
            if (state == State::Spike2) {
                current_state_ = state;
            }
            if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Spike2:
            if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Outro:
            break;
    }
}

void Level2::Spike1Act() {
    timer_ -= float(Util::Time::GetDeltaTimeMs());
    if (timer_ <= 0) {
        if (spike_num_ < 22) {
            spikes_[spike_num_]->Enable();
        }
        if (spike_num_ > 3) {
            spikes_[spike_num_ - 4]->Disable();
        }
        timer_ = 110;
        spike_num_++;
    }
}

void Level2::Spike2Act() {
    timer_ -= float(Util::Time::GetDeltaTimeMs());
    if (timer_ <= 0) {
        if (spike_num_ >= 0) {
            spikes_[spike_num_]->Enable();
        }
        timer_ = 70;
        spike_num_--;
    }
}