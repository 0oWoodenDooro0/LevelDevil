//
// Created by User on 2024/3/21.
//

#ifndef LEVELDEVIL_LEVEL1_HPP
#define LEVELDEVIL_LEVEL1_HPP

#include "ILevel.hpp"
#include "AudioManager.hpp"
#include "Door.hpp"
#include "Character.hpp"
#include "Level.hpp"
#include "Background.hpp"
#include "Util/Root.hpp"

class Level1 : public ILevel {
public:
    explicit Level1(std::function<void(Level::State)> set_level_state_function);

    void Start() override;

    void Update() override;

    void End() override;

private:
    Util::Root root_;

    std::function<void(Level::State)> set_level_state_function_;

    AudioManager audio_maganer_;
    std::shared_ptr<Background> background_;
    std::vector<std::shared_ptr<Sprite>> walls_;
    std::shared_ptr<Door> door_;
    std::shared_ptr<Character> character_;
};

#endif //LEVELDEVIL_LEVEL1_HPP
