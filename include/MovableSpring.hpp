//
// Created by User on 2024/5/3.
//

#ifndef LEVELDEVIL_MOVABLESPRING_HPP
#define LEVELDEVIL_MOVABLESPRING_HPP

#include "Spring.hpp"
#include "Core/Drawable.hpp"

class MovableSpring : public Spring {
public:
    explicit MovableSpring(const std::vector<std::string> &image_paths, AudioManager audio_manager);

    void Move(glm::vec2 target_position, float speed);
};


#endif //LEVELDEVIL_MOVABLESPRING_HPP
