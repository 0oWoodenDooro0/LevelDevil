#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Level1.hpp"
#include "LevelSelect.hpp"
#include "Level2.hpp"
#include "Level3.hpp"

void App::UpdateCurrentLevelState(Level::State level_state) {
    auto set_level_state_function = [this](Level::State level_state) { current_level_state_ = level_state; };
    switch (level_state) {
        case Level::State::LEVEL_SELECT:
            level_ = std::make_unique<LevelSelect>(audio_manager_, set_level_state_function);
            break;
        case Level::State::LEVEL_1:
            level_ = std::make_unique<Level1>(audio_manager_, set_level_state_function);
            break;
        case Level::State::LEVEL_2:
            level_ = std::make_unique<Level2>(audio_manager_, set_level_state_function);
            break;
        case Level::State::LEVEL_3:
            level_ = std::make_unique<Level3>(audio_manager_, set_level_state_function);
            break;
    }
    level_->Start();
}

void App::Start() {
    LOG_TRACE("Start");

    UpdateCurrentLevelState(Level::State::LEVEL_SELECT);

    current_state_ = State::UPDATE;
}

void App::Update() {
    Level::State level_state = current_level_state_;

    audio_manager_.Update();
    level_->Update();

    if (level_state != current_level_state_) {
        UpdateCurrentLevelState(current_level_state_);
    }

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        current_state_ = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
