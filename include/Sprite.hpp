//
// Created by User on 2024/3/12.
//

#ifndef LEVELDEVIL_SPRITE_HPP
#define LEVELDEVIL_SPRITE_HPP


#include <string>
#include "Collider.hpp"
#include "GameObject.hpp"

class Sprite : public GameObject{
public:
    explicit Sprite(const std::shared_ptr<Core::Drawable> &drawable, float z_index = 0);
};


#endif //LEVELDEVIL_SPRITE_HPP
