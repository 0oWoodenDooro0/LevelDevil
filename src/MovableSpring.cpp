//
// Created by User on 2024/5/3.
//

#include "MovableSpring.hpp"
#include "Util/Time.hpp"

MovableSpring::MovableSpring(std::vector<std::string> image_paths, AudioManager audio_manager) : Spring(image_paths, audio_manager) {}

void MovableSpring::Move(glm::vec2 target_position, float speed) {
    auto vector = target_position - GetPosition();
    auto normalize = vector / hypot(vector.x, vector.y);
    if (hypot(vector.x, vector.y) != 0) {
        auto position = speed * Util::Time::GetDeltaTimeMs() / 1000 * normalize;
        auto new_position = glm::vec2(int(position.x), int(position.y));
        if (abs(new_position.x) < abs(vector.x) || abs(new_position.y) < abs(vector.y)) {
            SetPosition(GetPosition() + new_position);
        }
        else {
            SetPosition(target_position);
        }
    }
}
