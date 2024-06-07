#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Level1.hpp"
#include "LevelSelect.hpp"
#include "Level2.hpp"
#include "Level3.hpp"
#include "Level4.hpp"
#include "Level5.hpp"
#include "Level7.hpp"
#include "Level8.hpp"
#include "LevelDevil.hpp"

void App::UpdateCurrentLevelState(Level::State level_state) {
    auto set_level_state_function = [this](Level::State state) { current_level_state_ = state; };
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
        case Level::State::LEVEL_4:
            level_ = std::make_unique<Level4>(audio_manager_, set_level_state_function);
            break;
        case Level::State::LEVEL_5:
            level_ = std::make_unique<Level5>(audio_manager_, set_level_state_function);
            break;
        case Level::State::LEVEL_7:
            level_ = std::make_unique<Level7>(audio_manager_, set_level_state_function);
            break;
        case Level::State::LEVEL_8:
            level_ = std::make_unique<Level8>(audio_manager_, set_level_state_function);
            break;
        case Level::State::LEVEL_DEVIL:
            level_ = std::make_unique<LevelDevil>(audio_manager_, set_level_state_function);
            break;
        case Level::State::END:
            current_state_ = State::END;
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
    if (Util::Input::IfExit()) {
        current_state_ = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
