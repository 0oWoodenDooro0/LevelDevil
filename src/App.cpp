#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "CollisionHandler.hpp"
#include "Sprite.hpp"

void App::Start() {
    LOG_TRACE("Start");

    audio_maganer_ = AudioManager();

    background_ = std::make_shared<Background>();

    std::array<glm::vec2, 15> door_positions = {glm::vec2(-728, 4), glm::vec2(-580, -72), glm::vec2(-404, -8),
                                                glm::vec2(-236, -44), glm::vec2(-332, -184), glm::vec2(-284, -344),
                                                glm::vec2(-84, -324), glm::vec2(24, -192), glm::vec2(0, -28),
                                                glm::vec2(128, 32), glm::vec2(304, -80), glm::vec2(316, -244),
                                                glm::vec2(404, -364), glm::vec2(620, -372), glm::vec2(728, -260)};

    for (int i = 0; i < 15; ++i) {

        auto door_button = std::make_shared<Button<int>>(
                std::make_shared<Util::Image>(RESOURCE_DIR"/image/ui/unlock_door.png"),
                [&](int level) { level_ = level; });
        door_button->SetPosition(door_positions[i]);

        auto button_hover = std::make_shared<Sprite>(std::make_shared<Util::Animation>(
                std::vector<std::string>{(RESOURCE_DIR"/image/ui/door_hover1.png"),
                                         (RESOURCE_DIR"/image/ui/door_hover2.png"),
                                         (RESOURCE_DIR"/image/ui/door_hover3.png"),
                                         (RESOURCE_DIR"/image/ui/door_hover4.png")}, true, 200, true, 0), 10);
        button_hover->SetPosition(door_positions[i] + glm::vec2(0, 32));
        button_hover->SetVisible(false);

        door_buttons_.push_back(door_button);

        button_hovers_.push_back(button_hover);

        door_button->AddChild(button_hover);

        background_->AddChild(door_button);
    }

    root_.AddChild(background_);

    current_state_ = State::UPDATE;
}

void App::Update() {

    audio_maganer_.Update();

    for (int i = 0; i < int(door_buttons_.size()); ++i) {
        if (CollisionHandler::IsCollide(Util::Input::GetCursorPosition(), door_buttons_[i]->GetCollider())) {
            door_buttons_[i]->UpdateState(Button<int>::State::Hover);
            if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
                door_buttons_[i]->OnClick(i + 1);
            }
        } else {
            door_buttons_[i]->UpdateState(Button<int>::State::Idle);
        }
    }

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
