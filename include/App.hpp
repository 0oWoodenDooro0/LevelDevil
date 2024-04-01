#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "ILevel.hpp"
#include "Level.hpp"
#include "AudioManager.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END
    };

    [[nodiscard]] inline State GetCurrentState() const { return current_state_; }

    void UpdateCurrentLevelState(Level::State level_state);

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    State current_state_ = State::START;
    Level::State current_level_state_ = Level::State::LEVEL_SELECT;

    std::unique_ptr<ILevel> level_ = nullptr;
    AudioManager audio_manager_;
};

#endif
