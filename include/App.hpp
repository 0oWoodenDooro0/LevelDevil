#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Character.hpp"
#include "Util/Root.hpp"
#include "Background.hpp"
#include "Spike.hpp"
#include "AudioManager.hpp"
#include "Button.hpp"
#include "Sprite.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    [[nodiscard]] State GetCurrentState() const { return current_state_; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    State current_state_ = State::START;
    unsigned int level_ = 0;

    Util::Root root_;

    AudioManager audio_maganer_;
    std::shared_ptr<Background> background_;

    std::vector<std::shared_ptr<Button<int>>> door_buttons_;
    std::vector<std::shared_ptr<Sprite>> button_hovers_;

    std::vector<std::shared_ptr<Sprite>> walls_;
    std::vector<std::shared_ptr<Spike>> spikes_;
    std::shared_ptr<Character> character_;
};

#endif
