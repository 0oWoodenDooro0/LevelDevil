//
// Created by User on 2024/3/21.
//

#include "Level1.hpp"
#include "InputHandler.hpp"
#include "Util/Time.hpp"

#include <utility>

Level1::Level1(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void Level1::Start() {
    renderer_.AddChild(transition_.GetTop());
    renderer_.AddChild(transition_.GetBottom());
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/level1/background.png");
    renderer_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_manager_);
    button_->SetPosition({-800, 416});
    renderer_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_manager_);
    character_->SetCheckPoint({-576, -128});
    renderer_.AddChild(character_);

    std::vector<std::string> img_paths = {RESOURCE_DIR"/image/level/level1/door.png",
                                          RESOURCE_DIR"/image/level/level1/in_door1.png",
                                          RESOURCE_DIR"/image/level/level1/in_door2.png",
                                          RESOURCE_DIR"/image/level/level1/in_door3.png",
                                          RESOURCE_DIR"/image/level/level1/in_door4.png",
                                          RESOURCE_DIR"/image/level/level1/in_door5.png"};
    door_ = std::make_shared<Door>(audio_manager_, img_paths);
    door_->SetPosition({576, -128});
    renderer_.AddChild(door_);
    auto wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/level1/top.png",
                                               RESOURCE_DIR"/image/level/level1/bottom.png",
                                               RESOURCE_DIR"/image/level/level1/side.png",
                                               RESOURCE_DIR"/image/level/level1/side.png"};
    for (int i = 0; i < 4; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image[i]));
        walls_.push_back(wall);
        renderer_.AddChild(wall);
    }
    walls_[0]->SetPosition({0, 320});
    walls_[1]->SetPosition({640, -320});
    walls_[2]->SetPosition({768, 0});
    walls_[3]->SetPosition({-768, 0});
    auto movable_wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/level1/move1.png",
                                                       RESOURCE_DIR"/image/level/level1/move2.png"};
    for (int i = 0; i < 2; ++i) {
        auto movable_wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(movable_wall_image[i]));
        walls_.push_back(movable_wall);
        renderer_.AddChild(movable_wall);
    }
    walls_[4]->SetPosition({-288, -320});
    walls_[5]->SetPosition({256, -320});

    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({-200, 0}, {20, 1000})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({275, 0}, {20, 1000})));
}

void Level1::Update() {
    if (character_->GetEnabled()) {
        if (character_->GetPosition().y < -480 && character_->GetCurrentState() != Character::State::Vanish) {
            character_->UpdateState(Character::State::Dead);
        }
        if (InputHandler::isGodPressed()) {
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

    switch (current_state_) {
        case State::Intro:
            transition_.Intro([this]() { UpdateCurrentState(State::Start); });
            break;
        case State::Start:
            triggerColliders_[0]->Update(character_->GetPosition());
            if (triggerColliders_[0]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move1);
            }
            break;
        case State::Move1:
            Movable::Move(walls_[4], {480, -320}, 750);
            triggerColliders_[1]->Update(character_->GetPosition());
            if (triggerColliders_[1]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move2);
            }
            break;
        case State::Move2:
            walls_[4]->SetPosition({100, 1000});
            walls_[4]->Disable();
            Movable::Move(walls_[5], {192, -320}, 500);
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

void Level1::ResetLevel() {
    character_->Revive();
    revive_timer_ = 500;
    walls_[4]->SetPosition({-288, -320});
    walls_[5]->SetPosition({256, -320});
    current_state_ = State::Start;
    walls_[4]->Enable();
}

void Level1::UpdateCurrentState(State state) {
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
            if (state == State::Move1) {
                current_state_ = state;
                audio_manager_.Play(AudioManager::SFX::WallTrap);
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Move1:
            if (state == State::Move2) {
                current_state_ = state;
                audio_manager_.Play(AudioManager::SFX::WallTrap);
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Move2:
            if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Outro:
            break;
    }
}
