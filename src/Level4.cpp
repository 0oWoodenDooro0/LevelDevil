//
// Created by User on 2024/4/26.
//

#include "Level4.hpp"
#include "InputHandler.hpp"
#include "Util/Time.hpp"
#include "Movable.hpp"

#include <utility>

Level4::Level4(AudioManager audio_manager, std::function<void(Level::State)> set_level_state_function)
        : set_level_state_function_(std::move(set_level_state_function)), audio_manager_(std::move(audio_manager)) {}

void Level4::Start() {
    renderer_.AddChild(transition_.GetTop());
    renderer_.AddChild(transition_.GetBottom());
    background_ = std::make_shared<Background>(RESOURCE_DIR"/image/level/Level4/background.png");
    renderer_.AddChild(background_);
    button_ = std::make_shared<EscButton>(audio_manager_);
    button_->SetPosition({-800, 416});
    renderer_.AddChild(button_);
    character_ = std::make_shared<Character>(audio_manager_);
    character_->SetCheckPoint({-704, -64});
    renderer_.AddChild(character_);
    door_ = std::make_shared<Door>(audio_manager_,
                                   std::vector<std::string>{RESOURCE_DIR"/image/level/level4/door.png",
                                                            RESOURCE_DIR"/image/level/level4/in_door1.png",
                                                            RESOURCE_DIR"/image/level/level4/in_door2.png",
                                                            RESOURCE_DIR"/image/level/level4/in_door3.png",
                                                            RESOURCE_DIR"/image/level/level4/in_door4.png",
                                                            RESOURCE_DIR"/image/level/level4/in_door5.png",});
    door_->SetPosition({768, -64});
    renderer_.AddChild(door_);
    auto wall_images = std::vector<std::string>{RESOURCE_DIR"/image/level/level4/bottom.png",
                                                RESOURCE_DIR"/image/level/level4/wall3.png",
                                                RESOURCE_DIR"/image/level/level4/wall2.png",
                                                RESOURCE_DIR"/image/level/level4/wall1.png",
                                                RESOURCE_DIR"/image/level/level4/wall3.png",
                                                RESOURCE_DIR"/image/level/level4/wall2.png",
                                                RESOURCE_DIR"/image/level/level4/wall1.png",
                                                RESOURCE_DIR"/image/level/level4/wall3.png",
                                                RESOURCE_DIR"/image/level/level4/wall2.png",
                                                RESOURCE_DIR"/image/level/level4/wall1.png",
                                                RESOURCE_DIR"/image/level/level4/wall5.png",
                                                RESOURCE_DIR"/image/level/level4/wall4.png",
                                                RESOURCE_DIR"/image/level/level4/wall3.png",
                                                RESOURCE_DIR"/image/level/level4/wall2.png",
                                                RESOURCE_DIR"/image/level/level4/wall1.png"};
    for (auto const &wall_image: wall_images) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(wall_image));
        walls_.push_back(wall);
        renderer_.AddChild(wall);
    }
    walls_[0]->SetPosition({0, -288});
    walls_[1]->SetPosition({-512, -64});
    walls_[2]->SetPosition({-480, 0});
    walls_[3]->SetPosition({-448, 64});
    walls_[4]->SetPosition({-192, -64});
    walls_[5]->SetPosition({-160, 0});
    walls_[6]->SetPosition({-128, 64});
    walls_[7]->SetPosition({128, -64});
    walls_[8]->SetPosition({160, 0});
    walls_[9]->SetPosition({192, 64});
    walls_[10]->SetPosition({512, -64});
    walls_[11]->SetPosition({544, 0});
    walls_[12]->SetPosition({576, 64});
    walls_[13]->SetPosition({608, 128});
    walls_[14]->SetPosition({640, 192});

    for (int i = 0; i < 15; ++i) {
        auto coin = std::make_shared<Coin>(audio_manager_,
                                           std::string{RESOURCE_DIR"/image/level/level4/coin.png"});
        coins_.push_back(coin);
        renderer_.AddChild(coin);
    }
    ResetCoins();

    triggerColliders_.push_back(std::make_shared<TriggerCollider>(Collider({224, 128}, {64, 1000})));
}

void Level4::Update() {
    if (character_->GetEnabled()) {
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

    for (const auto &coin: coins_) {
        coin->Update(character_);
        if (coin->GetState() == Coin::State::Trigger && character_->GetCurrentState() != Character::State::Vanish) {
            character_->UpdateState(Character::State::Dead);
        }
    }

    switch (current_state_) {
        case State::Intro:
            transition_.Intro([this]() { UpdateCurrentState(State::Start); });
            break;
        case State::Start:
            triggerColliders_[0]->Update(character_->GetPosition());
            if (triggerColliders_[0]->GetState() == TriggerCollider::State::Trigger) {
                UpdateCurrentState(State::Coin);
                audio_manager_.Play(AudioManager::SFX::WallTrap);
            }
            break;
        case State::Coin:
            MoveCoins();
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

void Level4::ResetLevel() {
    character_->Revive();
    revive_timer_ = 500;
    ResetCoins();
    current_state_ = State::Start;
}

void Level4::UpdateCurrentState(Level4::State state) {
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
            if (state == State::Coin) {
                current_state_ = state;
            } else if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Coin:
            if (state == State::Outro) {
                current_state_ = state;
                transition_.ResetTimer();
            }
            break;
        case State::Outro:
            break;
    }
}

void Level4::MoveCoins() const {
    for (const auto &coin: coins_) {
        Movable::Move(coin, {1728, coin->GetPosition().y}, coin_speed_);
    }
}

void Level4::ResetCoins() const {
    coins_[0]->SetPosition({-768, -64});
    coins_[1]->SetPosition({-576, 128});
    coins_[2]->SetPosition({-256, 128});
    coins_[3]->SetPosition({-64, -64});
    coins_[4]->SetPosition({64, 128});
    coins_[5]->SetPosition({320, -64});
    coins_[6]->SetPosition({-896, -64});
    coins_[7]->SetPosition({-896, 0});
    coins_[8]->SetPosition({-896, 64});
    coins_[9]->SetPosition({-896, 128});
    coins_[10]->SetPosition({-896, 192});
    coins_[11]->SetPosition({-896, 256});
    coins_[12]->SetPosition({-896, 320});
    coins_[13]->SetPosition({-896, 384});
    coins_[14]->SetPosition({-896, 448});
    for (const auto &coin: coins_) {
        coin->Enable();
    }
}
