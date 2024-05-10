//
// Created by User on 2024/5/10.
//

#include "MovablePortal.hpp"
#include "Util/Time.hpp"

MovablePortal::MovablePortal(std::vector<std::string> image_paths, glm::vec2 position, AudioManager audiomanager)
    : Portal(image_paths, position, audiomanager) {}

void MovablePortal::Move(glm::vec2 target_position, float speed) {
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
