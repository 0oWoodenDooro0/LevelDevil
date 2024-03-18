#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Character.hpp"
#include "Util/Root.hpp"
#include "Background.hpp"
#include "Spike.hpp"
#include "Door.hpp"
#include "AudioManager.hpp"
#include "Sprite.hpp"
#include "DoorButton.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    enum class Level{
        LEVEL_SELECT,
        LEVEL_1
    };

    [[nodiscard]] State GetCurrentState() const { return current_state_; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    void LoadLevel();

    void UpdateLevel();

private:
    State current_state_ = State::START;
    Level current_level_ = Level::LEVEL_SELECT;

    Util::Root root_;

    AudioManager audio_maganer_ = AudioManager();
    std::shared_ptr<Background> background_ = std::make_shared<Background>();

    std::vector<std::shared_ptr<DoorButton>> door_buttons_;
    std::vector<std::shared_ptr<Sprite>> button_hovers_;

    std::vector<std::shared_ptr<Sprite>> walls_;
    std::vector<std::shared_ptr<Spike>> spikes_;
    std::shared_ptr<Door> door_ = std::make_shared<Door>();
    std::shared_ptr<Character> character_ = std::make_shared<Character>(audio_maganer_);
};

#endif
