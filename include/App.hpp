#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Character.hpp"
#include "Util/Root.hpp"
#include "Background.hpp"
#include "Wall.hpp"
#include "spike.hpp"

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
    Util::Root root_;

    std::shared_ptr<Background> background_;
    std::vector<std::shared_ptr<Wall>> walls_;
    std::vector<std::shared_ptr<Spike>> spikes_;
    std::shared_ptr<Character> character_;
};

#endif
