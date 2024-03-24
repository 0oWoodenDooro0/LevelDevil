//
// Created by User on 2024/3/24.
//

#include "MovableSprite.hpp"
#include "Util/Time.hpp"

MovableSprite::MovableSprite(const std::shared_ptr<Core::Drawable> &drawable, float z_index) : Sprite(drawable,
                                                                                                      z_index) {}

void MovableSprite::Move(glm::vec2 target_position, float speed) {
    auto vector = target_position - GetPosition();
    auto normalize = vector / hypot(vector.x, vector.y);
    if (hypot(vector.x, vector.y) != 0) {
        auto position = normalize * speed * float(Util::Time::GetDeltaTime());
        if (position.x > vector.x) {
            SetPosition(target_position);
        } else {
            SetPosition(GetPosition() + position);
        }
    }
}
