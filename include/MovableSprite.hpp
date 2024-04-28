//
// Created by User on 2024/3/24.
//

#ifndef LEVELDEVIL_MOVABLESPRITE_HPP
#define LEVELDEVIL_MOVABLESPRITE_HPP

#include "Sprite.hpp"
#include "Core/Drawable.hpp"
#include "Character.hpp"

class MovableSprite : public Sprite {
public:
    explicit MovableSprite(const std::shared_ptr<Core::Drawable> &drawable, float z_index = 0);

    void Move(glm::vec2 target_position, float speed, const Character& character);

    void Move(glm::vec2 target_position, float speed);
};


#endif //LEVELDEVIL_MOVABLESPRITE_HPP
