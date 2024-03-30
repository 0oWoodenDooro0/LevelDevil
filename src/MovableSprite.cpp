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
        auto position = speed * float(Util::Time::GetDeltaTime()) * normalize;
        if (abs(position.x) < abs(vector.x) || abs(position.y) < abs(vector.y)) {
            SetPosition(GetPosition() + position);
        } else {
            SetPosition(target_position);
        }
    }
}
