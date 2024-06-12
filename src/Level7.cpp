//
// Created by User on 2024/3/21.
//

#include "Level7.hpp"
#include "InputHandler.hpp"
#include "Util/Time.hpp"
#include "Movable.hpp"

#include <utility>

Level7::Level7(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void Level7::Start() {
    renderer_.AddChild(transition_.GetTop());
    renderer_.AddChild(transition_.GetBottom());
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/Level7/background.png");
    renderer_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_manager_);
    button_->SetPosition({-800, 416});
    renderer_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_manager_);
    character_->SetCheckPoint({-640, -64});
    renderer_.AddChild(character_);

    std::vector<std::string> img_paths = {RESOURCE_DIR"/image/level/Level7/door.png",
                                          RESOURCE_DIR"/image/level/Level7/in_door1.png",
                                          RESOURCE_DIR"/image/level/Level7/in_door2.png",
                                          RESOURCE_DIR"/image/level/Level7/in_door3.png",
                                          RESOURCE_DIR"/image/level/Level7/in_door4.png",
                                          RESOURCE_DIR"/image/level/Level7/in_door5.png"};
    door_ = std::make_shared<Door>(audio_manager_, img_paths);
    door_->SetPosition({576, -64});
    renderer_.AddChild(door_);
    auto wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/Level7/top.png",
                                               RESOURCE_DIR"/image/level/Level7/bottom.png",
                                               RESOURCE_DIR"/image/level/Level7/bottom.png",
                                               RESOURCE_DIR"/image/level/Level7/side.png",
                                               RESOURCE_DIR"/image/level/Level7/side.png"};
    for (int i = 0; i < 5; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image[i]));
        walls_.push_back(wall);
        renderer_.AddChild(wall);
    }
    walls_[0]->SetPosition({0, 448});
    walls_[1]->SetPosition({-640, -288});
    walls_[2]->SetPosition({640, -288});
    walls_[3]->SetPosition({-832, 0});
    walls_[4]->SetPosition({832, 0});
    auto movable_wall_image = std::vector<std::string>{RESOURCE_DIR"/image/level/Level7/springBase.png",
                                                       RESOURCE_DIR"/image/level/Level7/springBase.png",
                                                       RESOURCE_DIR"/image/level/Level7/springBase.png",
                                                       RESOURCE_DIR"/image/level/Level7/springBase.png"};
    for (int i = 0; i < 4; ++i) {
        auto movable_wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(movable_wall_image[i]));
        walls_.push_back(movable_wall);
        renderer_.AddChild(movable_wall);
        movable_wall->SetPosition({-320 + 192 * i, -256});
    }

    auto movable_spring_image = std::vector<std::string>{RESOURCE_DIR"/image/level/Level7/springDown.png",
                                                         RESOURCE_DIR"/image/level/Level7/springUp.png"};
    for (int i = 0; i < 4; i++) {
        auto movable_spring = std::make_shared<Spring>(movable_spring_image, audio_manager_);
        springs_.push_back(movable_spring);
        walls_.push_back(movable_spring);
        renderer_.AddChild(movable_spring);
        movable_spring->SetPosition({-320 + 192 * i, -192});
    }

    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({-384, 0}, {20, 1000})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({-192, 0}, {20, 1000})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({0, 0}, {20, 1000})));
    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({192, 0}, {20, 1000})));
}

void Level7::Update() {
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

    for (const auto &spring: springs_) {
        spring->Update(character_);
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
            SpringMove(64);
            triggerColliders_[1]->Update(character_->GetPosition());
            if (triggerColliders_[1]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move2);
            }
            break;
        case State::Move2:
            SpringMove(0);
            triggerColliders_[2]->Update(character_->GetPosition());
            if (triggerColliders_[2]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move3);
            }
            break;
        case State::Move3:
            SpringMove(64);
            triggerColliders_[3]->Update(character_->GetPosition());
            if (triggerColliders_[3]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Move4);
            }
            break;
        case State::Move4:
            SpringMove(128);
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

void Level7::ResetLevel() {
    character_->Revive();
    revive_timer_ = 500;
    for (int i = 0; i < 4; i++) {
        walls_[i + 5]->SetPosition({-320 + 192 * i, -256});
        springs_[i]->SetPosition({-320 + 192 * i, -192});
        springs_[i]->Reset();
    }
    current_state_ = State::Start;
}

void Level7::UpdateCurrentState(State state) {
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
            if (state == State::Move3) {
                current_state_ = state;
                audio_manager_.Play(AudioManager::SFX::WallTrap);
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Move3:
            if (state == State::Move4) {
                current_state_ = state;
                audio_manager_.Play(AudioManager::SFX::WallTrap);
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Move4:
            if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Outro:
            break;
    }
}

void Level7::SpringMove(float distance) {
    for (int i = 0; i < 4; i++) {
        auto X = -320 + 192 * i;
        auto Ywall = walls_[i + 5]->GetPosition().y;
        auto Yspring = springs_[i]->GetPosition().y;
        Movable::Move(walls_[i + 5], {X + distance, Ywall}, 300);
        Movable::Move(springs_[i], {X + distance, Yspring}, 300);
    }
}