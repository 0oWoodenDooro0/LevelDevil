#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "CollisionHandler.hpp"
#include "Sprite.hpp"

void App::Start() {
    LOG_TRACE("Start");

    std::array<glm::vec2, 15> door_positions = {glm::vec2(-728, 4), glm::vec2(-580, -72), glm::vec2(-404, -8),
                                                glm::vec2(-236, -44), glm::vec2(-332, -184), glm::vec2(-284, -344),
                                                glm::vec2(-84, -324), glm::vec2(24, -192), glm::vec2(0, -28),
                                                glm::vec2(128, 32), glm::vec2(304, -80), glm::vec2(316, -244),
                                                glm::vec2(404, -364), glm::vec2(620, -372), glm::vec2(728, -260)};

    for (int i = 0; i < 15; ++i) {

        auto door_button = std::make_shared<DoorButton>(
                std::make_shared<Util::Image>(RESOURCE_DIR"/image/ui/unlock_door.png"));
        door_button->SetPosition(door_positions[i]);
        door_button->Disable();

        auto button_hover = std::make_shared<Sprite>(std::make_shared<Util::Animation>(
                std::vector<std::string>{(RESOURCE_DIR"/image/ui/door_hover1.png"),
                                         (RESOURCE_DIR"/image/ui/door_hover2.png"),
                                         (RESOURCE_DIR"/image/ui/door_hover3.png"),
                                         (RESOURCE_DIR"/image/ui/door_hover4.png")}, true, 200, true, 0), 11);
        button_hover->SetPosition(door_positions[i] + glm::vec2(0, 32));
        button_hover->Disable();

        door_buttons_.push_back(door_button);

        button_hovers_.push_back(button_hover);

        door_button->AddChild(button_hover);

        root_.AddChild(door_button);
    }

    for (int i = 0; i < 10; ++i) {
        auto wall = std::make_shared<Sprite>(std::make_shared<Util::Image>(RESOURCE_DIR"/image/wall/test_wall.png"));
        wall->Disable();
        walls_.push_back(wall);
        root_.AddChild(wall);
    }

    root_.AddChild(character_);
    root_.AddChild(door_);

    LoadLevel();

    root_.AddChild(background_);

    current_state_ = State::UPDATE;
}

void App::Update() {

    audio_maganer_.Update();

    UpdateLevel();

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        current_state_ = State::END;
    }
    root_.Update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

void App::LoadLevel() {
    for (const auto &door_button: door_buttons_) {
        door_button->Disable();
    }
    for (const auto &wall: walls_) {
        wall->Disable();
    }
    character_->Disable();
    door_->Disable();
    switch (current_level_) {
        case Level::LEVEL_SELECT:
            for (const auto &door_button: door_buttons_) {
                door_button->Enable();
            }
            break;
        case Level::LEVEL_1:
            background_->SetDrawable(
                    std::make_shared<Util::Image>(RESOURCE_DIR"/image/background/test_background.png"));
            character_->SetPosition({0, 0});
            character_->Enable();
            door_->SetPosition({128, -136});
            door_->Enable();
            for (int i = 0; i < 9; ++i) {
                walls_[i]->Enable();
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
            break;
    }
}

void App::UpdateLevel() {
    switch (current_level_) {
        case Level::LEVEL_SELECT:
            for (int i = 0; i < int(door_buttons_.size()); ++i) {
                door_buttons_[i]->Update();
                if (door_buttons_[i]->GetState() == Button::State::Click) {
                    current_level_ = Level::LEVEL_1;
                    LoadLevel();
                }
            }
            break;
        case Level::LEVEL_1:
            character_->Update(walls_);
            door_->Update(character_);
            break;
    }
}
