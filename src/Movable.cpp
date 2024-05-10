//
// Created by User on 2024/5/10.
//

#include "Movable.hpp"

#include "Util/Time.hpp"

void Movable::Move(const std::shared_ptr<GameObject> &gameObject, glm::vec2 target_position, float speed) {
    auto vector = target_position - gameObject->GetPosition();
    auto normalize = vector / hypot(vector.x, vector.y);
    if (hypot(vector.x, vector.y) != 0) {
        auto position = speed * Util::Time::GetDeltaTimeMs() / 1000 * normalize;
        auto new_position = glm::vec2(int(position.x), int(position.y));
        if (abs(new_position.x) < abs(vector.x) || abs(new_position.y) < abs(vector.y)) {
            gameObject->SetPosition(gameObject->GetPosition() + new_position);
        } else {
            gameObject->SetPosition(target_position);
        }
    }
}
